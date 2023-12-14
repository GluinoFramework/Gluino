#include "app.h"

#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>

using namespace Gluino;

WindowBase* App::SpawnWindow(WindowOptions* options, WindowEvents* events) {
    
}

void App::DespawnWindow(WindowBase *window) {
    
}

void App::Run() {
    @autoreleasepool {
        [NSApplication sharedApplication];
        [NSApp run];
    }
}

void App::Exit() {
    
}
