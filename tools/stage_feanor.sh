#!/bin/sh
# Staging de Feanor: tools/stage_data.sh (común) + lo propio de este IPA de iPhone OS 2.2:
#   - Info.plist viene en XML: foundation.c solo lee plist binario -> plutil
#   - font.ttf (NanumGothic) para el texto de UIKit/CG (también va dentro del VPK)
set -e
cd "$(dirname "$0")/.."
sh tools/stage_data.sh "$@"
plutil -convert binary1 ux0_data/feanor/game_app/Info.plist
cp font.ttf ux0_data/feanor/font.ttf
grep -q "^# Feanor" ux0_data/feanor/controls.txt 2>/dev/null || cp tools/controls.feanor.txt ux0_data/feanor/controls.txt
find ux0_data -name '._*' -delete
echo "Feanor: Info.plist binario + font.ttf listos"
