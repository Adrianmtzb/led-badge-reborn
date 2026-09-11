#!/usr/bin/env bash
#
# Comprueba las dependencias del proyecto e instala lo que falte.
# Es idempotente: puedes ejecutarlo las veces que quieras.
#
#   ./scripts/setup.sh    (o: make setup)

set -euo pipefail

CORE="m5stack:esp32"
BOARD_URL="https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json"

# Versiones fijas a propósito: una actualización de librería no debe romper la
# compilación sin que nadie haya tocado el código. Si las cambias aquí, cámbialas
# también en el Makefile.
LIBS=("IRremoteESP8266@2.9.0" "Adafruit NeoPixel@1.15.2")

if [ -t 1 ]; then
  VERDE=$'\033[32m'; AZUL=$'\033[34m'; AMBAR=$'\033[33m'; ROJO=$'\033[31m'; FIN=$'\033[0m'
else
  VERDE=''; AZUL=''; AMBAR=''; ROJO=''; FIN=''
fi

ok()    { printf '  %sok%s     %s\n'    "$VERDE" "$FIN" "$1"; }
paso()  { printf '  %s..%s     %s\n'    "$AZUL"  "$FIN" "$1"; }
aviso() { printf '  %saviso%s  %s\n'    "$AMBAR" "$FIN" "$1"; }
falta() { printf '  %sfalta%s  %s\n'    "$ROJO"  "$FIN" "$1"; }

echo
echo "Dependencias de LED Badge Blaster"
echo

# ---------------------------------------------------------------- python3
# Lo necesita la validación del manifest y el servidor local de pruebas.
if command -v python3 >/dev/null 2>&1; then
  ok "python3 $(python3 -c 'import platform; print(platform.python_version())')"
else
  falta "python3 no está instalado"
  echo "        macOS:  brew install python3"
  echo "        Debian: sudo apt install python3"
  exit 1
fi

# ------------------------------------------------------------ arduino-cli
if command -v arduino-cli >/dev/null 2>&1; then
  ok "arduino-cli $(arduino-cli version | awk '{print $3}')"
else
  falta "arduino-cli no está instalado"
  if command -v brew >/dev/null 2>&1; then
    paso "instalando con Homebrew"
    brew install arduino-cli
    ok "arduino-cli $(arduino-cli version | awk '{print $3}')"
  else
    # No canalizamos un script remoto directo a la shell: que decida quien
    # ejecuta esto, no nosotros.
    echo
    echo "        Instálalo y vuelve a ejecutar este script:"
    echo "          macOS:  brew install arduino-cli"
    echo "          Otros:  https://arduino.github.io/arduino-cli/latest/installation/"
    echo
    exit 1
  fi
fi

# ------------------------------------------------------------------- core
if arduino-cli core list 2>/dev/null | grep -q "^${CORE} "; then
  ok "core ${CORE} $(arduino-cli core list 2>/dev/null | awk -v c="$CORE" '$1==c {print $2}')"
else
  falta "core ${CORE} no instalado"
  paso "descargando (son varios cientos de MB, tarda un rato)"
  arduino-cli core update-index --additional-urls "$BOARD_URL"
  arduino-cli core install "$CORE" --additional-urls "$BOARD_URL"
  ok "core ${CORE} instalado"
fi

# -------------------------------------------------------------- librerías
version_instalada() {
  # El listado se lee en JSON porque hay nombres con espacios y el formato de
  # texto no se puede trocear de forma fiable.
  arduino-cli lib list --format json 2>/dev/null | python3 -c '
import json, sys

buscada = sys.argv[1]
datos = json.load(sys.stdin)
libs = datos.get("installed_libraries", []) if isinstance(datos, dict) else datos

for entrada in libs:
    lib = entrada.get("library", entrada)
    if lib.get("name") == buscada:
        print(lib.get("version", ""))
        break
' "$1"
}

for spec in "${LIBS[@]}"; do
  nombre="${spec%@*}"
  fijada="${spec##*@}"
  actual="$(version_instalada "$nombre" || true)"

  if [ "$actual" = "$fijada" ]; then
    ok "$nombre $fijada"
  elif [ -z "$actual" ]; then
    falta "$nombre no instalada"
    paso "instalando $spec"
    arduino-cli lib install "$spec"
    ok "$nombre $fijada"
  else
    aviso "$nombre $actual instalada, el proyecto fija $fijada"
    paso "ajustando a $fijada"
    arduino-cli lib install "$spec"
    ok "$nombre $fijada"
  fi
done

# ----------------------------------------------------------------- puerto
puerto="$(ls /dev/cu.usbserial-* /dev/ttyUSB* /dev/ttyACM* 2>/dev/null | head -1 || true)"
if [ -n "$puerto" ]; then
  ok "dispositivo detectado en $puerto"
else
  aviso "no hay ningún ATOM conectado (no hace falta para compilar)"
fi

# -------------------------------------------------------------- opcional
command -v gh >/dev/null 2>&1 \
  && ok "gh $(gh version | head -1 | awk '{print $3}') (opcional)" \
  || aviso "gh no instalado: opcional, solo para publicar releases"

echo
echo "Todo listo. Siguiente paso:  make build"
echo
