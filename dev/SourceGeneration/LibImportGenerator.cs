using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;

namespace Gluino.SourceGeneration;

[Generator]
public class LibImportGenerator : ISourceGenerator
{
    public void Initialize(GeneratorInitializationContext context)
    {
        context.RegisterForSyntaxNotifications(() => new SyntaxReceiver());
    }

    public void Execute(GeneratorExecutionContext context)
    {
        if (context.SyntaxReceiver is not SyntaxReceiver receiver)
            return;

        var compilation = context.Compilation;
        var libDetailsAttributeSymbol = compilation.GetTypeByMetadataName("Gluino.Native.LibDetailsAttribute");
        var libImportAttributeSymbol = compilation.GetTypeByMetadataName("Gluino.Native.LibImportAttribute");

        foreach (var classDeclaration in receiver.CandidateClasses) {
            var classModel = compilation.GetSemanticModel(classDeclaration.SyntaxTree);
            var classSymbol = ModelExtensions.GetDeclaredSymbol(classModel, classDeclaration);

            if (!classDeclaration.IsPartial() ||
                classSymbol == null ||
                !classSymbol.TryGetAttribute(libDetailsAttributeSymbol, out var libDetailsAttributeData))
                continue;

            var namespaceName = classSymbol.ContainingNamespace.IsGlobalNamespace ? "" : classSymbol.ContainingNamespace.ToDisplayString();
            var className = classDeclaration.Identifier.Text;
            var libName = libDetailsAttributeData.ConstructorArguments[0].Value?.ToString();

            if (libName == null)
                throw new Exception($"LibName is not specified for {className}");
            
            var nativeMethods = new List<NativeMethod>();
            foreach (var methodDeclaration in classDeclaration.DescendantNodes().OfType<MethodDeclarationSyntax>()) {
                var methodModel = compilation.GetSemanticModel(methodDeclaration.SyntaxTree);
                var methodSymbol = ModelExtensions.GetDeclaredSymbol(methodModel, methodDeclaration) as IMethodSymbol;

                if (!methodDeclaration.IsPartial() ||
                    methodSymbol == null ||
                    !methodSymbol.TryGetAttribute(libImportAttributeSymbol, out var libImportAttributeData))
                    continue;

                if (className == "NativeWindow")
                {
                    ;
                }

                var methodName = methodDeclaration.Identifier.Text;
                var methodSignature = methodSymbol.GetMethodSignature();

                //var libNameOverride = libImportAttributeData.GetFieldValue("LibName").Value?.ToString() ?? libName;

                var libNameOverride = libImportAttributeData.ConstructorArguments.Length switch {
                    2 => libImportAttributeData.ConstructorArguments[0].Value?.ToString(),
                    _ => libImportAttributeData.NamedArguments
                        .FirstOrDefault(kv => kv.Key == "LibName")
                        .Value.Value?.ToString()
                } ?? libName;
                var entryPoint = libImportAttributeData.ConstructorArguments.Length switch {
                    1 => libImportAttributeData.ConstructorArguments[0].Value?.ToString(),
                    2 => libImportAttributeData.ConstructorArguments[1].Value?.ToString(),
                    _ => libImportAttributeData.NamedArguments
                        .FirstOrDefault(kv => kv.Key == "EntryPoint")
                        .Value.Value?.ToString()
                } ?? methodName;
                
                var callingConvention = libImportAttributeData.GetFieldValueCSharpString("CallingConvention", "CallingConvention.Cdecl");
                var charSet = libImportAttributeData.GetFieldValueCSharpString("CharSet", "CharSet.Auto");
                var setLastError = libImportAttributeData.GetFieldValueCSharpString("SetLastError", "true");

                nativeMethods.Add(new NativeMethod(
                    methodSignature,
                    new(libNameOverride, entryPoint, callingConvention, charSet, setLastError))
                );
            }

            var source = GeneratePartialClassSource(namespaceName, className, libName, nativeMethods);
            context.AddSource($"{className}.Generated", source);
        }
    }

