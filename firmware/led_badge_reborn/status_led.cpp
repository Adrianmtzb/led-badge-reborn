#include "status_led.h"
#include "config.h"

#include <Adafruit_NeoPixel.h>

static Adafruit_NeoPixel rgb(1, PIN_RGBLED, NEO_GRB + NEO_KHZ800);

// El color lógico se guarda aparte: Adafruit_NeoPixel::getPixelColor() devuelve
// el valor ya escalado por el brillo, y reinyectarlo lo vuelve a escalar,
// atenuando el LED en cada ida y vuelta.
static uint8_t baseR = 0, baseG = 0, baseB = 0;

static bool flashActive = false;
static uint32_t flashEndMs = 0;

static void paint(uint8_t r, uint8_t g, uint8_t b) {
  rgb.setPixelColor(0, rgb.Color(r, g, b));
  rgb.show();
}

void ledBegin() {
  rgb.begin();
  rgb.setBrightness(LED_BRIGHTNESS);
  paint(0, 0, 0);
}

void ledSetBase(uint8_t r, uint8_t g, uint8_t b) {
  baseR = r;
  baseG = g;
  baseB = b;
  if (!flashActive) paint(r, g, b);
}

void ledFlash(uint8_t r, uint8_t g, uint8_t b, uint32_t ms) {
  paint(r, g, b);
  flashActive = true;
  flashEndMs = millis() + ms;
}

void ledUpdate() {
  if (!flashActive) return;
  if ((int32_t)(millis() - flashEndMs) < 0) return;

  flashActive = false;
  paint(baseR, baseG, baseB);
}
