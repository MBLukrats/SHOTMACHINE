
#ifndef CONTROL_CARTEL_H
#define CONTROL_CARTEL_H

#include <Arduino.h>

// Selección de efecto
bool seleccionarEfecto(int efecto);
int obtenerEfectoActual();

// Colores
void establecerColorPrincipal(uint32_t color);
void establecerColorFondo(uint32_t color);
void establecerColores(uint32_t principal, uint32_t fondo);

uint32_t obtenerColorPrincipalRGB();
uint32_t obtenerColorFondoRGB();

// Color principal separado en canales para palpitacion()
uint8_t obtenerColorR();
uint8_t obtenerColorG();
uint8_t obtenerColorB();

#endif
