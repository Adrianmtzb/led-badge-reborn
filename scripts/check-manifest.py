#!/usr/bin/env python3
"""Valida el instalador web antes de publicarlo.

Dos cosas que fallan en silencio si nadie las mira:

  - Que los offsets del manifest sigan siendo los verificados contra el
    merged.bin. Si cambian, esptool escribe las partes en el sitio equivocado
    y el dispositivo queda inservible sin un solo mensaje de error.
  - Que la version anunciada en el manifest sea la que compila el firmware. Si
    se desincronizan, la pagina ofrece una version que no es la que instala.

Se ejecuta en el CI y desde `make check`.
"""

import json
import pathlib
import re
import sys

RAIZ = pathlib.Path(__file__).resolve().parent.parent

# Offsets verificados byte a byte contra el merged.bin. Los nombres son los que
# arma el job de publicacion a partir de los binarios compilados.
ESPERADO = {
    "bootloader.bin":       4096,   # 0x1000
    "partitions.bin":      32768,   # 0x8000
    "boot_app0.bin":       57344,   # 0xE000
    "led_badge_reborn.bin": 65536,  # 0x10000
}


def main() -> int:
    manifest_ruta = RAIZ / "docs" / "manifest.json"
    config_ruta = RAIZ / "firmware" / "led_badge_reborn" / "config.h"

    manifest = json.loads(manifest_ruta.read_text())
    partes = {p["path"]: p["offset"] for b in manifest["builds"] for p in b["parts"]}

    if partes != ESPERADO:
        print("Las partes del manifest no cuadran.", file=sys.stderr)
        print(f"  esperado: {ESPERADO}", file=sys.stderr)
        print(f"  manifest: {partes}", file=sys.stderr)
        return 1

    config = config_ruta.read_text()
    encontrado = re.search(r'#define\s+FW_VERSION\s+"([^"]+)"', config)
    if not encontrado:
        print("No se encontro FW_VERSION en config.h", file=sys.stderr)
        return 1

    firmware = encontrado.group(1)
    if firmware != manifest["version"]:
        print(
            f"Versiones distintas: config.h dice {firmware} "
            f'y manifest.json dice {manifest["version"]}',
            file=sys.stderr,
        )
        return 1

    print(f'OK: {manifest["name"]} {manifest["version"]}')
    return 0


if __name__ == "__main__":
    sys.exit(main())
