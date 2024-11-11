 // Created by Simple Circuits 

#include <Wire.h> 

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);   

#include <Servo.h> 



Servo myservo;



int IR1 = 2;

int IR2 = 3;



int Slot = 4;           //Total number of parking Slots



int flag1 = 0;

int flag2 = 0;



void setup() {

  Serial.begin(9600); 

    lcd.init(); //initialize the lcd

    lcd.backlight(); //open the backlight

 

 

pinMode(IR1, INPUT);

pinMode(IR2, INPUT);

  

myservo.attach(4);

myservo.write(100);



lcd.setCursor (0,0);

lcd.print("     DJ    ");

lcd.setCursor (0,1);

lcd.print(" PARKING IOT ");

delay (2000);

lcd.clear();  

}



void loop(){ 



if(digitalRead (IR1) == LOW && flag1==0){

if(Slot>0){flag1=1;

if(flag2==0){myservo.write(0); Slot = Slot-1;}

}else{

lcd.setCursor (0,0);

lcd.print("    Estacionamientos    ");  

lcd.setCursor (0,1);

lcd.print("  Llenos  "); 

delay (3000);

lcd.clear(); 

}

}



if(digitalRead (IR2) == LOW && flag2==0){flag2=1;

if(flag1==0){myservo.write(0); Slot = Slot+1;}

}



if(flag1==1 && flag2==1){

delay (1000);

myservo.write(100);

flag1=0, flag2=0;

}



lcd.setCursor (0,0);

lcd.print("    BIENVENIDO!    ");

lcd.setCursor (0,1);

lcd.print("Disponibles: ");

lcd.print(Slot);

}
