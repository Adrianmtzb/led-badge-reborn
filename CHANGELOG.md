# Changelog

Formato basado en [Keep a Changelog](https://keepachangelog.com/es-ES/1.1.0/).
Este proyecto sigue [Versionado Semántico](https://semver.org/lang/es/).

## [Sin publicar]

## [1.0.0] - 2026-09-11

Primera versión pública.

### Añadido

- Emisión de 55 tramas PRONTO en tres categorías: `SPECIAL`, `PULSO` y `FADE`.
- Portal cautivo: sin credenciales guardadas el dispositivo levanta la red
  `LED-Badge-XXXX` y sirve su propio panel de control.
- Modo estación con mDNS: al guardar una red, queda accesible en
  `http://led-badge.local`.
- Panel web adaptado a móvil, con selección de efecto por categoría y
  configuración de WiFi con escaneo de redes.
- API HTTP (`/api/commands`, `/api/state`, `/api/scan`, `/api/send`,
  `/api/wifi`, `/api/forget`) protegida contra CSRF por cabecera.
- Control por el botón de la placa: clic para emitir y avanzar, doble clic para
  cambiar de categoría y tres segundos para olvidar el WiFi.
- LED de estado que indica la categoría activa y destella al emitir.
- Consola serie de depuración a 115200 baudios.
- Instalador web con ESP Web Tools, sin necesidad de instalar nada.

[Sin publicar]: https://github.com/Adrianmtzb/led-badge-reborn/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/Adrianmtzb/led-badge-reborn/releases/tag/v1.0.0
