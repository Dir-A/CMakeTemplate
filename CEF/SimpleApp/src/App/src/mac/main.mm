#import <Cocoa/Cocoa.h>

#include "include/cef_application_mac.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_library_loader.h"
#include "MyApp.hpp"
#include "MyClient.hpp"

@interface SimpleAppDelegate : NSObject <NSApplicationDelegate>

- (void)createApplication:(id)object;
- (void)tryToTerminateApplication:(NSApplication*)app;
@end


@interface SimpleApplication : NSApplication <CefAppProtocol> 
{
 @private
  BOOL handlingSendEvent_;
}
@end


@implementation SimpleApplication
- (BOOL)isHandlingSendEvent {
  return handlingSendEvent_;
}

- (void)setHandlingSendEvent:(BOOL)handlingSendEvent 
{
  handlingSendEvent_ = handlingSendEvent;
}

- (void)sendEvent:(NSEvent*)event 
{
  CefScopedSendingEvent sendingEventScoper;
  [super sendEvent:event];
}

- (void)terminate:(id)sender 
{
  SimpleAppDelegate* delegate = static_cast<SimpleAppDelegate*>([NSApp delegate]);
  [delegate tryToTerminateApplication:self];
}
@end // SimpleApplication


@implementation SimpleAppDelegate

- (void)createApplication:(id)object 
{
  [[NSBundle mainBundle] loadNibNamed:@"MainMenu" owner:NSApp topLevelObjects:nil];
  [[NSApplication sharedApplication] setDelegate:self];
}

- (void)tryToTerminateApplication:(NSApplication*)app 
{
  auto handler = MyClient::GetInstance();
  if (handler && !handler->IsClosing()) 
  {
    handler->CloseAllBrowsers(false);
  }
}

- (NSApplicationTerminateReply)applicationShouldTerminate: (NSApplication*)sender 
{
  return NSTerminateNow;
}

- (BOOL)applicationShouldHandleReopen:(NSApplication*)theApplication hasVisibleWindows:(BOOL)flag 
{
  auto handler = MyClient::GetInstance();
  if (handler && !handler->IsClosing()) 
  {
    handler->ShowMainWindow();
  }
  return NO;
}

- (BOOL)applicationSupportsSecureRestorableState:(NSApplication*)app 
{
  return YES;
}
@end // SimpleAppDelegate


int main(int argc, char* argv[]) 
{
  CefScopedLibraryLoader library_loader;
  if (!library_loader.LoadInMain()) 
  {
    return 1;
  }

  @autoreleasepool 
  {
    [SimpleApplication sharedApplication];
    CHECK([NSApp isKindOfClass:[SimpleApplication class]]);

    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromArgv(argc, argv);

    CefSettings settings;
    settings.no_sandbox = true;

    SimpleAppDelegate* delegate = [[SimpleAppDelegate alloc] init];
    NSApp.delegate = delegate;
    [delegate performSelectorOnMainThread:@selector(createApplication:) withObject:nil waitUntilDone:NO];

    CefInitialize(CefMainArgs{argc, argv}, settings, new MyApp, nullptr);
    CefRunMessageLoop();
    CefShutdown();

#if !__has_feature(objc_arc)
    [delegate release];
#endif  // !__has_feature(objc_arc)
    delegate = nil;
  }  // @autoreleasepool

  return 0;
}
