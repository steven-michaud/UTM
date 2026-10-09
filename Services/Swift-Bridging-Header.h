//
// Copyright © 2020 osy. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <os/proc.h>
#include "TargetConditionals.h"
#include "UTMLegacyQemuConfiguration.h"
#include "UTMLegacyQemuConfiguration+Constants.h"
#include "UTMLegacyQemuConfiguration+Display.h"
#include "UTMLegacyQemuConfiguration+Drives.h"
#include "UTMLegacyQemuConfiguration+Miscellaneous.h"
#include "UTMLegacyQemuConfiguration+Networking.h"
#include "UTMLegacyQemuConfiguration+Sharing.h"
#include "UTMLegacyQemuConfiguration+System.h"
#include "UTMLegacyQemuConfigurationPortForward.h"
#include "UTMLogging.h"
#include "UTMASIFImage.h"
#include "VMKeyboardMap.h"
#if !defined(WITH_REMOTE)
#include "UTMProcess.h"
#include "UTMQemuSystem.h"
#include "QEMUHelperProtocol.h"
#include "UTMJailbreak.h"
#else
#include "UTMQemuSystemBackends.h"
#endif
#include "UTMLegacyViewState.h"
#include "UTMSpiceIO.h"
#include "GenerateKey.h"
#if TARGET_OS_IPHONE
#if defined(WITH_LOCATION_BACKGROUND)
#include "UTMLocationManager.h"
#endif
#include "VMDisplayViewController.h"
//#if !defined(TARGET_OS_VISION) || !TARGET_OS_VISION
#include "VMDisplayMetalViewController.h"
#include "VMDisplayMetalViewController+Keyboard.h"
//#endif
#include "VMKeyboardView.h"
#elif TARGET_OS_OSX
#include <Virtualization/Virtualization.h>
typedef uint32_t CGSConnectionID;
typedef CF_ENUM(uint32_t, CGSGlobalHotKeyOperatingMode) {
    kCGSGlobalHotKeyOperatingModeEnable = 0,
    kCGSGlobalHotKeyOperatingModeDisable = 1,
};
extern CGSConnectionID CGSMainConnectionID(void);
extern CGError CGSSetGlobalHotKeyOperatingMode(CGSConnectionID connection, CGSGlobalHotKeyOperatingMode mode);
#ifdef __arm64__
VZ_EXPORT API_AVAILABLE(macos(12.0))
@interface NSObject (_VZVirtualMachineStartOptions)
@property BOOL bootMacOSRecovery;
@property BOOL forceDFU;
@property BOOL stopInIBootStage1;
@property BOOL stopInIBootStage2;
@end
VZ_EXPORT API_AVAILABLE(macos(12.0))
@interface VZVirtualMachine()
- (void)_startWithOptions:(NSObject * _Nonnull)options completionHandler:(void (^ _Nonnull)(NSError * _Nullable errorOrNil))completionHandler;
@end
VZ_EXPORT API_AVAILABLE(macos(12.0))
@interface _VZGDBDebugStubConfiguration : NSObject <NSCopying>
@property short port;
- (NSObject * _Nonnull)makeDebugStubForVirtualMachine:(NSObject * _Nullable)unused;
@end
VZ_EXPORT API_AVAILABLE(macos(12.0))
@interface VZVirtualMachineConfiguration()
@property (atomic, setter=_setDebugStub:) id _Nullable _debugStub;
@end
VZ_EXPORT API_AVAILABLE(macos(13.0))
@interface VZMacOSVirtualMachineStartOptions (VZPrivate)
@property (atomic, setter=_setForceDFU:) BOOL _forceDFU;
@property (atomic, setter=_setStopInIBootStage1:) BOOL _stopInIBootStage1;
@property (atomic, setter=_setStopInIBootStage2:) BOOL _stopInIBootStage2;
@end
VZ_EXPORT API_AVAILABLE(macos(12.0))
@interface _VZPL011SerialPortConfiguration : VZSerialPortConfiguration
- (instancetype _Nonnull)init;
@end
#endif
#endif
