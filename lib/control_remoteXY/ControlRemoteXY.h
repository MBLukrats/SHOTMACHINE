#ifndef CONTROL_REMOTEXY_H
#define CONTROL_REMOTEXY_H

#include <Arduino.h>

void iniciarControlRemoteXY();
void actualizarControlRemoteXY();

bool remoteXYConectado();

bool obtenerColorPrincipalRGB(uint32_t &color);
bool obtenerColorFondoRGB(uint32_t &color);

uint8_t obtenerEfectoRemoteXY();

const char* obtenerColorPrincipal();
const char* obtenerColorFondo();

bool leerCambiosRemoteXY(
    int &efecto,
    bool &cambioEfecto,
    uint32_t &principal,
    bool &cambioPrincipal,
    uint32_t &fondo,
    bool &cambioFondo
);

#endif