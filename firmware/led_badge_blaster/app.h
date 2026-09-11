#pragma once
#include "catalog.h"

// Capa que une catálogo, emisor IR y LED. La usan tanto el botón como la API.

void appBegin();

bool appSendCurrent();
bool appSendAbsolute(uint16_t absIdx);

void appSetMode(Mode m);
void appCycleMode();
void appSetPosition(uint16_t pos);
void appNext();

void appRefreshLed();
void appPrintStatus();
