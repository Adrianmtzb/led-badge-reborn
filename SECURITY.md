# Política de seguridad

## Versiones con soporte

Solo la última versión publicada. Es un proyecto pequeño y no se mantienen ramas
antiguas.

## Cómo reportar

**No abras un issue público.** Usa el apartado
[Security → Report a vulnerability](https://github.com/Adrianmtzb/led-badge-reborn/security/advisories/new)
del repositorio, que es privado.

Cuenta qué encontraste, cómo reproducirlo y qué impacto le ves. Intento
responder en unos días; si el fallo es real, se arregla y se publica una versión
antes de hacerlo público.

## Modelo de amenaza

Conviene ser claro con lo que este firmware protege y lo que no, para no
reportar como fallo algo que es una decisión consciente.

**Lo que sí se defiende:**

- **CSRF.** Las rutas que cambian estado exigen la cabecera `X-Requested-With`.
  Una web cualquiera que visites no puede, desde tu navegador, mandar órdenes al
  dispositivo de tu red.
- **Inyección desde nombres de red.** Los SSID son texto que controla un tercero:
  se escapan al construir el JSON y se insertan en el DOM con `textContent`.
- **Índices fuera de rango.** Todo índice que llega por HTTP se valida contra el
  catálogo real antes de usarse.
- **Filtración de credenciales.** La contraseña de tu WiFi viaja en el cuerpo del
  POST y no se devuelve en ninguna respuesta.

**Lo que no se defiende, a propósito:**

- **El tráfico va en HTTP plano.** No hay TLS. Cualquiera en la misma red puede
  ver las órdenes y, durante la configuración inicial, la contraseña WiFi que
  envías por el portal. Poner TLS en un ESP32 con certificado autofirmado añade
  avisos de navegador y rompe la detección del portal cautivo, así que se
  descartó.
- **El AP tiene una contraseña por defecto conocida** (`ledbadge`, está en el
  README). Quien esté a tiro de la señal puede conectarse mientras el
  dispositivo esté en modo portal. Se eligió eso frente a dejar el AP abierto,
  pero no es un secreto.
- **No hay autenticación en el panel ni en la API.** Quien alcance el dispositivo
  en la red puede controlarlo. Es un badge de luces, no una cerradura.
- **Las credenciales se guardan sin cifrar** en la NVS del ESP32. Con acceso
  físico se pueden extraer volcando la flash.

Si encuentras algo que se salga de esta lista, repórtalo.
