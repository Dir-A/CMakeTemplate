#import <Cocoa/Cocoa.h>
#include "include/cef_application_mac.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_library_loader.h"

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

}

- (NSApplicationTerminateReply)applicationShouldTerminate: (NSApplication*)sender
{
  return NSTerminateNow;
}

- (BOOL)applicationShouldHandleReopen:(NSApplication*)theApplication hasVisibleWindows:(BOOL)flag
{
  return NO;
}

- (BOOL)applicationSupportsSecureRestorableState:(NSApplication*)app
{
  return YES;
}
@end // SimpleAppDelegate

static SimpleAppDelegate* g_delegate = nil;

auto CEFMacOSEntryInit() -> void
{
  @autoreleasepool
  {
    [SimpleApplication sharedApplication];
    CHECK([NSApp isKindOfClass:[SimpleApplication class]]);
    g_delegate = [[SimpleAppDelegate alloc] init];
    NSApp.delegate = g_delegate;
    [g_delegate performSelectorOnMainThread:@selector(createApplication:) withObject:nil waitUntilDone:NO];
  }
}

auto CEFMacOSEntryClean() -> void
{
    @autoreleasepool
    {
#if !__has_feature(objc_arc)
        [g_delegate release];
#endif
        g_delegate = nil;
    }
}
