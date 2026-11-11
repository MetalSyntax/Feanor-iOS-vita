#pragma once
// Configuración propia de este juego (el resto de source/ es común a los ports iOS; se sincroniza
// con tools/sync_runtime.sh desde Ferrari-GT-Evolution-vita).
#define PORT_NAME "Inotia: Legend of Feanor"
#define PORT_SLUG "feanor"
#define PORT_EXEC "feanor2i"            // v1.0.4: thin armv6, cryptid 0 (la 1.0.5 está cifrada)
#define PORT_ROTATION 1                 // Info.plist: LandscapeRight; drawRect: entrega 320x480 girado (como Inotia 2)
#define PORT_STRETCH 0
#define PORT_SYSTEM_VERSION "3.1.3"     // MinimumOSVersion 2.2
#define PORT_DEVICE_MACHINE "iPhone1,2"
#define GLES_POOL_MB 16
#define PORT_ACCEL_FROM_STICK 0
#define PORT_SCREEN_W 320
#define PORT_SCREEN_H 480
#define PORT_SCREEN_SCALE 1.0f
#define PORT_CFTIMER_MAX_INTERVAL 0.004 // el CFRunLoopTimer (0.05 s) despacha los timers WIPI: más fino = frames a tiempo
