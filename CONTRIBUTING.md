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

El CI compila cada PR automáticamente, así que no hace falta que demuestres que
compila. Lo que el CI **no** puede comprobar es lo importante de este proyecto:

1. **Que funcione en hardware.** El temporizado del botón, la emisión IR y el
   portal cautivo no se pueden verificar sin la placa delante. Cuenta en la PR
   qué probaste.

2. **Si tocaste la red**, prueba los dos modos: arranque sin credenciales
   (portal) y arranque con credenciales válidas (STA).

3. **Si tocaste `web_ui.h`**, ábrelo en un móvil de verdad. El panel se diseñó
   para eso y hay diferencias reales con el escritorio.

Para compilar en local:

```bash
cd firmware/led_badge_reborn
arduino-cli compile --fqbn m5stack:esp32:m5stack_atom .
```

### Los binarios del instalador

No están en el repositorio. `docs/` solo guarda la página y el `manifest.json`;
los `.bin` los compila y publica el CI al crear un tag. Así no hay conflictos
binarios entre PRs ni riesgo de que la página publicada instale una versión
distinta de la que está en el código.

Si cambias los nombres o los offsets del `manifest.json`, tendrás que ajustar
también el job `instalador` del CI, que los valida contra una lista fija.

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
   `docs/manifest.json`. **Tienen que coincidir**: el CI falla si no.
2. Anotar los cambios en [CHANGELOG.md](CHANGELOG.md).
3. Fusionar eso en `main` por PR, como cualquier otro cambio.
4. Crear el tag y empujarlo:

   ```bash
   git tag -a v1.2.3 -m "LED Badge Reborn v1.2.3"
   git push origin v1.2.3
   ```

El resto lo hace el CI: compila, crea la Release con los binarios adjuntos y
despliega el instalador en GitHub Pages. No hay que tocar ningún `.bin` a mano.

---

## Reportar un fallo

Cuenta qué esperabas, qué pasó, y si puedes, pega la salida de la consola serie a
115200 baudios. Indica también si estabas en modo portal o conectado a tu red:
muchos comportamientos dependen de eso.
