#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "../../include/config.h"

#include "EfectosLED.h"

#define ROWS       15
#define COLS       49
#define BRIGHTNESS 160
#define DELAY_MS   25    // velocidad del movimiento (más bajo = más rápido)
#define HUE_SCALE  3     // cuánto separa el hue por columna (ajusta patrón)

/////////////////////////
///////Violeta con chispas
/////////////////////////
#define MAX_CHISPAS_VIOLETA 10
#define VIO_CHISPA_BRILLO_MAX 180   // blanco suave
#define VIO_CHISPA_FADE 4
#define VIO_CHISPA_INTERVALO 220    // ms entre nuevas chispas

Adafruit_NeoPixel strip(NUM_LEDS_CARTEL, PIN_CARTEL, NEO_GRB + NEO_KHZ800);


// --- mapeo ---
const int mapa[ROWS][COLS] = {
{ -1, -1,  8,  7,  6,  5,  4,  3,  2,  1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{ -1,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,254,253,252,251,250,249,248, -1, -1, -1,293,294,295,296,297,298,299,300,301,302,303},
{ 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, -1,110,111,134, -1, -1, -1, -1, -1, -1, -1,149,172,173, -1, -1,239,240,241,242,243,244,245,246,247, -1, -1,292,291,290,289,288,287,286,285,284,283,282},
{ 30, 35, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,109,112,133, -1, -1, -1, -1, -1, -1, -1,150,171,174, -1,228,229,230,231,232,233,234,235,236,237,238, -1,271,272,273,274,275,276,277,278,279,280,281},
{ 31, 34, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,108,113,132, -1, -1, -1, -1, -1, -1, -1,151,170,175, -1,227,219, -1, -1, -1, -1, -1, -1, -1,263,262, -1, -1, -1, -1, -1,324,323,304, -1, -1, -1, -1},
{ 32, 33, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,107,114,131, -1, -1, -1, -1, -1, -1, -1,152,169,176, -1,226,218, -1, -1, -1, -1, -1, -1, -1,264,261, -1, -1, -1, -1, -1,325,322,305, -1, -1, -1, -1},
{ 36, 37, 38, 39, 40, 41, 42, 43, 44, -1, -1, -1,106,115,130, -1, -1, -1, -1, -1, -1, -1,153,168,177, -1,225,217, -1, -1, -1, -1, -1, -1, -1,265,260, -1, -1, -1, -1, -1,326,321,306, -1, -1, -1, -1},
{ -1, 53, 52, 51, 50, 49, 48, 47, 46, 45, -1, -1,105,116,129,148,147,146,145,144,143,142,154,167,178, -1,224,216, -1, -1, -1, -1, -1, -1, -1,266,259, -1, -1, -1, -1, -1,327,320,307, -1, -1, -1, -1},
{ -1, -1, 54, 55, 56, 57, 58, 59, 60, 61, 62, -1,104,117,128,135,136,137,138,139,140,141,155,166,179, -1,223,215, -1, -1, -1, -1, -1, -1, -1,267,258, -1, -1, -1, -1, -1,328,319,308, -1, -1, -1, -1},
{ -1, -1, -1, -1, -1, -1, -1, -1, -1, 68, 63, -1,103,118,127, -1, -1, -1, -1, -1, -1, -1,156,165,180, -1,222,214, -1, -1, -1, -1, -1, -1, -1,268,257, -1, -1, -1, -1, -1,329,318,309, -1, -1, -1, -1},
{ -1, -1, -1, -1, -1, -1, -1, -1, -1, 67, 64, -1,102,119,126, -1, -1, -1, -1, -1, -1, -1,157,164,181, -1,221,213, -1, -1, -1, -1, -1, -1, -1,269,256, -1, -1, -1, -1, -1,330,317,310, -1, -1, -1, -1},
{ -1, -1, -1, -1, -1, -1, -1, -1, -1, 66, 65, -1,101,120,125, -1, -1, -1, -1, -1, -1, -1,158,163,182, -1,220,212, -1, -1, -1, -1, -1, -1, -1,270,255, -1, -1, -1, -1, -1,331,316,311, -1, -1, -1, -1},
{ 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, -1,100,121,124, -1, -1, -1, -1, -1, -1, -1,159,162,183, -1,211,210,209,208,207,206,205,204,203,202,201, -1, -1, -1, -1, -1,332,315,312, -1, -1, -1, -1},
{ 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, -1, -1, 99,122,123, -1, -1, -1, -1, -1, -1, -1,160,161,184, -1, -1,192,193,194,195,196,197,198,199,200, -1, -1, -1, -1, -1, -1,333,314,313, -1, -1, -1, -1},
{ 98, 97, 96, 95, 94, 93, 92, 91, 90, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,185,186,187,188,189,190,191, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};




uint16_t velocidadPalpitacion = 20; // MÁS grande = más lento

int chispaVioletaPos[MAX_CHISPAS_VIOLETA];
int chispaVioletaBrillo[MAX_CHISPAS_VIOLETA];
unsigned long lastChispaVioleta = 0;

struct Chispa {
  int led;
  uint8_t brillo;
  bool activa;
};
//////////////////////
//ARCOIRIS
//////////////////////
int velocidadArcoiris = 30; //+mas alto = +lento & +mas bajo = +lento 

// Función Wheel -> devuelve uint32_t con strip.Color(r,g,b) Sirve para el arcoiris
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}


///////////////////////
//SCANER
///////////////////////
// Colores
uint32_t shtFondoScanner;
uint32_t shtColorScanner;

// Parámetros ajustables
int velocidadScanner = 20;   // ms entre frames
int shtLargoColaScanner = 5;           // controla cuántos LEDs atrás del scanner pierden intensidad.
int pasoFade = 40;           // cuánto baja intensidad por paso 
int scannerCol = 0;
int scannerDir = 1;
int shtScannerMinCol = COLS;
int shtScannerMaxCol = 0;
unsigned long shtLastUpdateScanner = 0;

int shtScannerColA;
int shtScannerColB;
int shtScannerDirA = 1;
int shtScannerDirB = -1;



void iniciarCartel() {
    strip.begin();
    strip.setBrightness(BRIGHTNESS);
    strip.show();


      // scanner
  shtFondoScanner   = strip.Color(0, 8, 0);     // Color del fondo
  shtColorScanner = strip.Color(0,200, 0);   // Color del scaner

  // Calcular límites reales
  for (int f = 0; f < ROWS; f++) {
    for (int c = 0; c < COLS; c++) {
      if (mapa[f][c] != -1) {
        if (c < shtScannerMinCol) shtScannerMinCol = c;
        if (c > shtScannerMaxCol) shtScannerMaxCol = c;
      }
    }
  }

  scannerCol = shtScannerMinCol;
  shtScannerColA = shtScannerMinCol;
  shtScannerColB = shtScannerMaxCol;
}


void palpitacion(uint8_t r, uint8_t g, uint8_t b) {
  static uint32_t lastTime = 0;
  static int brillo = 0;
  static int direccion = 1;

  if (millis() - lastTime < velocidadPalpitacion) return;
  lastTime = millis();

  brillo += direccion * 5;

  if (brillo >= 200) {
    brillo = 200;
    direccion = -1;
  }

  if (brillo <= 0) {
    brillo = 0;
    direccion = 1;
  }

  // Aplicamos el brillo a cada componente RGB
  uint8_t rojo = (r * brillo) / 255;
  uint8_t verde = (g * brillo) / 255;
  uint8_t azul = (b * brillo) / 255;

  for (int i = 0; i < NUM_LEDS_CARTEL; i++) {
    strip.setPixelColor(i, strip.Color(rojo, verde, azul));
  }

  strip.show();
}

void violetaConChispas() {
  // 🌸 base violeta suave
  for (int i = 0; i < NUM_LEDS_CARTEL; i++) {
    strip.setPixelColor(i, 90, 0, 120); // violeta no al palo
  }

  // ✨ generar chispa
  if (millis() - lastChispaVioleta > VIO_CHISPA_INTERVALO) {
    lastChispaVioleta = millis();

    for (int i = 0; i < MAX_CHISPAS_VIOLETA; i++) {
      if (chispaVioletaBrillo[i] <= 0) {
        chispaVioletaPos[i] = random(NUM_LEDS_CARTEL);
        chispaVioletaBrillo[i] = random(100, VIO_CHISPA_BRILLO_MAX);
        break;
      }
    }
  }

  // ✨ dibujar y apagar chispas
  for (int i = 0; i < MAX_CHISPAS_VIOLETA; i++) {
    if (chispaVioletaBrillo[i] > 0) {
      int b = chispaVioletaBrillo[i];
      strip.setPixelColor(
        chispaVioletaPos[i],
        b, b, b   // blanco suave
      );
      chispaVioletaBrillo[i] -= VIO_CHISPA_FADE;
    }
  }

  strip.show();
}

void arcoiris() {
  static uint32_t lastTime = 0;
  static int offset = 0;

  if (millis() - lastTime < velocidadArcoiris) return;
  lastTime = millis();

  uint32_t colorLed1 = 0;

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      int led = mapa[r][c];
      if (led == -1) continue;

      int pos = (c + offset) % COLS;
      uint8_t colorIndex = pos * 256 / COLS;
      uint32_t color = Wheel(colorIndex);

      if (led == 1) {
        colorLed1 = color;   // guardamos el color del LED 1
      }

      strip.setPixelColor(led, color);
    }
  }

  // tratamiento especial 😎
  strip.setPixelColor(0, colorLed1);

  strip.show();
  offset = (offset + 1) % COLS;
}

