#pragma once
#include <Arduino.h>

// Catálogo de comandos IR: datos estáticos + navegación por categoría.

enum class Mode : uint8_t { SPECIAL = 0, PULSO = 1, FADE = 2 };
static const uint8_t MODE_COUNT = 3;

struct Command {
  const char* label;
  const char* pronto;  // PRONTO_HEX
  Mode mode;
};

extern const Command COMMANDS[];
extern const uint16_t COMMAND_COUNT;

const char* modeName(Mode m);
bool modeFromName(const char* name, Mode& out);

void catalogBegin();

Mode     catalogMode();
uint16_t catalogPosition();
uint16_t catalogCount(Mode m);
uint16_t catalogCurrentCount();

// Devuelve false si la categoría actual no tiene comandos.
bool catalogCurrentIndex(uint16_t& outAbsIdx);

void catalogSetMode(Mode m);   // reinicia la posición a 0
void catalogCycleMode();
void catalogSetPosition(uint16_t pos);
void catalogNext();

// Fija categoría y posición a partir de un índice absoluto de COMMANDS[].
bool catalogSelect(uint16_t absIdx);
