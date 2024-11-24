#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Configuración de LCD y Servo
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;

// Pines del ESP8266
int IR1 = 12;  // Entrada
int IR2 = 13;  // Salida
int servo= 14; // Servo para la barrera
int Slot = 4; // Total de espacios disponibles

// Configuración WiFi y MQTT
const char* ssid = "danielr";
const char* password = "polo978*";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.print("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Conectado a WiFi");
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266_ParkingClient")) {
      client.subscribe("estacionamiento/status");
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  myservo.attach(servo);
  myservo.write(100);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  
  // Pantalla de bienvenida
  lcd.setCursor(0, 0);
  lcd.print("   Estacionamiento  ");
  lcd.setCursor(0, 1);
  lcd.print("   IOT Sistema     ");
  delay(2000);
  lcd.clear();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (digitalRead(IR1) == LOW) {
    if (Slot > 0) {
      Slot--;
      myservo.write(0);  // Abre la barrera
      client.publish("estacionamiento/entrada", String(Slot).c_str()); // Publica el nuevo estado
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Estacionamientos");
      lcd.setCursor(0, 1);
      lcd.print(" Llenos");
      delay(3000);
      lcd.clear();
    }
  }

  if (digitalRead(IR2) == LOW) {
    Slot++;
    myservo.write(0); // Abre la barrera
    client.publish("estacionamiento/salida", String(Slot).c_str()); // Publica el nuevo estado
  }

  // Mostrar el número de espacios disponibles en el LCD
  lcd.setCursor(0, 0);
  lcd.print("Disponible: ");
  lcd.print(Slot);
  delay(1000);
}
