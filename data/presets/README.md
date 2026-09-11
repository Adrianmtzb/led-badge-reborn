# Presets

Los efectos que emite el firmware no se escriben a mano: son exportaciones
literales de un generador de tramas que corre en el navegador.

- **Generador:** <https://adrianmtzb.github.io/led-badge-encoder/>
  ([código](https://github.com/Adrianmtzb/led-badge-encoder), MIT)
- **Un fichero por categoría.** `capturas.json`, `basicos.json`, …
- Cada comando trae el color solicitado, el color ya cuantizado por el badge,
  los bytes lógicos y codificados, el bitstream y la trama `prontoHex`.
  El firmware solo usa el color cuantizado, el nombre y la trama; el resto se
  conserva porque es lo que permite auditar de dónde salió cada byte.

## Regenerar

```bash
make presets   # reexporta estos JSON desde el generador (Playwright + Chrome)
make catalog   # genera pronto_data.h, catalog_data.cpp y docs/catalog.json
```

`make check-catalog` falla si los ficheros generados no cuadran con estos JSON,
así que el CI no deja que se desincronicen.

## Procedencia y licencias

El protocolo está documentado por la comunidad y publicado bajo MIT. Cada JSON
conserva el campo `disclaimer` del generador con esa atribución. Las
atribuciones completas están en
[THIRD_PARTY_NOTICES.md](../../THIRD_PARTY_NOTICES.md).

Todos los comandos son tramas de color único: encienden el badge con un color y
una envolvente. No hay comandos de configuración, escritura de EEPROM ni cambio
de grupo de ningún dispositivo.
