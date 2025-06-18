#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h>

int RST_PIN = 9; 
int SS_PIN = 10; 
int servoPin = 8; 

Servo motor; 
MFRC522 rfid(SS_PIN, RST_PIN); 
byte ID[4] = { 205, 17, 0, 50};
byte ID2[4] = { 102 , 7 , 188 , 34 }; //Yetkili kart ID'sini tanımlıyoruz. okul kartı

void setup() {
  motor.attach(servoPin); 
  Serial.begin(9600); 
  SPI.begin(); 
  rfid.PCD_Init(); 
}

void loop() {

  if (!rfid.PICC_IsNewCardPresent()) //Yeni kartın okunmasını bekliyoruz.
    return;

  if (!rfid.PICC_ReadCardSerial()) //Kart okunmadığı zaman bekliyoruz.
    return;

  if (rfid.uid.uidByte[0] == ID[0] &&  rfid.uid.uidByte[1] == ID[1] &&  rfid.uid.uidByte[2] == ID[2] &&  rfid.uid.uidByte[3] == ID[3])//Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
   {
    Serial.println("Kapi acildi");
    ekranaYazdir();
    motor.write(180); 
    delay(3000);
    motor.write(0); 
    delay(1000);
  } 
  else if (rfid.uid.uidByte[0] == ID2[0] &&  rfid.uid.uidByte[1] == ID2[1] &&  rfid.uid.uidByte[2] == ID2[2] &&  rfid.uid.uidByte[3] == ID2[3])//Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
   {
    Serial.println("Kapi acildi");
    ekranaYazdir();
    motor.write(180); //Servo motoru 180 dereceye getiriyoruz.
    delay(3000);
    motor.write(0); //Servo motoru 0 dereceye getiriyoruz.
    delay(1000);
   }
  else 
  { //Yetkisiz girişte içerideki komutlar çalıştırılır.
    Serial.println("Yetkisiz Kart");
    ekranaYazdir();
  }
  rfid.PICC_HaltA();
}
void ekranaYazdir() {
  Serial.print("ID Numarasi: ");
  for (int sayac = 0; sayac < 4; sayac++) {
    Serial.print(rfid.uid.uidByte[sayac]);
    Serial.print(" ");
  }
  Serial.println("");
}