void arcoirisRebote() {
  static uint32_t lastTime = 0;
  static int offset = 0;
  static int direccion = 1;   // 1 = derecha, -1 = izquierda

  if (millis() - lastTime < velocidadArcoiris) return;
  lastTime = millis();

  uint32_t colorLed1 = 0;

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      int led = mapa[r][c];
      if (led == -1) continue;

      int pos = c + offset;
      if (pos < 0) pos += COLS;
      if (pos >= COLS) pos -= COLS;

      uint8_t colorIndex = pos * 256 / COLS;
      uint32_t color = Wheel(colorIndex);

      if (led == 1) colorLed1 = color;

      strip.setPixelColor(led, color);
    }
  }

  // LED 0 bajo protección federal 😎
  strip.setPixelColor(0, colorLed1);

  strip.show();

  offset += direccion;

  if (offset >= COLS - 1 || offset <= 0) {
    direccion *= -1;   // rebote
  }
}



void dibujarColaScanner(int posicion, uint32_t colorScanner, bool parcheLED0) {
  for (int f = 0; f < ROWS; f++) {
    for (int offset = -shtLargoColaScanner;
         offset <= shtLargoColaScanner; offset++) {

      int col = posicion + offset;

      if (col < shtScannerMinCol || col > shtScannerMaxCol) continue;

      int index = mapa[f][col];
      if (index == -1) continue;

      int intensidad = 255 - abs(offset) * pasoFade;
      if (intensidad < 0) intensidad = 0;

      uint8_t r = ((colorScanner >> 16) & 0xFF) * intensidad / 255;
      uint8_t g = ((colorScanner >> 8) & 0xFF) * intensidad / 255;
      uint8_t b = (colorScanner & 0xFF) * intensidad / 255;

      if (parcheLED0 && index == 0 && posicion > 25) {
        strip.setPixelColor(0, shtFondoScanner);
      } else {
        strip.setPixelColor(index, r, g, b);
      }
    }
  }
}


