#include "app.h"
#include "config.h"
#include "ir_sender.h"
#include "status_led.h"

void appBegin() {
  catalogBegin();
  irBegin();
  appRefreshLed();
}

void appRefreshLed() {
  switch (catalogMode()) {  // SPECIAL=blanco, PULSO=verde, FADE=azul
    case Mode::SPECIAL: ledSetBase(255, 255, 255); break;
    case Mode::PULSO:   ledSetBase(0, 255, 0);     break;
    case Mode::FADE:    ledSetBase(0, 0, 255);     break;
  }
}

bool appSendCurrent() {
  uint16_t absIdx = 0;
  if (!catalogCurrentIndex(absIdx)) {
    Serial.printf("ERR: la categoria %s esta vacia\n", modeName(catalogMode()));
    return false;
  }

  const Command& cmd = COMMANDS[absIdx];
  Serial.printf("SEND mode=%s pos=%u abs=%u label=%s\n",
                modeName(catalogMode()), catalogPosition(), absIdx, cmd.label);

  if (!irSendPronto(cmd.pronto)) {
    Serial.println("ERR: trama PRONTO invalida");
    return false;
  }

  ledFlash(255, 0, 0, FLASH_MS);
  return true;
}

bool appSendAbsolute(uint16_t absIdx) {
  if (!catalogSelect(absIdx)) return false;
  appRefreshLed();
  return appSendCurrent();
}

void appSetMode(Mode m) {
  catalogSetMode(m);
  appRefreshLed();
  appPrintStatus();
}

void appCycleMode() {
  catalogCycleMode();
  appRefreshLed();
  appPrintStatus();
}

void appSetPosition(uint16_t pos) {
  catalogSetPosition(pos);
  appPrintStatus();
}

void appNext() {
  catalogNext();
}

void appPrintStatus() {
  uint16_t absIdx = 0;
  if (!catalogCurrentIndex(absIdx)) {
    Serial.printf("STATUS mode=%s (vacia)\n", modeName(catalogMode()));
    return;
  }

  Serial.printf("STATUS mode=%s pos=%u/%u abs=%u label=%s\n",
                modeName(catalogMode()),
                catalogPosition(),
                (unsigned)(catalogCurrentCount() - 1),
                absIdx,
                COMMANDS[absIdx].label);
}
