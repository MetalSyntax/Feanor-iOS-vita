# hosttest: el loader de Inotia: Feanor (../source, sin cambios) para Linux ARM32
# softfp (Debian armel), con psp2/kubridge falsos y vitaGL sobre GLES 1.1 de Mesa.
# Lo compila y ejecuta run.sh en Docker; ver "Prueba-ARM32-sin-consola.md".
cmake_minimum_required(VERSION 3.16)
project(feanor-hosttest C ASM)

set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/../source")
file(GLOB LOADER_SOURCES "${SRC}/*.c")
list(FILTER LOADER_SOURCES EXCLUDE REGEX "/(\\._[^/]*|vitagl_no_splash\\.c)$")   # parche de vitaGL; AppleDouble

add_executable(feanor ${LOADER_SOURCES} "${SRC}/asm.S"
  psp2_fake.c vitagl_fake.c window_sdl.c vorbis_fake.c gles2_fake.c gl_thread.c psp2_extra.c)
# include/ primero: su vitaGL.h sustituye al de vendor/vitaGL
target_include_directories(feanor PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include" "${PSP2_INCLUDE}" "${SRC}" /usr/include/freetype2)
# Como en la Vita: ABI softfp del código iOS, r9 reservado, Cortex-A9 con NEON.
set(FLAGS -march=armv7-a -mfpu=neon -mfloat-abi=softfp -ffixed-r9)
target_compile_options(feanor PRIVATE ${FLAGS} -O2 -g -w)
target_compile_definitions(feanor PRIVATE SKIP_SPLASHSCREEN _GNU_SOURCE _FILE_OFFSET_BITS=64 _TIME_BITS=64
  DATA_PATH="ux0:data/feanor/")
# usan SceIoStat.st_mtime (include/sys/stat.h)
set_source_files_properties("${SRC}/objc.c" "${SRC}/main.c" PROPERTIES COMPILE_DEFINITIONS HOSTTEST_SCE_STAT)
# Ejecutable y heap (brk) en 0x81000000, como el heap de la Vita: el loader valida
# punteros del juego contra 0x81000000..0x99000000 (patch.c, darwin.c).
# libstdc++ estática como en VitaSDK: cxx.c usa vtables de construcción de std::*fstream.
target_link_options(feanor PRIVATE ${FLAGS} -Wl,--wrap=pthread_create -no-pie -Wl,-Ttext-segment=0x81000000)
target_link_libraries(feanor SDL2 EGL OpenGL openal freetype png16 z bz2 -Wl,-Bstatic stdc++ -Wl,-Bdynamic m pthread)
