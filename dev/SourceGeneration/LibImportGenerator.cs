using System;
using System.Collections.Generic;
using System.Linq;
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
            var classSymbol = classModel.GetDeclaredSymbol(classDeclaration);

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
                
                var methodName = methodDeclaration.Identifier.Text;
                var methodSignature = methodSymbol.GetMethodSignature();

                //var libNameOverride = libImportAttributeData.GetFieldTypeConstant("LibName").Value?.ToString() ?? libName;

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

                nativeMethods.Add(new(
                    methodName,
                    methodSignature,
                    new(
                        libNameOverride,
                        entryPoint,
                        libImportAttributeData.GetFieldValueCSharpString("CallingConvention", "CallingConvention.Cdecl"),
                        libImportAttributeData.GetFieldValueCSharpString("CharSet", "CharSet.Auto"),
                        libImportAttributeData.GetFieldValueCSharpString("SetLastError", "true"),
                        libImportAttributeData.GetFieldValue("IsStatic", false),
                        libImportAttributeData.GetFieldValue("Managed", false),
                        libImportAttributeData.GetFieldValue("PropertyData", ManagedProperty.None),
                        libImportAttributeData.GetFieldValue<string>("Option")),
                    methodSymbol
                ));
            }

            var source = GeneratePartialClassSource(namespaceName, className, libName, nativeMethods);
            context.AddSource($"{className}.Generated", source);

            GenerateManagedClassSource(context, libDetailsAttributeData, nativeMethods);
        }
    }

    private string GeneratePartialClassSource(string namespaceName, string className, string libName, List<NativeMethod> nativeMethods)
    {
        var methodsSourceBuilder = new StringBuilder();
        
        foreach (var nativeMethod in nativeMethods) {
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

    private void GenerateManagedClassSource(
        GeneratorExecutionContext context,
        AttributeData libDetailsAttributeData,
        List<NativeMethod> nativeMethods)
    {
        if (!libDetailsAttributeData.HasFieldValue("ManagedType")) return;
        if (!nativeMethods.Any(nm => nm.Import.Managed)) return;

        var managedTypeConstant = libDetailsAttributeData.GetFieldTypeConstant("ManagedType");
        if (managedTypeConstant.Value is not INamedTypeSymbol managedTypeSymbol) return;

        var namespaceName = managedTypeSymbol.ContainingNamespace.IsGlobalNamespace ? "" : managedTypeSymbol.ContainingNamespace.ToDisplayString();
        var managedClassName = managedTypeSymbol.Name;

        var properties = nativeMethods
            .Where(nm => nm.Import.Managed && nm.Import.Property != ManagedProperty.None)
            .GroupBy(m => m.Import.Option)
            .Select(g => {
                var getter = g.FirstOrDefault(nm => nm.Import.Property == ManagedProperty.Get);
                var setter = g.FirstOrDefault(nm => nm.Import.Property == ManagedProperty.Set);
                return new PropertyData(g.Key, getter, setter);
            }).ToList();

        var propSources = new List<string>();
        foreach (var property in properties) {
            propSources.Add(GenerateManagedPropertySource(property));
        }
        var propsSource = string.Join("\r\n\r\n\r\n", propSources);

        var source =
            $$"""
              using System.Drawing;
              using Gluino.Native;

              namespace {{namespaceName}};

              public partial class {{managedClassName}}
              {
              {{propsSource}}
              }
              """;


        context.AddSource($"{managedClassName}.Generated", source);
    }
    
    private string GenerateManagedPropertySource(PropertyData propertyData)
    {
        var className = propertyData.Getter.Symbol.ContainingType.Name;

        var getType = propertyData.Getter.Symbol.ReturnType.ToDisplayString();
        var getSource =
            $$"""
                private {{(propertyData.Getter.Import.IsStatic ? "static " : "")}}{{getType}} {{propertyData.Getter.Name}}()
                {
                   if (_nativeInstance == nint.Zero) {
                       {{(propertyData.Getter.Symbol.ReturnType.IsString() ? $"return App.Platform.IsWindows ? _nativeOptions.{propertyData.Option}W : _nativeOptions.{propertyData.Option}A" : $"return _nativeOptions.{propertyData.Option}")}};
                   }
                   return Invoke(() => {{className}}.{{propertyData.Getter.Name}}(_nativeInstance));
                }
            """;

        var setParam = propertyData.Setter.Symbol.Parameters[1];
        var setParamType = setParam.Type.ToDisplayString();
        var setParamName = setParam.Name;
        var setSource =
            $$"""
                private {{(propertyData.Setter.Import.IsStatic ? "static " : "")}}void {{propertyData.Setter.Name}}({{setParamType}} {{setParamName}})
                {
                    if (_nativeInstance == nint.Zero) {
                        {{(setParam.Type.IsString() ? $"if (App.Platform.IsWindows) {{\r\n                _nativeOptions.{propertyData.Option}W = {setParamName};\r\n            }}\r\n            else {{\r\n                _nativeOptions.{propertyData.Option}A = {setParamName};\r\n            }}" : $"_nativeOptions.{propertyData.Option} = {setParamName};")}}
                    }
                    else {
                        Invoke(() => {{className}}.{{propertyData.Setter.Name}}(_nativeInstance, {{setParamName}}));
                    }
                }
            """;

        return $"{getSource}\r\n\r\n\r\n{setSource}";
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

    private class NativeMethod(string name, string signature, LibImport import, IMethodSymbol symbol)
    {
        public readonly string Name = name;
        public readonly string Signature = signature;
        public readonly LibImport Import = import;
        public readonly IMethodSymbol Symbol = symbol;
    }

    private class LibImport(
        string libName,
        string entryPoint,
        string callingConvention,
        string charSet,
        string setLastError,
        bool isStatic,
        bool managed,
        ManagedProperty property,
        string option)
    {
        public readonly string LibName = libName;
        public readonly string EntryPoint = entryPoint;
        public readonly string CallingConvention = callingConvention;
        public readonly string CharSet = charSet;
        public readonly string SetLastError = setLastError;
        public readonly bool IsStatic = isStatic;
        public readonly bool Managed = managed;
        public readonly ManagedProperty Property = property;
        public readonly string Option = option;
    }

    private class PropertyData(string option, NativeMethod getter, NativeMethod setter)
    {
        public readonly string Option = option;
        public readonly NativeMethod Getter = getter;
        public readonly NativeMethod Setter = setter;
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

    public static bool HasFieldValue(this AttributeData attributeData, string name)
    {
        return attributeData.NamedArguments.Any(kvp => kvp.Key == name);
    }
    
    public static TypedConstant GetFieldTypeConstant(this AttributeData attributeData, string name)
    {
        return attributeData.NamedArguments.FirstOrDefault(kvp => kvp.Key == name).Value;
    }

    public static T GetFieldValue<T>(this AttributeData attributeData, string name, T defaultValue = default)
    {
        var dict = attributeData.NamedArguments.ToDictionary(kv => kv.Key, kv => kv.Value);
        if (!dict.TryGetValue(name, out var tc)) return defaultValue;

        if (typeof(T).IsEnum) {
            return (T)Enum.Parse(typeof(T), tc.Value?.ToString() ?? "None");
        }

        return (T)tc.Value;
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
        var parameters = string.Join(", ", methodSymbol.Parameters.Select(p => p.OriginalDefinition));
        var methodSignature = $"{returnType} {methodName}({parameters})";

        return methodSignature;
    }

    public static bool IsString(this ITypeSymbol typeSymbol)
    {
        return typeSymbol.SpecialType == SpecialType.System_String;
    }
}

public enum ManagedProperty
{
    None,
    Get,
    Set
}

