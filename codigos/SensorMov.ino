const int LEDPin = 13;        
const int PIRPin = 2;         
int pirState = LOW;           
int val = 0;                   
void setup() 
{
   pinMode(LEDPin, OUTPUT); 
   pinMode(PIRPin, INPUT);
   Serial.begin(9600);
}
 
void loop()
{
   val = digitalRead(PIRPin);
   if (val == HIGH)   
   { 
      digitalWrite(LEDPin, HIGH);  
      if (pirState == LOW)  
      {
        Serial.println("Sensor activado");
        pirState = HIGH;
      }
   } 
   else    
   {
   digitalWrite(LEDPin, LOW); 
   if (pirState == HIGH)  
   {
   Serial.println("Sensor parado");
  pirState = LOW;
}
}
}