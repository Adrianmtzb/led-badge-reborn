#include "console.h"
#include "app.h"
#include "catalog.h"
#include "net_portal.h"

// LINE_MAX es una macro de limits.h en este toolchain, de ahí el nombre propio.
static const uint16_t kLineMax = 96;

static String readLine() {
  static String buf;
  while (Serial.available()) {
    const char c = (char)Serial.read();
    if (c == '\n') {
      String out = buf;
      buf = "";
      out.trim();
      return out;
    }
    if (buf.length() < kLineMax) buf += c;  // descarta el resto de líneas largas
  }
  return "";
}

static void handleLine(const String& line) {
  String lower = line;
  lower.toLowerCase();

  if (lower == "help") {
    Serial.println("Comandos:");
    Serial.println("  status              estado actual");
    Serial.println("  mode special|pulso|fade");
    Serial.println("  pos <n>             posicion dentro de la categoria");
    Serial.println("  send [n]            emite el actual, o el indice absoluto n");
    Serial.println("  next                avanza posicion");
    Serial.println("  net                 estado de red");
    Serial.println("  forget              borra el WiFi y reinicia en portal");
    return;
  }

  if (lower == "status") { appPrintStatus(); return; }

  if (lower == "net") {
    Serial.printf("NET mode=%s ssid=%s ip=%s\n",
                  netMode() == NetMode::STA ? "STA" : "AP",
                  netSsid().c_str(), netIp().c_str());
    return;
  }

  if (lower == "forget") { netForget(); return; }
  if (lower == "next")   { appNext(); appPrintStatus(); return; }
  if (lower == "send")   { appSendCurrent(); return; }

  if (lower.startsWith("mode ")) {
    String m = lower.substring(5); m.trim();
    Mode parsed;
    if (modeFromName(m.c_str(), parsed)) appSetMode(parsed);
    else Serial.println("ERR: mode special|pulso|fade");
    return;
  }

  if (lower.startsWith("pos ")) {
    String n = lower.substring(4); n.trim();
    const long p = n.toInt();
    if (p < 0 || catalogCurrentCount() == 0) { Serial.println("ERR: posicion invalida"); return; }
    appSetPosition((uint16_t)(p % catalogCurrentCount()));
    return;
  }

  if (lower.startsWith("send ")) {
    String n = lower.substring(5); n.trim();
    const long idx = n.toInt();
    if (idx < 0 || idx >= (long)COMMAND_COUNT) { Serial.println("ERR: indice fuera de rango"); return; }
    appSendAbsolute((uint16_t)idx);
    return;
  }

  Serial.println("ERR: comando desconocido (escribe 'help')");
}

void consoleUpdate() {
  const String line = readLine();
  if (line.length()) handleLine(line);
}
