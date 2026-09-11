# Cómo contribuir

Gracias por pasarte. Esto es un proyecto pequeño, así que el proceso es ligero a
propósito.

---

## Modelo de ramas

Usamos **trunk-based**: una sola rama larga, `main`, siempre en estado publicable.

```
main ────●────●────●────●──── (siempre desplegable, protegida)
          \        /
           ●──●──●  feat/algo   (vida corta, se fusiona con squash)
```

- `main` está protegida. No se hace push directo.
- Cada cambio va en una rama corta que sale de `main` y vuelve por PR.
- Las PRs se fusionan con **squash**, para que `main` quede con un commit legible
  por cambio.
- Las versiones se marcan con **tags** `vX.Y.Z`, no con ramas.

Prefijos de rama: `feat/`, `fix/`, `docs/`, `refactor/`, `chore/`.

---

## Antes de mandar la PR

No hay CI todavía, así que la verificación es manual:

1. **Que compile.**

   ```bash
   cd firmware/led_badge_reborn
   arduino-cli compile --fqbn m5stack:esp32:m5stack_atom .
   ```

2. **Que funcione en hardware.** Casi todo lo interesante de este proyecto
   (temporizado del botón, emisión IR, portal cautivo) no se puede comprobar sin
   la placa delante. Dinos en la PR qué probaste.

3. **Si tocaste la red**, prueba los dos modos: arranque sin credenciales
   (portal) y arranque con credenciales válidas (STA).

4. **Si tocaste `web_ui.h`**, ábrelo en un móvil de verdad. El panel se diseñó
   para eso y hay diferencias reales con el escritorio.

### Los binarios de `docs/`

`docs/led_badge_reborn.bin` es un artefacto compilado que está versionado a
propósito, porque es lo que instala la página. **No lo regeneres en tu PR**: lo
actualiza el mantenedor al publicar una versión. Así se evitan conflictos
binarios entre PRs.

---

## Estilo

El código y los comentarios están **en español**. Mantenlo.

Los comentarios explican **por qué**, no qué. Si una línea necesita explicar qué
hace, normalmente lo que necesita es un nombre mejor. Los que hay en el código
suelen documentar una trampa concreta; por ejemplo, por qué el LED no se restaura
con `getPixelColor()`, o por qué el AP usa `WIFI_AP_STA`.

Otras convenciones que conviene respetar:

- Nada de `delay()` en `loop()`. El temporizado se hace con `millis()`.
- La categoría de un efecto vive en `Command::mode`, nunca deducida por rangos de
  índices.
- Todo lo que llegue de fuera (parámetros HTTP, tramas PRONTO, SSIDs) se valida
  antes de usarse.

### Mensajes de commit

Título en imperativo, sin punto final, hasta unos 72 caracteres. El cuerpo, si
hace falta, explica el porqué y el contexto:

```
Mostrar las redes WiFi como lista tocable

El datalist solo sugiere al escribir y en movil practicamente no se ve, asi
que el escaneo ahora pinta botones con el nombre y la intensidad.
```

No añadas firmas ni menciones de herramientas de IA en los commits.

---

## Publicar una versión (mantenedor)

1. Subir `FW_VERSION` en `firmware/led_badge_reborn/config.h` y `version` en
   `docs/manifest.json`.
2. Compilar con `--export-binaries` y copiar
   `build/m5stack.esp32.m5stack_atom/led_badge_reborn.ino.bin` a
   `docs/led_badge_reborn.bin`.
   Las otras tres partes solo cambian si se toca el esquema de particiones.
3. Commit, tag `vX.Y.Z` y push del tag.
4. Crear la Release en GitHub adjuntando el `.bin`.

---

## Reportar un fallo

Cuenta qué esperabas, qué pasó, y si puedes, pega la salida de la consola serie a
115200 baudios. Indica también si estabas en modo portal o conectado a tu red:
muchos comportamientos dependen de eso.
