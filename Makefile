# LED Badge Reborn — atajos de desarrollo.
# `make` a secas lista lo que hay.

FQBN      := m5stack:esp32:m5stack_atom
SKETCH    := firmware/led_badge_reborn
BUILD     := $(SKETCH)/build/m5stack.esp32.m5stack_atom
CORE      := m5stack:esp32
BOARD_URL := https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json

# Versiones fijas: una actualización de librería no debe romper la compilación
# sin que nadie haya tocado el código. Si las cambias, cámbialas también en
# scripts/setup.sh.
LIBS := "IRremoteESP8266@2.9.0" "Adafruit NeoPixel@1.15.2"

# Puerto del ATOM. Se detecta solo; si tienes varios conectados, pásalo a mano:
#   make flash PORT=/dev/cu.usbserial-XXXX
PORT ?= $(shell ls /dev/cu.usbserial-* /dev/ttyUSB* /dev/ttyACM* 2>/dev/null | head -1)
BAUD ?= 115200

# Dónde guarda arduino-cli los cores. El fallback cubre instalaciones antiguas
# donde `config get` todavía no existía.
DATA_DIR = $(shell arduino-cli config get directories.data 2>/dev/null || echo $$HOME/.arduino15)

.DEFAULT_GOAL := help
.PHONY: help setup deps build bootapp0 flash monitor check site serve clean bump

help: ## Muestra esta ayuda
	@echo
	@echo "LED Badge Reborn"
	@echo
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) \
		| awk 'BEGIN {FS = ":.*?## "} {printf "  \033[36m%-10s\033[0m %s\n", $$1, $$2}'
	@echo
	@echo "  Puerto detectado: $(if $(PORT),$(PORT),ninguno)"
	@echo

setup: ## Comprueba las dependencias e instala lo que falte
	@./scripts/setup.sh

deps: ## Instala el core y las librerías (lo usa también el CI)
	arduino-cli core update-index --additional-urls "$(BOARD_URL)"
	arduino-cli core install $(CORE) --additional-urls "$(BOARD_URL)"
	arduino-cli lib install $(LIBS)

build: ## Compila el firmware
	arduino-cli compile --fqbn $(FQBN) --export-binaries --warnings default $(SKETCH)

bootapp0: ## Copia boot_app0.bin del core junto a los binarios compilados
	@origen=$$(find "$(DATA_DIR)/packages/m5stack/hardware/esp32" -name boot_app0.bin 2>/dev/null | head -1); \
	 test -n "$$origen" || { echo "No encontré boot_app0.bin en el core. Ejecuta: make deps"; exit 1; }; \
	 cp "$$origen" "$(BUILD)/"

flash: build ## Compila y sube al dispositivo
	@test -n "$(PORT)" || { \
		echo "No hay ningún puerto serie a la vista."; \
		echo "Conecta el ATOM, o indícalo: make flash PORT=/dev/cu.usbserial-XXXX"; \
		exit 1; }
	@echo "Subiendo a $(PORT)"
	@arduino-cli upload --fqbn $(FQBN) -p $(PORT) $(SKETCH) || { \
		echo; \
		echo "Si falló por 'port is busy', tienes un monitor serie abierto ocupando"; \
		echo "el puerto. Ciérralo y repite. Para verlo: lsof $(PORT)"; \
		exit 1; }

monitor: ## Abre la consola serie (escribe 'help' dentro; Ctrl-C para salir)
	@test -n "$(PORT)" || { echo "No hay ningún puerto serie a la vista."; exit 1; }
	arduino-cli monitor -p $(PORT) --config baudrate=$(BAUD)

check: ## Valida los offsets del manifest y que las versiones cuadren
	@python3 scripts/check-manifest.py

site: build bootapp0 ## Arma el instalador web en _site/, igual que el CI
	@rm -rf _site && mkdir -p _site
	@cp docs/index.html docs/manifest.json          _site/
	@cp $(BUILD)/led_badge_reborn.ino.bin           _site/led_badge_reborn.bin
	@cp $(BUILD)/led_badge_reborn.ino.bootloader.bin _site/bootloader.bin
	@cp $(BUILD)/led_badge_reborn.ino.partitions.bin _site/partitions.bin
	@cp $(BUILD)/boot_app0.bin                      _site/boot_app0.bin
	@echo "Instalador armado en _site/"

serve: site ## Sirve el instalador en local para probarlo antes de publicar
	@echo
	@echo "Abre http://localhost:8000 en Chrome o Edge."
	@echo "Web Serial acepta localhost sin HTTPS, así que aquí sí funciona."
	@echo
	@cd _site && python3 -m http.server 8000

clean: ## Borra los artefactos de compilación y el sitio local
	rm -rf $(SKETCH)/build _site

bump: ## Sube la versión en los dos sitios: make bump VERSION=1.1.0
	@test -n "$(VERSION)" || { echo "Falta la versión: make bump VERSION=1.1.0"; exit 1; }
	@echo "$(VERSION)" | grep -Eq '^[0-9]+\.[0-9]+\.[0-9]+$$' \
		|| { echo "La versión debe ser X.Y.Z"; exit 1; }
	@sed -i.bak -E 's/(#define[[:space:]]+FW_VERSION[[:space:]]+")[^"]+(")/\1$(VERSION)\2/' \
		$(SKETCH)/config.h && rm -f $(SKETCH)/config.h.bak
	@sed -i.bak -E 's/("version"[[:space:]]*:[[:space:]]*")[^"]+(")/\1$(VERSION)\2/' \
		docs/manifest.json && rm -f docs/manifest.json.bak
	@$(MAKE) --no-print-directory check
	@echo
	@echo "Versión $(VERSION) puesta en config.h y manifest.json."
	@echo "Cuando lo tengas probado:"
	@echo "  git commit -am 'Version $(VERSION)' && git tag v$(VERSION) && git push --follow-tags"
