# AGENTS.md

Contexto para asistentes de IA que trabajen en este repositorio. Lee también
[CONTRIBUTING.md](CONTRIBUTING.md): el modelo de ramas y el estilo aplican igual.

## Qué es esto

Firmware Arduino/C++ para **M5Stack ATOM Lite** (ESP32 clásico). Emite tramas
PRONTO por infrarrojos y sirve un panel web desde el propio dispositivo.

La carpeta del proyecto puede llamarse `dtmf_ir` en algunas máquinas por motivos
históricos. El proyecto es `led-badge-reborn`.

## Comandos

```bash
cd firmware/led_badge_reborn
arduino-cli compile --fqbn m5stack:esp32:m5stack_atom --export-binaries .
arduino-cli upload  --fqbn m5stack:esp32:m5stack_atom -p /dev/cu.usbserial-XXXX .
```

- **La compilación tarda 2–3 minutos.** Si tu herramienta de terminal tiene modo
  asíncrono, úsalo; en modo síncrono es fácil que se dé por vencida antes.
- La carpeta debe llamarse igual que el `.ino`, o `arduino-cli` responde
  *"Can't open sketch: main file missing"*.
- Si la subida falla con *port is busy*, hay un monitor serie abierto. En VS Code
  se ve con `lsof /dev/cu.usbserial-*` como proceso `serial-mo`. Hay que pedirle
  al usuario que lo cierre; no se puede resolver desde el agente.
- Ocupación actual: ~33 % de flash y ~14 % de RAM.

## Trampas ya pagadas

No las redescubras:

- **`LINE_MAX` es una macro de `limits.h`** en este toolchain. Usarla como nombre
  de constante rompe la compilación con un error críptico
  (*expected unqualified-id before numeric constant*). Lo mismo puede pasar con
  otros nombres en mayúsculas muy genéricos.
- **`Adafruit_NeoPixel::getPixelColor()` no sirve para restaurar un color**:
  devuelve el valor ya escalado por el brillo, así que releerlo y reaplicarlo
  atenúa el LED un poco más en cada ciclo. El color lógico se guarda aparte en
  `status_led.cpp`.
- **GPIO39 no tiene resistencias internas** (es input-only). La placa lleva
  pull-up externo, así que va con `INPUT`, nunca `INPUT_PULLUP`.
- **El AP usa `WIFI_AP_STA`, no `WIFI_AP`.** Hace falta la interfaz STA para que
  `/api/scan` pueda escanear sin tumbar el punto de acceso.
- **Los clics se detectan en el flanco de pulsación** ya rebotado, no al soltar.
  El temporizador de clic simple solo corre con el botón liberado.
- **Guardar u olvidar WiFi responde primero y reinicia 1,2 s después.** El cliente
  está hablando por el AP que se va a tumbar; reiniciar antes de contestar deja
  al navegador con un error.
- **Los offsets de flasheo del instalador están verificados** byte a byte contra
  el `merged.bin`: bootloader `0x1000`, particiones `0x8000`, boot_app0 `0xE000`,
  app `0x10000`. En `manifest.json` van en decimal porque JSON no tiene hex. No
  los cambies "a ojo".
- **`boot_app0.bin` no lo genera la compilación**; sale del core instalado, en
  `~/Library/Arduino15/packages/m5stack/hardware/esp32/*/tools/partitions/`.

## Seguridad

Cosas que ya están resueltas y no hay que desmontar por descuido:

- Las rutas que mutan estado exigen la cabecera `X-Requested-With` y responden
  403 sin ella. Es la defensa contra CSRF desde otra web.
- Los SSID escaneados son texto de terceros: se escapan al meterlos en el JSON y
  se insertan en el DOM con `textContent`, nunca con `innerHTML`.
- Las contraseñas viajan en el cuerpo del POST y no se devuelven nunca en las
  respuestas.
- Todo índice que llega por HTTP se valida contra el rango real del catálogo.

## Al hacer cambios

- Comentarios y mensajes de commit **en español**.
- **No añadas firmas ni atribuciones de IA** en commits, PRs, código ni docs.
- No regeneres `docs/*.bin` salvo que se esté publicando una versión.
- Si no puedes verificar algo en hardware, dilo en lugar de darlo por bueno.
