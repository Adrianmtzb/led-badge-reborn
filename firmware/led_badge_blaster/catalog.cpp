#include "catalog.h"

// Los datos viven en catalog_data.cpp (generado). Aquí solo está el estado de
// navegación: qué categoría está activa y en qué posición dentro de ella.

// COMMANDS llega agrupado por categoría, así que a cada modo le basta con su
// primer índice y su longitud; no hace falta un mapa en RAM.
static uint16_t modeStart[MODE_COUNT];
static uint16_t modeCount[MODE_COUNT];

static Mode currentMode = Mode::CAPTURAS;
static uint16_t modePosition = 0;

const char* modeName(Mode m) {
  const uint8_t i = (uint8_t)m;
  return i < MODE_COUNT ? CATEGORIES[i].key : "?";
}

bool modeFromName(const char* name, Mode& out) {
  if (!name) return false;
  for (uint8_t i = 0; i < MODE_COUNT; i++) {
    if (!strcasecmp(name, CATEGORIES[i].key)) {
      out = (Mode)i;
      return true;
    }
  }
  return false;
}

void catalogBegin() {
  for (uint8_t m = 0; m < MODE_COUNT; m++) {
    modeStart[m] = 0;
    modeCount[m] = 0;
  }

  for (uint16_t i = 0; i < COMMAND_COUNT; i++) {
    const uint8_t m = (uint8_t)COMMANDS[i].mode;
    if (m >= MODE_COUNT) continue;
    if (modeCount[m] == 0) modeStart[m] = i;
    modeCount[m]++;
  }

  // El rango [start, start+count) solo es una categoría si el generador la dejó
  // en un bloque seguido. Si algún día deja de serlo, salta aquí y no en forma
  // de efecto equivocado al pulsar un botón.
  for (uint8_t m = 0; m < MODE_COUNT; m++) {
    for (uint16_t p = 0; p < modeCount[m]; p++) {
      if ((uint8_t)COMMANDS[modeStart[m] + p].mode != m) {
        Serial.printf("[catalog] ERR: la categoria %s no esta agrupada\n", CATEGORIES[m].key);
        break;
      }
    }
  }
}

Mode     catalogMode()         { return currentMode; }
uint16_t catalogPosition()     { return modePosition; }
uint16_t catalogCount(Mode m)  { return modeCount[(uint8_t)m]; }
uint16_t catalogCurrentCount() { return modeCount[(uint8_t)currentMode]; }

bool catalogCurrentIndex(uint16_t& outAbsIdx) {
  const uint16_t count = catalogCurrentCount();
  if (count == 0) return false;
  if (modePosition >= count) modePosition = 0;
  outAbsIdx = modeStart[(uint8_t)currentMode] + modePosition;
  return true;
}

void catalogSetMode(Mode m) {
  currentMode = m;
  modePosition = 0;
}

void catalogCycleMode() {
  catalogSetMode((Mode)(((uint8_t)currentMode + 1) % MODE_COUNT));
}

void catalogSetPosition(uint16_t pos) {
  const uint16_t count = catalogCurrentCount();
  if (count == 0) return;
  modePosition = pos % count;
}

void catalogNext() {
  const uint16_t count = catalogCurrentCount();
  if (count == 0) return;
  modePosition = (modePosition + 1) % count;
}

bool catalogSelect(uint16_t absIdx) {
  if (absIdx >= COMMAND_COUNT) return false;

  const uint8_t m = (uint8_t)COMMANDS[absIdx].mode;
  if (m >= MODE_COUNT || absIdx < modeStart[m]) return false;

  currentMode = (Mode)m;
  modePosition = absIdx - modeStart[m];
  return true;
}
