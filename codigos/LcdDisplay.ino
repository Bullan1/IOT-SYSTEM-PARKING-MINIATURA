#include <LiquidCrystal.h>

// Inicializar el objeto LiquidCrystal
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

void setup() {
  // Inicializar el LCD con 16 columnas y 2 filas
  lcd.begin(10, 2);
}

void loop() {
  // Establecer el cursor en la posición (0, 0) (primera columna, primera fila)
  lcd.setCursor(0, 0);
  
  // Imprimir el mensaje "Hola, han pasado" en la primera fila del LCD
  lcd.print("Hola");
  
  // Establecer el cursor en la posición (0, 1) (primera columna, segunda fila)
  lcd.setCursor(0, 1);
  
  // Imprimir el tiempo transcurrido en segundos utilizando la función millis()
  lcd.print(millis() / 1000);
  
  // Imprimir el texto "seg." después del tiempo transcurrido
  lcd.print(" seg.");
}