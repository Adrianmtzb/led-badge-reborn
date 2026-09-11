#pragma once
#include <Arduino.h>

enum class NetMode : uint8_t { AP, STA };

void netBegin();
void netUpdate();

NetMode netMode();
String  netSsid();       // SSID de la red unida, o el del propio AP
String  netIp();
String  netApSsid();
bool    netHasCredentials();

// Guarda credenciales y programa un reinicio para aplicarlas.
// Devuelve false si el SSID o la contraseña no son válidos.
bool netSaveCredentials(const String& ssid, const String& pass);

// Borra las credenciales y programa un reinicio (vuelve al portal).
void netForget();

void netScheduleRestart(uint32_t delayMs);
