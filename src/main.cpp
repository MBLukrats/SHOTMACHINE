
#include <Arduino.h>

#include "ControlCartel.h"
#include "ControlSerial.h"
#include "ControlRemoteXY.h"
#include "EfectosLED.h"

void setup() {
  iniciarControlSerial();
  iniciarControlRemoteXY();
  iniciarCartel();

  Serial.println("Cartel listo");
}

void loop() {
  // 1. Actualizar RemoteXY y detectar cambios
  actualizarControlRemoteXY();

  int efectoRemoto;
  bool cambioEfecto;
  bool cambioPrincipal;
  bool cambioFondo;

  uint32_t principalRemoto;
  uint32_t fondoRemoto;

  leerCambiosRemoteXY(
    efectoRemoto,
    cambioEfecto,
    principalRemoto,
    cambioPrincipal,
    fondoRemoto,
    cambioFondo
  );

  // 2. Aplicar únicamente los cambios de RemoteXY
  if (cambioEfecto) {
    if (!seleccionarEfecto(efectoRemoto)) {
      Serial.println("Efecto RemoteXY fuera de rango");
    } else {
      Serial.print("Efecto seleccionado desde RemoteXY: ");
      Serial.println(efectoRemoto);
    }
  }

  if (cambioPrincipal) {
    establecerColorPrincipal(principalRemoto);
  }

  if (cambioFondo) {
    establecerColorFondo(fondoRemoto);
  }

  // 3. Leer Serial después de RemoteXY.
  // Si llegan órdenes en la misma vuelta, Serial tiene prioridad.
  leerSerial();

  // 4. Ejecutar el efecto activo usando el estado compartido
  switch (obtenerEfectoActual()) {
    case 0:
      arcoiris();
      break;

    case 1:
      arcoirisRebote();
      break;

    case 2:
      palpitacion(
        obtenerColorR(),
        obtenerColorG(),
        obtenerColorB()
      );
      break;

    case 3:
      efectoScanner(
        false,
        obtenerColorPrincipalRGB(),
        obtenerColorFondoRGB()
      );
      break;

    case 4:
      efectoScanner(
        true,
        obtenerColorPrincipalRGB(),
        obtenerColorFondoRGB()
      );
      break;
  }
}
