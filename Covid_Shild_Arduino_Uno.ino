float force;
unsigned int poids;
#include <SPI.h>
#include <MFRC522.h>
#define sensor A1
#include <Servo.h>
Servo myservo;
#define RST_PIN 8
#define SS_PIN 10
#include <SoftwareSerial.h>// nouvelle liaison série ici BT
MFRC522 mfrc522(SS_PIN, RST_PIN);
SoftwareSerial HC05(2,3);
const int BATTERYPIN = A3; //pin de la batterie
const float TensionMin = 4; //tension min
const float TensionMax = 5; //tension max

void setup() {
    pinMode(A0, INPUT);
    pinMode(6, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    myservo.attach(9);
    HC05.begin(9600);
    Serial.begin(9600);
    while (!Serial);
    SPI.begin();
    mfrc522.PCD_Init();
    mfrc522.PCD_DumpVersionToSerial();      // Affichage des données de la bibliothèque
    Serial.println(F("Scan PICC to see UID, type, and data blocks..."));
    myservo.write(0);
}

int getBattery ()
 {float b = analogRead(BATTERYPIN);
  int minValue = (1023 * TensionMin) / 5;
  int maxValue = (1023 * TensionMax) / 5;

  b = ((b - minValue) / (maxValue - minValue)) * 100; //mettre en pourcentage

  if (b > 100) //max is 100%
    b = 100;
  else if (b < 0) //min is 0%
    b = 0;
  int valeur = b;
  return b;}

void loop() {
  force = analogRead(A0);
  poids = 100*force/212;
  delay(10);
  float volts = analogRead(sensor)*0.0048828125;
  int distance = 13*pow(volts, -1);
  Serial.println(distance);
  Serial.println("poids =" + String () +" %");
  Serial.println("Charge = " + String (getBattery()) + " %");
  Serial.println("Tout le monde se met du gel");
  HC05.print(poids);//mettre en poucentage
  HC05.print("x");
  HC05.print(getBattery());
  HC05.print("x");
  HC05.print("aucun");
  delay(1000);


  if (poids>50)
   {digitalWrite(6, HIGH); 
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
  else if (poids>25)
   {digitalWrite(4, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
  }
  else if (poids>5)
   {digitalWrite(5, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(6, LOW);
   }   
  else if (poids<=5)
   {digitalWrite(4, LOW);
    digitalWrite(6, LOW);
    digitalWrite(5, HIGH);
    delay(250);
    digitalWrite(5, LOW);
    delay(250);
  }
 
 if ( ! mfrc522.PICC_IsNewCardPresent()) {
  return; // Attente d'une carte RFID
 }
 if ( ! mfrc522.PICC_ReadCardSerial()) {
   return; // Récupération des informations de la carte RFID
 }
 
  Serial.print("UID tag :"); // Affichage des informations de la carte RFID ( UID TAG )
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
    {Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));}
     
  Serial.println();
  content.toUpperCase();
  
  if (content.substring(1) == "56 1A 25 72")// mettre le UID de la carte souhaitée
   {delay(500);
    int distance = 13*pow(volts, -1);
    delay(500);
     if(distance >= 13)
      {Serial.println("AODREN n'a pas mis de gel");
       HC05.print(poids);//mettre en poucentage
       HC05.print("x");
       HC05.print(getBattery());
       HC05.print("x");
       HC05.print("AODREN");
       HC05.print("x");}
     else if(distance <13) 
      {myservo.write(30);
       HC05.print(poids);//mettre en poucentage
       HC05.print("x");
       HC05.print(getBattery());
       HC05.print("x");
       HC05.print("aucun");
       delay(2000);
       myservo.write(0);}
}


         
  if (content.substring(1) == "76 43 20 72")// mettre le UID de la carte souhaitée
   {delay(500);
    int distance = 13*pow(volts, -1);
    delay(500);
     if(distance >= 13)
      {Serial.println("Lilian n'a pas mis de gel");
       HC05.print(poids);//mettre en poucentage
       HC05.print("x");
       HC05.print(getBattery());
       HC05.print("x");
       HC05.print("LILIAN");
       HC05.print("x");}
     else if(distance <13) 
      {myservo.write(30);
       HC05.print(poids);//mettre en poucentage
       HC05.print("x");
       HC05.print(getBattery());
       HC05.print("x");
       HC05.print("aucun");
       delay(2000);
       myservo.write(0);}
   }
}
