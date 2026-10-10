#include <Arduino.h>
#include <EfectosLED.h>
#include <config.h>
#include <BLEDevice.h>
#include <RemoteXY.h>

// Selección de efectos
int efectoActual = 0;
const int EFECTOS_MAX = 5;
uint8_t colorR = 255;
uint8_t colorG = 255;
uint8_t colorB = 255;

uint32_t colorScanner = 0xFF0000;  // Rojo
uint32_t colorFondo = 0x000064;    // Azul oscuro


String comandoSerial = "";



bool parseRGB(String texto, uint32_t &resultado) {
  int coma1 = texto.indexOf(',');
  int coma2 = texto.indexOf(',', coma1 + 1);

  if (coma1 < 0 || coma2 < 0) return false;

  String sr = texto.substring(0, coma1);
  String sg = texto.substring(coma1 + 1, coma2);
  String sb = texto.substring(coma2 + 1);

  sr.trim();
  sg.trim();
  sb.trim();

  if (sr.length() == 0 || sg.length() == 0 || sb.length() == 0)
    return false;

  for (int i = 0; i < sr.length(); i++)
    if (!isDigit(sr[i])) return false;

  for (int i = 0; i < sg.length(); i++)
    if (!isDigit(sg[i])) return false;

  for (int i = 0; i < sb.length(); i++)
    if (!isDigit(sb[i])) return false;

  int r = sr.toInt();
  int g = sg.toInt();
  int b = sb.toInt();

  if (r > 255 || g > 255 || b > 255) return false;

  resultado = ((uint32_t)r << 16) |
              ((uint32_t)g << 8) |
              (uint32_t)b;

  return true;
}


void leerSerial() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      comandoSerial.trim();
      comandoSerial.toLowerCase();

      if (comandoSerial.length() == 0) continue;

      Serial.print("Comando recibido: ");
      Serial.println(comandoSerial);

      int separador = comandoSerial.indexOf(':');

      String efecto = comandoSerial;
      String parametros = "";

      if (separador >= 0) {
        efecto = comandoSerial.substring(0, separador);
        parametros = comandoSerial.substring(separador + 1);
        parametros.trim();
      }

      if (efecto == "arcoiris" && separador < 0) {
        efectoActual = 0;
        Serial.println("Efecto seleccionado: arcoiris");
      }
      else if (efecto == "rebote" && separador < 0) {
        efectoActual = 1;
        Serial.println("Efecto seleccionado: rebote");
      }

      else if (efecto == "palpitacion" && separador >= 0) {
        int primeraComa = parametros.indexOf(',');
        int segundaComa = parametros.indexOf(',', primeraComa + 1);

        if (primeraComa >= 0 && segundaComa >= 0) {
          String textoR = parametros.substring(0, primeraComa);
          String textoG = parametros.substring(primeraComa + 1, segundaComa);
          String textoB = parametros.substring(segundaComa + 1);

          textoR.trim();
          textoG.trim();
          textoB.trim();

          // Verificar que los tres componentes sean números
          bool formatoValido =
            textoR.length() > 0 &&
            textoG.length() > 0 &&
            textoB.length() > 0;

          for (int i = 0; i < textoR.length(); i++) {
            if (!isDigit(textoR[i])) formatoValido = false;
          }

          for (int i = 0; i < textoG.length(); i++) {
            if (!isDigit(textoG[i])) formatoValido = false;
          }

          for (int i = 0; i < textoB.length(); i++) {
            if (!isDigit(textoB[i])) formatoValido = false;
          }

          if (formatoValido) {
            int r = textoR.toInt();
            int g = textoG.toInt();
            int b = textoB.toInt();

            if (r <= 255 && g <= 255 && b <= 255) {
              colorR = r;
              colorG = g;
              colorB = b;

              efectoActual = 2;

              Serial.print("Palpitacion RGB: ");
              Serial.print(colorR);
              Serial.print(",");
              Serial.print(colorG);
              Serial.print(",");
              Serial.println(colorB);
            } else {
              Serial.println("Error: los valores RGB deben estar entre 0 y 255");
            }
          } else {
            Serial.println("Error: formato RGB invalido");
          }
        } else {
          Serial.println("Formato incorrecto. Usa palpitacion:R,G,B");
        }
      }
      
      else if ((efecto == "scanner" || efecto == "scannerd") && separador >= 0) {

        int separadorBG = parametros.indexOf(":bg:");

        if (separadorBG >= 0) {
          String textoScanner = parametros.substring(0, separadorBG);
          String textoFondo = parametros.substring(separadorBG + 4);

          uint32_t nuevoScanner;
          uint32_t nuevoFondo;

          if (parseRGB(textoScanner, nuevoScanner) &&
              parseRGB(textoFondo, nuevoFondo)) {

            colorScanner = nuevoScanner;
            colorFondo = nuevoFondo;

            if (efecto == "scanner") {
              efectoActual = 3;
              Serial.println("Scanner simple seleccionado");
            } else {
              efectoActual = 4;
              Serial.println("Scanner doble seleccionado");
            }

            Serial.print("Color scanner: 0x");
            Serial.println(colorScanner, HEX);

            Serial.print("Color fondo: 0x");
            Serial.println(colorFondo, HEX);

          } else {
            Serial.println("RGB invalido. Usa valores entre 0 y 255");
          }
        } else {
          Serial.println("Formato: scanner:R,G,B:bg:R,G,B");
        }
      }

      else {
        Serial.println("Comando desconocido");
      }

      comandoSerial = "";
    }
    else {
      comandoSerial += c;
    }
  }
}


void setup() {

  Serial.begin(115200);
  Serial.println("Sistema iniciado");


  iniciarCartel();  
  

}

void loop() {
  leerSerial();

  switch (efectoActual) {
    case 0:
      arcoiris();
      break;
    case 1:
      arcoirisRebote();
      break;
    case 2:
      palpitacion(colorR, colorG, colorB);
      break;
    case 3:
      efectoScanner(false, colorScanner, colorFondo);
      break;
    case 4:
      efectoScanner(true, colorScanner, colorFondo);
      break;
  }

}





