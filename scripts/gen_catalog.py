#!/usr/bin/env python3
"""Genera el catalogo de efectos a partir de data/presets/*.json.

Escribe firmware/led_badge_blaster/pronto_data.h, catalog_data.cpp y
docs/catalog.json. Los JSON de entrada son exportaciones literales del
generador web; su procedencia esta en data/presets/README.md.

  make catalog         regenera los tres ficheros
  make check-catalog   comprueba que lo del disco cuadra con los presets
"""

import argparse
import json
import pathlib
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
PRESETS = ROOT / "data" / "presets"
FIRMWARE = ROOT / "firmware" / "led_badge_blaster"
DOCS = ROOT / "docs"

# Orden de las categorías en el firmware y en la interfaz. El prefijo es el que
# lleva el id estable de cada comando (CAP_00, BAS_00, ...).
CATEGORIES = [
    ("capturas",    "CAPTURAS",    "CAP", "Capturas",    "Colores decodificados de capturas reales. La referencia de compatibilidad más fiable."),
    ("basicos",     "BASICOS",     "BAS", "Básicos",     "Primarios y secundarios al máximo que el badge puede representar."),
    ("envolventes", "ENVOLVENTES", "ENV", "Envolventes", "Un mismo color con distinta forma temporal: attack, sustain y release."),
    ("multitud",    "MULTITUD",    "MUL", "Multitud",    "Usan el campo chance: solo una fracción aleatoria de los badges responde."),
    ("escenas",     "ESCENAS",     "ESC", "Escenas",     "Paletas coherentes pensadas para lanzarse en secuencia."),
    ("grupos",      "GRUPOS",      "GRP", "Grupos",      "El mismo color restringido a un group id. Solo responden los badges de ese grupo."),
]

HEADER_NOTE = """// GENERADO POR scripts/gen_catalog.py — NO EDITAR A MANO.
// Fuente: data/presets/*.json (exportaciones del generador web).
// Regenerar con: make catalog
"""


def c_string(value: str) -> str:
    escaped = value.replace("\\", "\\\\").replace('"', '\\"')
    return f'"{escaped}"'


def load(category_id: str) -> list[dict]:
    path = PRESETS / f"{category_id}.json"
    if not path.exists():
        sys.exit(f"falta {path}; exporta la categoria desde el generador web")
    return json.loads(path.read_text(encoding="utf-8"))["commands"]


def build() -> dict[pathlib.Path, str]:
    pronto_lines = ["#pragma once", "", HEADER_NOTE.rstrip(), ""]
    entries = []

    for category_id, enum_name, prefix, _label, _blurb in CATEGORIES:
        commands = load(category_id)
        pronto_lines.append(f"// ----- {enum_name} ({len(commands)}) -----")
        for index, command in enumerate(commands):
            symbol = f"P_{prefix}_{index:02d}"
            pronto_lines.append(f"static const char {symbol}[] =")
            pronto_lines.append(f'{c_string(command["prontoHex"])};')
            pronto_lines.append("")

            color = command["quantizedColor"]
            entries.append(
                {
                    "id": f"{prefix}_{index:02d}",
                    "name": command["name"],
                    "note": command.get("note") or "",
                    "rgb": (color["r"] << 16) | (color["g"] << 8) | color["b"],
                    "symbol": symbol,
                    "mode": enum_name,
                }
            )

    out: dict[pathlib.Path, str] = {FIRMWARE / "pronto_data.h": "\n".join(pronto_lines)}

    width = max(len(e["symbol"]) for e in entries)
    rows = []
    current_mode = None
    for entry in entries:
        if entry["mode"] != current_mode:
            current_mode = entry["mode"]
            rows.append("")
        rows.append(
            "  {{ {id:<10} {name:<20} {note:<26} 0x{rgb:06X}, {symbol:<{w}} Mode::{mode} }},".format(
                id=c_string(entry["id"]) + ",",
                name=c_string(entry["name"]) + ",",
                note=c_string(entry["note"]) + ",",
                rgb=entry["rgb"],
                symbol=entry["symbol"] + ",",
                w=width + 1,
                mode=entry["mode"],
            )
        )

    body = "\n".join(
        [
            '#include "catalog.h"',
            '#include "pronto_data.h"',
            "",
            HEADER_NOTE.rstrip(),
            "",
            "const Command COMMANDS[] = {",
            *rows[1:],
            "};",
            "",
            "const uint16_t COMMAND_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);",
            "",
            "const CategoryInfo CATEGORIES[MODE_COUNT] = {",
        ]
    )

    category_rows = [
        "  {{ {key:<15} {label:<15} {blurb} }},".format(
            key=c_string(enum_name) + ",",
            label=c_string(label) + ",",
            blurb=c_string(blurb),
        )
        for _cid, enum_name, _prefix, label, blurb in CATEGORIES
    ]

    body += "\n" + "\n".join(category_rows) + "\n};\n"
    out[FIRMWARE / "catalog_data.cpp"] = body

    # La página de instalación pinta el catálogo real desde aquí, así no hay
    # una segunda copia de los colores que se quede atrás.
    catalog = {
        "categories": [
            {
                "key": enum_name,
                "label": label,
                "blurb": blurb,
                "colors": [
                    {"name": e["name"], "hex": "#%06X" % e["rgb"]}
                    for e in entries
                    if e["mode"] == enum_name
                ],
            }
            for _cid, enum_name, _prefix, label, blurb in CATEGORIES
        ],
        "total": len(entries),
    }
    out[DOCS / "catalog.json"] = json.dumps(catalog, ensure_ascii=False, indent=2) + "\n"

    print(f"{len(entries)} comandos en {len(CATEGORIES)} categorias")
    return out


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--check",
        action="store_true",
        help="no escribe nada; falla si el disco no cuadra con los presets",
    )
    args = parser.parse_args()

    files = build()

    if not args.check:
        for path, content in files.items():
            path.write_text(content, encoding="utf-8")
        return 0

    stale = [
        path.relative_to(ROOT)
        for path, content in files.items()
        if not path.exists() or path.read_text(encoding="utf-8") != content
    ]
    if stale:
        print("\nEstos ficheros no cuadran con data/presets/:", file=sys.stderr)
        for path in stale:
            print(f"  {path}", file=sys.stderr)
        print("Ejecuta 'make catalog' y commitea el resultado.", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