    private string GeneratePartialClassSource(string namespaceName, string className, string libName, List<NativeMethod> nativeMethdos)
    {
        var methodsSourceBuilder = new StringBuilder();
        
        foreach (var nativeMethod in nativeMethdos) {
            methodsSourceBuilder.AppendLine($"""    [DllImport("{nativeMethod.Import.LibName ?? libName}", EntryPoint = "{nativeMethod.Import.EntryPoint}", CallingConvention = {nativeMethod.Import.CallingConvention}, CharSet = {nativeMethod.Import.CharSet}, SetLastError = {nativeMethod.Import.SetLastError})]""");
            methodsSourceBuilder.AppendLine($"    public static extern partial {nativeMethod.Signature};");
            methodsSourceBuilder.AppendLine();
        }

        var methodsSource = methodsSourceBuilder.ToString().Trim();

        return $$"""
         using System.Runtime.InteropServices;
         
         namespace {{namespaceName}};
         
         internal partial class {{className}}
         {
            {{methodsSource}}
         }
         """;
    }

    private class SyntaxReceiver : ISyntaxReceiver
    {
        public List<ClassDeclarationSyntax> CandidateClasses { get; } = [];

        public void OnVisitSyntaxNode(SyntaxNode syntaxNode)
        {
            if (syntaxNode is ClassDeclarationSyntax { AttributeLists.Count: > 0 } cds) {
                CandidateClasses.Add(cds);
            }
        }
    }

    private class NativeMethod(string signature, LibImport import)
    {
        public readonly string Signature = signature;
        public readonly LibImport Import = import;
    }

    private class LibImport(string libName, string entryPoint, string callingConvention, string charSet, string setLastError)
    {
        public readonly string LibName = libName;
        public readonly string EntryPoint = entryPoint;
        public readonly string CallingConvention = callingConvention;
        public readonly string CharSet = charSet;
        public readonly string SetLastError = setLastError;
    }
}

public static class Extensions
{
    public static bool TryGetAttribute(this ISymbol symbol, INamedTypeSymbol attributeSymbol, out AttributeData attributeData)
    {  
        attributeData = symbol
            .GetAttributes()
            .FirstOrDefault(ad => 
                ad.AttributeClass?.Equals(attributeSymbol, SymbolEqualityComparer.Default) ?? false);
        return attributeData != null;
    }

    public static bool IsPartial(this MemberDeclarationSyntax methodDeclaration)
    {
        return methodDeclaration.Modifiers.Any(m => m.IsKind(SyntaxKind.PartialKeyword));
    }
    
    public static TypedConstant GetFieldValue(this AttributeData attributeData, string name)
    {
        return attributeData.NamedArguments.FirstOrDefault(kvp => kvp.Key == name).Value;
    }

    public static string GetFieldValueCSharpString(this AttributeData attributeData, string name, string defaultValue)
    {
        var dict = attributeData.NamedArguments.ToDictionary(kv => kv.Key, kv => kv.Value);
        return dict.ContainsKey(name) ? dict[name].ToCSharpString() : defaultValue;
    }

    public static string GetMethodSignature(this IMethodSymbol methodSymbol)
    {
        var returnType = methodSymbol.ReturnType.ToDisplayString();
        var methodName = methodSymbol.Name;
        var refKeyword = methodSymbol.RefKind switch {
            RefKind.None => "",
            RefKind.Ref => "ref ",
            RefKind.Out => "out ",
            RefKind.In => "in ",
            _ => throw new Exception($"Unknown RefKind: {methodSymbol.RefKind}")
        };
        //var parameters = string.Join(", ", methodSymbol.Parameters.Select(p => $"{refKeyword}{p.Type} {p.Name}"));
        var parameters = string.Join(", ", methodSymbol.Parameters.Select(p => p.OriginalDefinition));
        var methodSignature = $"{returnType} {methodName}({parameters})";

        return methodSignature;
    }
}

