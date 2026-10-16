
#include "ControlSerial.h"
#include "ControlCartel.h"

static String comandoSerial = "";

static bool parseRGB(String texto, uint32_t &resultado) {
  int coma1 = texto.indexOf(',');
  int coma2 = texto.indexOf(',', coma1 + 1);

  if (coma1 < 0 || coma2 < 0) return false;

  String sr = texto.substring(0, coma1);
  String sg = texto.substring(coma1 + 1, coma2);
  String sb = texto.substring(coma2 + 1);

  sr.trim();
  sg.trim();
  sb.trim();

  if (sr.length() == 0 || sg.length() == 0 ||
      sb.length() == 0) {
    return false;
  }

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

void iniciarControlSerial() {
  Serial.begin(115200);
  Serial.println("Sistema iniciado");
}

void leerSerial() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c != '\n' && c != '\r') {
      comandoSerial += c;
      continue;
    }

    comandoSerial.trim();
    comandoSerial.toLowerCase();

    if (comandoSerial.length() == 0) {
      comandoSerial = "";
      continue;
    }

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
      seleccionarEfecto(0);
      Serial.println("Efecto seleccionado: arcoiris");
    }
    else if (efecto == "rebote" && separador < 0) {
      seleccionarEfecto(1);
      Serial.println("Efecto seleccionado: rebote");
    }
    else if (efecto == "palpitacion" && separador >= 0) {

        uint32_t nuevoColor;

        if (parseRGB(parametros, nuevoColor)) {
        establecerColorPrincipal(nuevoColor);
        seleccionarEfecto(2);

        Serial.print("Palpitacion RGB: ");
        Serial.print(obtenerColorR());
        Serial.print(",");
        Serial.print(obtenerColorG());
        Serial.print(",");
        Serial.println(obtenerColorB());
        } else {
        Serial.println("RGB invalido. Usa valores entre 0 y 255");
        }
    }
    else if ((efecto == "scanner" || efecto == "scannerd")
             && separador >= 0) {

      int separadorBG = parametros.indexOf(":bg:");

      if (separadorBG >= 0) {
        String textoScanner = parametros.substring(0, separadorBG);
        String textoFondo = parametros.substring(separadorBG + 4);

        uint32_t nuevoScanner;
        uint32_t nuevoFondo;

        if (parseRGB(textoScanner, nuevoScanner) &&
            parseRGB(textoFondo, nuevoFondo)) {

          establecerColores(nuevoScanner, nuevoFondo);

          if (efecto == "scanner") {
            seleccionarEfecto(3);
            Serial.println("Scanner simple seleccionado");
          } else {
            seleccionarEfecto(4);
            Serial.println("Scanner doble seleccionado");
          }

          Serial.print("Color scanner: 0x");
          Serial.println(obtenerColorPrincipalRGB(), HEX);

          Serial.print("Color fondo: 0x");
          Serial.println(obtenerColorFondoRGB(), HEX);
        } else {
          Serial.println("RGB invalido. Valores entre 0 y 255");
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
}
