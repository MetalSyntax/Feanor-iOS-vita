# Inotia: Legend of Feanor (iOS 1.0.4) → PS Vita — progreso

Base: runtime iOS común (copia maestra en `Ferrari-GT-Evolution-vita`, `tools/sync_runtime.sh`) + lo propio de
Feanor. IPA usado: `Chronicles_of_Inotia_v1.0.4_Arce_game.ipa` (thin armv6, `cryptid 0`). La 1.0.5 está
cifrada (armv7 `cryptid 1`, slice armv6 con cabecera borrada): no sirve.

## Estado (2026-11-10)

Hosttest ARM32 (`hosttest/run.sh`, Docker armel): **llega al gameplay**. Título, menú, opciones, dificultad,
creación de personaje, texto de la historia y el bosque inicial con el personaje caminando. Música por
AudioQueue (.wds = WAV). Botones físicos probados en hosttest. **Falta la corrida #1 en la consola.**

- [x] Repo `psvita-toolkit init --ipa` (PSVFEANOR, slug `feanor`), runtime de Ferrari sincronizado.
- [x] Rebase: `tools/ios_prep.py` ahora sigue los `___switch8/u8/16/32` de GCC Thumb-1 (95 tablas) y rebasea
      punteros de `__data` que apuntan exacto a un símbolo (`&____LangConfig`, estáticos de `__common`).
- [x] Imports: 0 sin implementar.
- [x] Render por software: `-[UIView setNeedsDisplay]` con `drawRect:` propio → contexto 320x480 (ARGB) →
      `gles_present_pixels` (rotación 1, como Inotia 2).
- [x] CoreGraphics: CTM completo, Save/RestoreGState, ConcatCTM, FillRect con CTM, FillEllipseInRect, texto
      (`CGContextShowTextAtPoint`, `-[NSString drawAtPoint:withFont:]`, `sizeWithFont:`) con FreeType y
      `font.ttf` (NanumGothic, va en el VPK). Fill clamp a [0,1] (el juego pasa 255.0).
- [x] Bucle: `CFRunLoopTimerCreate/AddTimer` (20 Hz) sobre los NSTimer del runtime.
- [x] Recursos: `CFBundleCopyResourceURLForLocalization` con `<Idioma>.lproj` viejo (Spanish, English...),
      `CFURLCreateDataAndPropertiesFromResource`, `CFData*`.
- [x] Partidas: `NSFileHandle` (FILE*), `fileSystemAttributesAtPath:`, `-[NSData getBytes:]`.
- [x] `MPMoviePlayerContentPreloadDidFinishNotification` (iPhone OS 2: el juego espera esto para `play`).
- [x] NSString `initWithBytesNoCopy:length:encoding:freeWhenDone:` y `getBytes:maxLength:...` (rutas de archivo).
- [x] CFNetwork (ranking) y `getnameinfo`: sin red.
- [x] Controles: cruceta/stick izq. → teclas WIPI 2/4/6/8, ✕ → 5 (OK), vía `patch_tick` (patch.c);
      ○ → botón de volver (toque, `controls.txt`). Touch frontal directo.

## Datos (no se commitean)

`sh tools/stage_feanor.sh` arma `ux0_data/feanor/` (bundle, rebase/binds, Info.plist binario, font.ttf,
controls.txt) y `python3 -I tools/deploy_data.py` lo sube a `ux0:data/feanor/`.

## Corrida #1 en consola: qué mirar

1. Arranca y muestra el título (si no: log `ux0:data/feanor/logs/feanor_NNN.log` + `.psp2dmp`).
2. Colores correctos (si salen cambiados R/B: formato ARGB vs BGRA de `drawRect:` en `uikit.c`).
3. Texto de la intro legible y en el idioma de la consola (Spanish.lproj si la Vita está en español).
4. Cruceta mueve en menús y en el mapa; ✕ confirma; ○ vuelve.
5. Música y efectos; fps (línea PERF del log; el juego va a 20 Hz).
6. Guardar, salir y volver a cargar.

## Pendiente

- [ ] Video de intro `Feanor2i.mp4` (hoy se salta: termina a los 0,5 s).
- [ ] LiveArea propia (hoy la del boilerplate).
- [ ] UITextField del nombre (`SetTextField_iPhone`): verificar IME en consola.
- [ ] MoreGames (UIScrollView/NSXMLParser): sin implementar; no tocar ese botón.
- [ ] Subir al maestro (Ferrari) lo genérico que se agregó acá: uikit.c (CG/texto/drawRect), foundation.c
      (CFRunLoopTimer, NSFileHandle, CF localizado), objc.c (`objc_add_methods`), gles.c
      (`gles_present_pixels`), ios_prep.py (switch Thumb, punteros a símbolos).
