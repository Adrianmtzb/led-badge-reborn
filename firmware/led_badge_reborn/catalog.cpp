#include "catalog.h"
#include "pronto_data.h"

// =====================================================
// Registro de comandos
// =====================================================
const Command COMMANDS[] = {
  { "SPECIAL_00", P_SPECIAL_00, Mode::SPECIAL },
  { "SPECIAL_01", P_SPECIAL_01, Mode::SPECIAL },
  { "SPECIAL_02", P_SPECIAL_02, Mode::SPECIAL },

  { "PULSO_00", P_PULSO_00, Mode::PULSO },
  { "PULSO_01", P_PULSO_01, Mode::PULSO },
  { "PULSO_02", P_PULSO_02, Mode::PULSO },
  { "PULSO_03", P_PULSO_03, Mode::PULSO },
  { "PULSO_04", P_PULSO_04, Mode::PULSO },
  { "PULSO_05", P_PULSO_05, Mode::PULSO },
  { "PULSO_06", P_PULSO_06, Mode::PULSO },
  { "PULSO_07", P_PULSO_07, Mode::PULSO },
  { "PULSO_08", P_PULSO_08, Mode::PULSO },
  { "PULSO_09", P_PULSO_09, Mode::PULSO },
  { "PULSO_10", P_PULSO_10, Mode::PULSO },
  { "PULSO_11", P_PULSO_11, Mode::PULSO },
  { "PULSO_12", P_PULSO_12, Mode::PULSO },
  { "PULSO_13", P_PULSO_13, Mode::PULSO },
  { "PULSO_14", P_PULSO_14, Mode::PULSO },
  { "PULSO_15", P_PULSO_15, Mode::PULSO },
  { "PULSO_16", P_PULSO_16, Mode::PULSO },
  { "PULSO_17", P_PULSO_17, Mode::PULSO },
  { "PULSO_18", P_PULSO_18, Mode::PULSO },
  { "PULSO_19", P_PULSO_19, Mode::PULSO },
  { "PULSO_20", P_PULSO_20, Mode::PULSO },
  { "PULSO_21", P_PULSO_21, Mode::PULSO },
  { "PULSO_22", P_PULSO_22, Mode::PULSO },
  { "PULSO_23", P_PULSO_23, Mode::PULSO },
  { "PULSO_24", P_PULSO_24, Mode::PULSO },

  { "FADE_00", P_FADE_00, Mode::FADE },
  { "FADE_01", P_FADE_01, Mode::FADE },
  { "FADE_02", P_FADE_02, Mode::FADE },
  { "FADE_03", P_FADE_03, Mode::FADE },
  { "FADE_04", P_FADE_04, Mode::FADE },
  { "FADE_05", P_FADE_05, Mode::FADE },
  { "FADE_06", P_FADE_06, Mode::FADE },
  { "FADE_07", P_FADE_07, Mode::FADE },
  { "FADE_08", P_FADE_08, Mode::FADE },
  { "FADE_09", P_FADE_09, Mode::FADE },
  { "FADE_10", P_FADE_10, Mode::FADE },
  { "FADE_11", P_FADE_11, Mode::FADE },
  { "FADE_12", P_FADE_12, Mode::FADE },
  { "FADE_13", P_FADE_13, Mode::FADE },
  { "FADE_14", P_FADE_14, Mode::FADE },
  { "FADE_15", P_FADE_15, Mode::FADE },
  { "FADE_16", P_FADE_16, Mode::FADE },
  { "FADE_17", P_FADE_17, Mode::FADE },
  { "FADE_18", P_FADE_18, Mode::FADE },
  { "FADE_19", P_FADE_19, Mode::FADE },
  { "FADE_20", P_FADE_20, Mode::FADE },
  { "FADE_21", P_FADE_21, Mode::FADE },
  { "FADE_22", P_FADE_22, Mode::FADE },
  { "FADE_23", P_FADE_23, Mode::FADE },
  { "FADE_24", P_FADE_24, Mode::FADE },
  { "FADE_25", P_FADE_25, Mode::FADE },
  { "FADE_26", P_FADE_26, Mode::FADE },
};

static const uint16_t kCommandCount = sizeof(COMMANDS) / sizeof(COMMANDS[0]);
const uint16_t COMMAND_COUNT = kCommandCount;

// =====================================================
// Estado de navegación
// =====================================================
static uint16_t modeIndexMap[MODE_COUNT][kCommandCount];
static uint16_t modeCount[MODE_COUNT];

static Mode currentMode = Mode::PULSO;
static uint16_t modePosition = 0;

const char* modeName(Mode m) {
  switch (m) {
    case Mode::SPECIAL: return "SPECIAL";
    case Mode::PULSO:   return "PULSO";
    case Mode::FADE:    return "FADE";
  }
  return "?";
}

bool modeFromName(const char* name, Mode& out) {
  if (!name) return false;
  if (!strcasecmp(name, "special")) { out = Mode::SPECIAL; return true; }
  if (!strcasecmp(name, "pulso"))   { out = Mode::PULSO;   return true; }
  if (!strcasecmp(name, "fade"))    { out = Mode::FADE;    return true; }
  return false;
}

void catalogBegin() {
  for (uint8_t m = 0; m < MODE_COUNT; m++) modeCount[m] = 0;

  for (uint16_t i = 0; i < kCommandCount; i++) {
    const uint8_t m = (uint8_t)COMMANDS[i].mode;
    modeIndexMap[m][modeCount[m]++] = i;
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
  outAbsIdx = modeIndexMap[(uint8_t)currentMode][modePosition];
  return true;
}

void catalogSetMode(Mode m) {
  currentMode = m;
  modePosition = 0;
}

void catalogCycleMode() {
  // SPECIAL -> PULSO -> FADE -> SPECIAL
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
  if (absIdx >= kCommandCount) return false;

  const Mode m = COMMANDS[absIdx].mode;
  const uint8_t mi = (uint8_t)m;

  for (uint16_t p = 0; p < modeCount[mi]; p++) {
    if (modeIndexMap[mi][p] == absIdx) {
      currentMode = m;
      modePosition = p;
      return true;
    }
  }
  return false;
}