void efectoScanner(bool doble, uint32_t colorScanner, uint32_t colorFondo) {
  if (millis() - shtLastUpdateScanner < velocidadScanner) return;
  shtLastUpdateScanner = millis();

  // Actualizar el fondo
  shtColorScanner = colorScanner;
  shtFondoScanner = colorFondo;

  for (int i = 0; i < NUM_LEDS_CARTEL; i++) {
    strip.setPixelColor(i, colorFondo);
  }

  // Dibujar scanner simple o doble
  if (doble) {
    dibujarColaScanner(shtScannerColA, colorScanner, false);
    dibujarColaScanner(shtScannerColB, colorScanner, false);

    shtScannerColA += shtScannerDirA;
    shtScannerColB += shtScannerDirB;

    if (shtScannerColA >= shtScannerMaxCol) {
      shtScannerColA = shtScannerMaxCol;
      shtScannerDirA = -1;
    }
    if (shtScannerColA <= shtScannerMinCol) {
      shtScannerColA = shtScannerMinCol;
      shtScannerDirA = 1;
    }

    if (shtScannerColB >= shtScannerMaxCol) {
      shtScannerColB = shtScannerMaxCol;
      shtScannerDirB = -1;
    }
    if (shtScannerColB <= shtScannerMinCol) {
      shtScannerColB = shtScannerMinCol;
      shtScannerDirB = 1;
    }
  } else {
    dibujarColaScanner(scannerCol, colorScanner, true);

    scannerCol += scannerDir;

    if (scannerCol >= shtScannerMaxCol) {
      scannerCol = shtScannerMaxCol;
      scannerDir = -1;
    }
    if (scannerCol <= shtScannerMinCol) {
      scannerCol = shtScannerMinCol;
      scannerDir = 1;
    }
  }

  strip.show();
}
