// =====================================================
// led-badge-blaster
// M5Stack ATOM Lite -> emisor IR (PRONTO) con portal WiFi propio.
//
// El reparto está en:
//   config.h       pines, tiempos y constantes
//   pronto_data.h  las 55 tramas PRONTO tal cual salieron del mando
//   catalog.*      registro de comandos y navegación por categoría
//   ir_sender.*    parser PRONTO y emisión
//   status_led.*   LED de estado
//   button.*       gestos del botón
//   net_portal.*   STA con credenciales guardadas, o AP con portal cautivo
//   web_ui.h       la página que sirve el dispositivo
//   web_api.*      rutas HTTP
//   console.*      consola serie de apoyo
// =====================================================

#include "config.h"
#include "app.h"
#include "button.h"
#include "console.h"
#include "net_portal.h"
#include "status_led.h"
#include "web_api.h"

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n" FW_NAME " " FW_VERSION);

  ledBegin();
  ledSetBase(255, 120, 0);  // naranja mientras arranca la red

  buttonBegin();
  appBegin();

  netBegin();
  webBegin();

  appRefreshLed();
  appPrintStatus();

  if (netMode() == NetMode::AP) {
    Serial.printf("Conectate a \"%s\" (clave %s) y abre http://%s/\n",
                  netApSsid().c_str(), AP_PASSWORD, netIp().c_str());
  }
}

void loop() {
  buttonUpdate();
  ledUpdate();
  netUpdate();
  webUpdate();
  consoleUpdate();
}
