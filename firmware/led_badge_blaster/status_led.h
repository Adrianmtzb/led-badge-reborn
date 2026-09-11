#pragma once
#include <Arduino.h>

void ledBegin();

// Color de reposo.
void ledSetBase(uint8_t r, uint8_t g, uint8_t b);

// Destello temporal que luego revierte al color de reposo.
void ledFlash(uint8_t r, uint8_t g, uint8_t b, uint32_t ms);

void ledUpdate();
