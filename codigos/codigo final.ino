#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Configuración del LCD y Servo
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;

// Cambiar las definiciones de pines para el ESP8266
int IR1 = 0;  // GPIO5 (D3 en NodeMCU)
int IR2 = 2;  // GPIO4 (D4 en NodeMCU)
int Slot = 4; // Número total de espacios de estacionamiento
int flag1 = 0;
int flag2 = 0;

// Configuración de WiFi y MQTT
const char* ssid = "danielr"; // Nombre de tu red WiFi
const char* password = "polo978*"; // Contraseña de tu red WiFi

const char* mqtt_server = "test.mosquitto.org"; // Dirección de tu broker MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Función para conectar al WiFi
void setup_wifi() {
  Serial.print("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Conectado al WiFi");
}

// Función para reconectar al servidor MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conectar al servidor MQTT...");
    if (client.connect("ESP8266_ParkingClient")) {
      Serial.println("Conectado al servidor MQTT");
      client.subscribe("estacionamiento/status"); // Puedes suscribirte a algún tema si es necesario
    } else {
      Serial.print("Error de conexión, reintentando en 5 segundos...");
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
  myservo.attach(14);
  myservo.write(100);

  // Configuración de WiFi y MQTT
  setup_wifi();
  client.setServer(mqtt_server, 1883); // Puerto por defecto de MQTT es 1883
  
  // Pantalla de bienvenida
  lcd.setCursor(0, 0);
  lcd.print("     DJ    ");
  lcd.setCursor(0, 1);
  lcd.print(" PARKING IOT ");
  delay(2000);
  lcd.clear();
}

void loop() {
  if (!client.connected()) {
    reconnect(); // Reconectar si la conexión se pierde
  }
  client.loop(); // Mantener la conexión MQTT activa

  // Verificar si un vehículo entra o sale
  if (digitalRead(IR1) == LOW && flag1 == 0) {
    if (Slot > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        myservo.write(0); // Abre la barrera
        Slot = Slot - 1; // Reducir espacios disponibles
        client.publish("estacionamiento/entrada", String(Slot).c_str()); // Enviar el estado actual a MQTT
      }
    } else {
      lcd.setCursor(0, 0);
      lcd.print(" Estacionamientos ");
      lcd.setCursor(0, 1);
      lcd.print("  Llenos  ");
      delay(3000);
      lcd.clear();
    }
  }

  if (digitalRead(IR2) == LOW && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) {
      myservo.write(0); // Abre la barrera
      Slot = Slot + 1; // Incrementar espacios disponibles
      client.publish("estacionamiento/salida", String(Slot).c_str()); // Enviar el estado actual a MQTT
    }
  }

  if (flag1 == 1 && flag2 == 1) {
    delay(1000);
    myservo.write(100); // Cierra la barrera
    flag1 = 0;
    flag2 = 0;
  }

  // Mostrar el número de espacios disponibles en el LCD
  lcd.setCursor(0, 0);
  lcd.print("    BIENVENIDO!    ");
  lcd.setCursor(0, 1);
  lcd.print("Disponibles: ");
  lcd.print(Slot);
  delay(500);
}