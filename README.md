# LED Badge Reborn

[![CI](https://github.com/Adrianmtzb/led-badge-reborn/actions/workflows/ci.yml/badge.svg)](https://github.com/Adrianmtzb/led-badge-reborn/actions/workflows/ci.yml)
[![Licencia: MIT](https://img.shields.io/badge/licencia-MIT-blue.svg)](LICENSE)

Firmware para **M5Stack ATOM Lite** que controla un badge LED por infrarrojos.
Se instala desde el navegador, sin instalar nada, y luego se maneja desde el
móvil: el propio dispositivo levanta una red WiFi y sirve su panel de control.

---

## Qué hace

- Emite **55 tramas PRONTO** repartidas en tres categorías: `SPECIAL` (3),
  `PULSO` (25) y `FADE` (27).
- **Portal cautivo**: sin credenciales guardadas crea la red `LED-Badge-XXXX` y
  abre solo el panel al conectarte.
- **Modo red**: si le das tu WiFi, se une a ella y queda en `http://led-badge.local`.
- **API HTTP** para automatizar desde curl, Postman o lo que quieras.
- **Control físico** con el único botón de la placa.

## Qué hace falta

| | |
|---|---|
| Placa | M5Stack ATOM Lite (ESP32) |
| Emisor | LED IR en GPIO12 (el del ATOM, o uno externo) |
| Navegador | Chrome o Edge de escritorio, solo para instalar |

---

## Instalación

La forma fácil es desde el navegador, con el cable USB conectado:

**https://adrianmtzb.github.io/led-badge-reborn/**

Usa Web Serial, así que el firmware se descarga en tu navegador y se escribe
directo al dispositivo; no pasa por ningún servidor. Necesita Chrome o Edge de
escritorio: Firefox y Safari no implementan Web Serial, y en móvil no existe.

Si prefieres compilarlo tú, mira [Compilar desde el código](#compilar-desde-el-código).

---

## Primer arranque

1. Conéctate a la red **`LED-Badge-XXXX`** (las X son los últimos bytes de la MAC)
   con la clave **`ledbadge`**.
2. El panel se abre solo. Si tu móvil no lo detecta, entra a `http://192.168.4.1`.
3. Elige categoría y efecto. Ya está funcionando.
4. Opcional: en la tarjeta **WiFi**, pulsa *Buscar redes*, elige la tuya y guarda.
   El dispositivo se reinicia, se une a tu red y pasa a responder en
   `http://led-badge.local`.

## El botón

| Gesto | Acción |
|---|---|
| Clic | Envía el efecto actual y avanza al siguiente |
| Doble clic | Cambia de categoría |
| Mantener 3 s | Olvida el WiFi y vuelve al portal |

El LED indica la categoría: **blanco** `SPECIAL`, **verde** `PULSO`, **azul** `FADE`.
Destella en rojo cada vez que emite, y en magenta al olvidar las credenciales.

---

## API HTTP

Las rutas que cambian estado exigen la cabecera `X-Requested-With` (cualquier
valor sirve). Es un cortafuegos contra CSRF: una web ajena no puede añadir
cabeceras propias en una petición `no-cors`, y el dispositivo no responde
preflight. Los `GET` no la necesitan.

Los parámetros van como query string o `x-www-form-urlencoded`. **No acepta JSON**:
el `WebServer` del core ESP32 no lo parsea.

| Método | Ruta | Parámetros | Devuelve |
|---|---|---|---|
| `GET` | `/api/commands` | — | Lista de efectos con su índice |
| `GET` | `/api/state` | — | Categoría, posición, efecto actual y estado de red |
| `GET` | `/api/scan` | — | Redes WiFi visibles |
| `POST` | `/api/send` | `index` (0–54) | `{"ok":true}` |
| `POST` | `/api/wifi` | `ssid`, `pass` | Guarda y reinicia |
| `POST` | `/api/forget` | — | Borra credenciales y reinicia |

```bash
curl http://led-badge.local/api/commands

curl -X POST http://led-badge.local/api/send \
  -H 'X-Requested-With: curl' \
  -d 'index=12'
```

`/api/wifi` y `/api/forget` responden y **reinician 1,2 s después**, así que la
conexión se corta: el cliente suele estar hablando por el AP que se va a tumbar.

---

## Compilar desde el código

Hace falta [arduino-cli](https://arduino.github.io/arduino-cli/) con el core de
M5Stack y dos librerías:

```bash
arduino-cli core install m5stack:esp32
arduino-cli lib install "IRremoteESP8266" "Adafruit NeoPixel"
```

```bash
cd firmware/led_badge_reborn
arduino-cli compile --fqbn m5stack:esp32:m5stack_atom --export-binaries .
arduino-cli upload  --fqbn m5stack:esp32:m5stack_atom -p /dev/cu.usbserial-XXXX .
```

La carpeta tiene que llamarse igual que el `.ino`; es un requisito de Arduino.
Tarda un par de minutos la primera vez. Si la subida falla con *port is busy*,
casi siempre es un monitor serie abierto ocupando el puerto.

Hay además una consola serie a 115200 baudios para depurar: escribe `help`.

---

## Estructura

```
firmware/led_badge_reborn/
├── led_badge_reborn.ino   solo setup() y loop()
├── config.h               pines, tiempos, identidad
├── pronto_data.h          las 55 tramas IR en crudo
├── catalog.*              tabla de efectos y navegación por categoría
├── ir_sender.*            parser PRONTO y emisión
├── status_led.*           LED de estado, sin bloqueos
├── button.*               clic, doble clic y pulsación larga
├── net_portal.*           AP con portal cautivo o STA con mDNS
├── web_ui.h               el panel, en PROGMEM
├── web_api.*              rutas HTTP
└── console.*              consola serie

docs/                      instalador web (los .bin los genera el CI)
```

---

## Contribuir

Se aceptan PRs. Lee [CONTRIBUTING.md](CONTRIBUTING.md) para el modelo de ramas y
cómo probar los cambios. Si usas un asistente de IA, [AGENTS.md](AGENTS.md) tiene
el contexto y las trampas conocidas del proyecto.

Otros documentos: [CHANGELOG.md](CHANGELOG.md) y [SECURITY.md](SECURITY.md), que
explica qué protege este firmware y qué no.

## Licencia

[MIT](LICENSE) © Adrianmb
