
#include "ControlCartel.h"

static const int EFECTOS_MAX = 5;

static int efectoActual = 0;

static uint32_t colorPrincipal = 0xFF0000;
static uint32_t colorFondo = 0x000064;

bool seleccionarEfecto(int efecto) {
  if (efecto < 0 || efecto >= EFECTOS_MAX) {
    return false;
  }

  efectoActual = efecto;
  return true;
}

int obtenerEfectoActual() {
  return efectoActual;
}

void establecerColorPrincipal(uint32_t color) {
  colorPrincipal = color & 0xFFFFFF;
}

void establecerColorFondo(uint32_t color) {
  colorFondo = color & 0xFFFFFF;
}

void establecerColores(uint32_t principal, uint32_t fondo) {
  establecerColorPrincipal(principal);
  establecerColorFondo(fondo);
}

uint32_t obtenerColorPrincipalRGB() {
  return colorPrincipal;
}

uint32_t obtenerColorFondoRGB() {
  return colorFondo;
}

uint8_t obtenerColorR() {
  return (colorPrincipal >> 16) & 0xFF;
}

uint8_t obtenerColorG() {
  return (colorPrincipal >> 8) & 0xFF;
}

uint8_t obtenerColorB() {
  return colorPrincipal & 0xFF;
}
