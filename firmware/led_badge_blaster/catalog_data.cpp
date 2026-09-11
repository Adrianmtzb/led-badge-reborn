#include "catalog.h"
#include "pronto_data.h"

// GENERADO POR scripts/gen_catalog.py — NO EDITAR A MANO.
// Fuente: data/presets/*.json (exportaciones del generador web).
// Regenerar con: make catalog

const Command COMMANDS[] = {
  { "CAP_00",  "Rojo",              "P_PULSO_00",              0xF02000, P_CAP_00, Mode::CAPTURAS },
  { "CAP_01",  "Naranja",           "P_PULSO_01",              0xF05000, P_CAP_01, Mode::CAPTURAS },
  { "CAP_02",  "Naranja oscuro",    "P_PULSO_02",              0xB84400, P_CAP_02, Mode::CAPTURAS },
  { "CAP_03",  "Amarillo dorado",   "P_PULSO_03",              0xE4BC00, P_CAP_03, Mode::CAPTURAS },
  { "CAP_04",  "Ámbar",             "P_PULSO_04",              0xE49400, P_CAP_04, Mode::CAPTURAS },
  { "CAP_05",  "Oliva",             "P_PULSO_05",              0xB8BC00, P_CAP_05, Mode::CAPTURAS },
  { "CAP_06",  "Verde",             "P_PULSO_06",              0x10D000, P_CAP_06, Mode::CAPTURAS },
  { "CAP_07",  "Verde claro",       "P_PULSO_07",              0x84F05C, P_CAP_07, Mode::CAPTURAS },
  { "CAP_08",  "Verde menta",       "P_PULSO_08",              0x68F0A0, P_CAP_08, Mode::CAPTURAS },
  { "CAP_09",  "Cian",              "P_PULSO_09",              0x34F0FC, P_CAP_09, Mode::CAPTURAS },
  { "CAP_10",  "Azul cielo",        "P_PULSO_10",              0x1094DC, P_CAP_10, Mode::CAPTURAS },
  { "CAP_11",  "Azul",              "P_PULSO_11",              0x480CFC, P_CAP_11, Mode::CAPTURAS },
  { "CAP_12",  "Morado",            "P_PULSO_12",              0x8838FC, P_CAP_12, Mode::CAPTURAS },
  { "CAP_13",  "Púrpura",           "P_PULSO_13",              0xB80CDC, P_CAP_13, Mode::CAPTURAS },
  { "CAP_14",  "Ciruela",           "P_PULSO_14",              0x840C5C, P_CAP_14, Mode::CAPTURAS },
  { "CAP_15",  "Frambuesa",         "P_PULSO_15",              0xE4385C, P_CAP_15, Mode::CAPTURAS },
  { "CAP_16",  "Fucsia",            "P_PULSO_16",              0xCC0C5C, P_CAP_16, Mode::CAPTURAS },
  { "CAP_17",  "Rosa",              "P_PULSO_17",              0xEC74A0, P_CAP_17, Mode::CAPTURAS },
  { "CAP_18",  "Melocotón",         "P_PULSO_18",              0xF09C5C, P_CAP_18, Mode::CAPTURAS },
  { "CAP_19",  "Salmón",            "P_PULSO_19",              0xF0745C, P_CAP_19, Mode::CAPTURAS },
  { "CAP_20",  "Lavanda",           "P_PULSO_20",              0xB894FC, P_CAP_20, Mode::CAPTURAS },
  { "CAP_21",  "Verde lima",        "P_PULSO_21",              0xCCF05C, P_CAP_21, Mode::CAPTURAS },
  { "CAP_22",  "Azul pálido",       "P_PULSO_22",              0xB8D0FC, P_CAP_22, Mode::CAPTURAS },
  { "CAP_23",  "Violeta",           "P_PULSO_23",              0xB80CFC, P_CAP_23, Mode::CAPTURAS },
  { "CAP_24",  "Magenta",           "P_PULSO_24",              0xF034FC, P_CAP_24, Mode::CAPTURAS },

  { "BAS_00",  "Rojo pleno",        "",                        0xFC0000, P_BAS_00, Mode::BASICOS },
  { "BAS_01",  "Verde pleno",       "",                        0x00FC00, P_BAS_01, Mode::BASICOS },
  { "BAS_02",  "Azul pleno",        "",                        0x0000FC, P_BAS_02, Mode::BASICOS },
  { "BAS_03",  "Cyan",              "",                        0x00FCFC, P_BAS_03, Mode::BASICOS },
  { "BAS_04",  "Magenta",           "",                        0xFC00FC, P_BAS_04, Mode::BASICOS },
  { "BAS_05",  "Amarillo",          "",                        0xFCFC00, P_BAS_05, Mode::BASICOS },
  { "BAS_06",  "Blanco",            "",                        0xFCFCFC, P_BAS_06, Mode::BASICOS },
  { "BAS_07",  "Blanco cálido",     "",                        0xFCB46C, P_BAS_07, Mode::BASICOS },
  { "BAS_08",  "Blanco frío",       "",                        0xD8ECFC, P_BAS_08, Mode::BASICOS },
  { "BAS_09",  "Apagado",           "Corta el efecto en curso", 0x000000, P_BAS_09, Mode::BASICOS },

  { "ENV_00",  "Destello",          "Ataque instantáneo, 32 ms de sostén", 0xFCFCFC, P_ENV_00, Mode::ENVOLVENTES },
  { "ENV_01",  "Pulso corto",       "32 / 96 / 96 ms",         0xFC5000, P_ENV_01, Mode::ENVOLVENTES },
  { "ENV_02",  "Respiración",       "960 ms de subida y bajada", 0x008CFC, P_ENV_02, Mode::ENVOLVENTES },
  { "ENV_03",  "Fundido lento",     "2.4 s de subida, casi 4.4 s en total", 0xB400FC, P_ENV_03, Mode::ENVOLVENTES },
  { "ENV_04",  "Onda larga",        "El envolvente más largo posible", 0x00FCB4, P_ENV_04, Mode::ENVOLVENTES },
  { "ENV_05",  "Golpe seco",        "Sustain 0: apenas un parpadeo", 0xFC003C, P_ENV_05, Mode::ENVOLVENTES },
  { "ENV_06",  "Brasa",             "Sube en 480 ms y se apaga muy despacio", 0xFC6400, P_ENV_06, Mode::ENVOLVENTES },
  { "ENV_07",  "Latido",            "Corto y con cola",        0xFC0000, P_ENV_07, Mode::ENVOLVENTES },

  { "MUL_00",  "Chispeo denso",     "67 % de los dispositivos", 0xFCFCFC, P_MUL_00, Mode::MULTITUD },
  { "MUL_01",  "Chispeo medio",     "50 %",                    0xFCECB4, P_MUL_01, Mode::MULTITUD },
  { "MUL_02",  "Chispeo disperso",  "16 %",                    0xB4DCFC, P_MUL_02, Mode::MULTITUD },
  { "MUL_03",  "Estrellas raras",   "4 %, puntos aislados en la multitud", 0xFCFCD8, P_MUL_03, Mode::MULTITUD },
  { "MUL_04",  "Lluvia azul",       "32 %, envolvente suave",  0x0078FC, P_MUL_04, Mode::MULTITUD },
  { "MUL_05",  "Ascuas dispersas",  "10 %, muy lento",         0xFC3C00, P_MUL_05, Mode::MULTITUD },

  { "ESC_00",  "Atardecer · 1",     "",                        0xFC7800, P_ESC_00, Mode::ESCENAS },
  { "ESC_01",  "Atardecer · 2",     "",                        0xFC3C28, P_ESC_01, Mode::ESCENAS },
  { "ESC_02",  "Atardecer · 3",     "",                        0xB41450, P_ESC_02, Mode::ESCENAS },
  { "ESC_03",  "Océano · 1",        "",                        0x0064B4, P_ESC_03, Mode::ESCENAS },
  { "ESC_04",  "Océano · 2",        "",                        0x00B4C8, P_ESC_04, Mode::ESCENAS },
  { "ESC_05",  "Océano · 3",        "",                        0x003C8C, P_ESC_05, Mode::ESCENAS },
  { "ESC_06",  "Bosque · 1",        "",                        0x14A028, P_ESC_06, Mode::ESCENAS },
  { "ESC_07",  "Bosque · 2",        "",                        0x78C800, P_ESC_07, Mode::ESCENAS },
  { "ESC_08",  "Neón · 1",          "",                        0xFC00C8, P_ESC_08, Mode::ESCENAS },
  { "ESC_09",  "Neón · 2",          "",                        0x00FCDC, P_ESC_09, Mode::ESCENAS },
  { "ESC_10",  "Neón · 3",          "",                        0xC8FC00, P_ESC_10, Mode::ESCENAS },
  { "ESC_11",  "Hielo · 1",         "",                        0xC8F0FC, P_ESC_11, Mode::ESCENAS },
  { "ESC_12",  "Hielo · 2",         "",                        0x78C8FC, P_ESC_12, Mode::ESCENAS },

  { "GRP_00",  "Broadcast",         "Grupo 0: todos",          0xFCFCFC, P_GRP_00, Mode::GRUPOS },
  { "GRP_01",  "Grupo 1 · rojo",    "",                        0xFC0000, P_GRP_01, Mode::GRUPOS },
  { "GRP_02",  "Grupo 2 · verde",   "",                        0x00FC00, P_GRP_02, Mode::GRUPOS },
  { "GRP_03",  "Grupo 3 · azul",    "",                        0x0000FC, P_GRP_03, Mode::GRUPOS },
  { "GRP_04",  "Grupo 4 · ámbar",   "",                        0xFCA000, P_GRP_04, Mode::GRUPOS },
};

const uint16_t COMMAND_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

const CategoryInfo CATEGORIES[MODE_COUNT] = {
  { "CAPTURAS",     "Capturas",     "Colores decodificados de capturas reales. La referencia de compatibilidad más fiable." },
  { "BASICOS",      "Básicos",      "Primarios y secundarios al máximo que el badge puede representar." },
  { "ENVOLVENTES",  "Envolventes",  "Un mismo color con distinta forma temporal: attack, sustain y release." },
  { "MULTITUD",     "Multitud",     "Usan el campo chance: solo una fracción aleatoria de los badges responde." },
  { "ESCENAS",      "Escenas",      "Paletas coherentes pensadas para lanzarse en secuencia." },
  { "GRUPOS",       "Grupos",       "El mismo color restringido a un group id. Solo responden los badges de ese grupo." },
};
