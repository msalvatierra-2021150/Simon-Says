#include <Adafruit_NeoPixel.h>

#define PIN 25  
#define NUMPIXELS 3
#define DELAYVAL 500

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int patronJuego[5];          
int patronPresionado[5];          
int pasoActual = 0;          
int nivelActual = 1;         
bool juegoIniciado = false;     

void setup() {
  pinMode(12, INPUT_PULLUP);  
  pinMode(26, INPUT_PULLUP); 
  pinMode(35, INPUT_PULLUP); 
  Serial.begin(9600);
  pixels.begin();
  pixels.show();  
  randomSeed(analogRead(0));  
}

void loop() {
  if (!juegoIniciado) {
    iniciarJuego();
  } else {
    // Lectura de los inputs del usuario
    if (digitalRead(12) == LOW) {  // Botón 1 presionado
      Serial.println("Uno");
      patronPresionado[pasoActual] = 0;
      pasoActual++;
      delay(300); 
    }
    if (digitalRead(26) == LOW) {  // Botón 2 presionado
      Serial.println("Dos");
      patronPresionado[pasoActual] = 1;
      pasoActual++;
      delay(300);
    }
    if (digitalRead(35) == LOW) {  // Botón 3 presionado
      Serial.println("Tres");
      patronPresionado[pasoActual] = 2;
      pasoActual++;
      delay(300);
    }

    // Chequear si el usuario ya presiono el número correcto de veces para el nivel actual
    if (pasoActual == nivelActual) {
      if (revisarPatron()) {
        Serial.println("Correcto!");
        displayCorrecto();
        if (nivelActual < 5) {
          nivelActual++;
          pasoActual = 0;
          delay(1000);
          mostrarSequencia();
        } else {
          Serial.println("GANASTE!");
          reseteoDelJuego();
        }
      } else {
        Serial.println("IncorrectO");
        displayIncorrecto();
        reseteoDelJuego();
      }
    }
  }
}

void iniciarJuego() {
  // Generatcion del arreglo general
  for (int i = 0; i < 5; i++) {
    patronJuego[i] = random(0, 3);  // Número random de 0 a 2
  }

  mostrarSequencia();
  juegoIniciado = true;
  pasoActual = 0;
}

void mostrarSequencia() {
  for (int i = 0; i < nivelActual; i++) {
    displayLEDS(patronJuego[i]);
    delay(DELAYVAL);
  }
}

void displayLEDS(int pattern) {
  pixels.clear();
  if (pattern == 0) {
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));  // Red
  } else if (pattern == 1) {
    pixels.setPixelColor(1, pixels.Color(0, 255, 0));  // Green
  } else if (pattern == 2) {
    pixels.setPixelColor(2, pixels.Color(0, 0, 255));  // Blue
  }
  pixels.show();
  delay(DELAYVAL);
  pixels.clear();
  pixels.show();
}

bool revisarPatron() {
  for (int i = 0; i < nivelActual; i++) {
    if (patronPresionado[i] != patronJuego[i]) {
      return false;
    }
  }
  return true;
}

void displayCorrecto() {
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.setPixelColor(1, pixels.Color(0, 255, 0));
  pixels.setPixelColor(2, pixels.Color(0, 255, 0));
  pixels.show();
  delay(1000);
  pixels.clear();
  pixels.show();
}

void displayIncorrecto() {
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.setPixelColor(1, pixels.Color(255, 0, 0));
  pixels.setPixelColor(2, pixels.Color(255, 0, 0));
  pixels.show();
  delay(1000);
  pixels.clear();
  pixels.show();
}

void reseteoDelJuego() {
  juegoIniciado = false;
  nivelActual = 1;
  pasoActual = 0;
  delay(2000);
}
