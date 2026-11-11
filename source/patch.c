// Parches de Inotia: Legend of Feanor (v1.0.4, armv6, iPhone OS 2.2). Cada parche se agrega con evidencia
// (log/dump + desensamblado) y su dirección del Mach-O.
//
// Motor: capa WIPI de Com2uS (MC_*/WV_*) con render por software. MyView (480x320 apaisado en un
// CGBitmapContext ARGB) -> MC_grpFlushLcd -> [MyView setRefreshScreenBufferImage] + setNeedsDisplay ->
// -[MyView drawRect:] lo copia girado al contexto de UIKit (320x480). El runtime (uikit.c) dibuja eso por
// software y lo presenta (gles_present_pixels); el bucle es un CFRunLoopTimer de 20 Hz (foundation.c).
#include <psp2/ctrl.h>
#include <string.h>

#include "imports.h"
#include "log.h"
#include "macho.h"
#include "patch.h"

const import_entry patch_imports[] = {
    {"", 0, 0},
};
TABLE_END(patch)

// ---------------------------------------------------------------- teclado WIPI desde los botones
// El juego viene de celular: EVENT_POINT_PRESS traduce toques a teclas y las procesa EVENT_KEYPRESS (0x2c084),
// que lee la tecla actual de una global (puntero en __data 0x3eb6c); 0xFF = soltar (EVENT_POINT_RELEASE,
// 0x2e098). Teclas del teclado numérico: 2/4/6/8 = arriba/izq/der/abajo (-> 0x12/0x10/0x11/0x13), 5 = OK (0x14).
// Cruceta y stick izquierdo -> 2/4/6/8, X -> 5. Se inyecta en el hilo del juego, antes de cada tick.
static uintptr_t ev_keypress;
static int32_t **key_var;
void patch_tick(void) {
    static int held; // tecla WIPI sostenida (0 = ninguna)
    if (!ev_keypress || !key_var || !*key_var)
        return;
    SceCtrlData pad;
    if (sceCtrlPeekBufferPositive(0, &pad, 1) < 0)
        return;
    uint32_t b = pad.buttons;
    if (pad.ly < 40) b |= SCE_CTRL_UP;
    if (pad.ly > 215) b |= SCE_CTRL_DOWN;
    if (pad.lx < 40) b |= SCE_CTRL_LEFT;
    if (pad.lx > 215) b |= SCE_CTRL_RIGHT;
    int want = b & SCE_CTRL_UP ? 2 : b & SCE_CTRL_DOWN ? 8 : b & SCE_CTRL_LEFT ? 4 : b & SCE_CTRL_RIGHT ? 6
             : b & SCE_CTRL_CROSS ? 5 : 0;
    if (want == held)
        return;
    if (held) { // soltar la anterior
        **key_var = 0xFF;
        call_guest((void *)ev_keypress, 0, 0, 0, 0);
    }
    if (want) {
        **key_var = want;
        call_guest((void *)ev_keypress, 0, 0, 0, 0);
    }
    held = want;
}

void patch_init(void) {
    ev_keypress = macho_sym("_EVENT_KEYPRESS");
    key_var = (int32_t **)(0x3eb6c + MACHO_SLIDE); // __data: puntero (rebasado) a la tecla actual
    l_info("teclado WIPI: EVENT_KEYPRESS en 0x%08X", (unsigned)(ev_keypress ? ev_keypress - MACHO_SLIDE : 0));
}
