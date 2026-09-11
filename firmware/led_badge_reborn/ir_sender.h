#pragma once
#include <Arduino.h>

void irBegin();

// Devuelve false si la trama PRONTO está malformada (no se emite nada).
bool irSendPronto(const char* pronto);
