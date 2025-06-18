Kullanılan Kütüphaneler ve Donanım Tanımları

#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h>
Açıklama:
Servo.h: Servo motoru kontrol etmek için kullanılan kütüphane.

MFRC522.h: RFID modülü (RC522) ile iletişim kurmak için gerekli olan kütüphane.

SPI.h: RC522 modülü SPI haberleşme protokolü ile çalışır. Bu kütüphane, Arduino ile SPI üzerinden haberleşmeyi sağlar.


int RST_PIN = 9;
int SS_PIN = 10;
int servoPin = 8;
Açıklama:
RST_PIN (9): RC522 modülünün reset pinine bağlanan Arduino pini.

SS_PIN (10): RC522'nin "Slave Select" pini (SPI’de hangi cihazla iletişim kurulacağını belirtir).

servoPin (8): Servo motorun sinyal kablosunun bağlandığı pin.


Servo motor;
MFRC522 rfid(SS_PIN, RST_PIN);
byte ID[4] = { 205, 17, 0, 50 };
byte ID2[4] = { 102 , 7 , 188 , 34 };
Açıklama:
motor: Servo motor kontrolü için oluşturulan nesne.

rfid: RC522 kart okuyucu nesnesi. SS ve RST pinleri tanımlanıyor.

ID ve ID2: Yetkili kartların UID numaraları. Bu UID'ler tanımlı kartlara aittir. Kart okunduğunda bu sayılarla karşılaştırılır.

 setup() Fonksiyonu

void setup() {
  motor.attach(servoPin); 
  Serial.begin(9600);
  SPI.begin(); 
  rfid.PCD_Init(); 
}

Açıklama:
motor.attach(servoPin): Servo motor nesnesini 8. pine bağlıyor.

Serial.begin(9600): Seri haberleşmeyi başlatıyor (9600 baud). Debug veya monitörleme için.

SPI.begin(): SPI haberleşmesini başlatıyor.

rfid.PCD_Init(): RC522 RFID okuyucuyu başlatıyor.

 loop() Fonksiyonu
Kod burada sürekli çalışır ve RFID kartların okunmasını bekler.


if (!rfid.PICC_IsNewCardPresent()) 
  return;

if (!rfid.PICC_ReadCardSerial()) 
  return;
Açıklama:
PICC_IsNewCardPresent(): Yeni bir kart okuyucuya tutulmuş mu diye kontrol eder.

PICC_ReadCardSerial(): Kart tanındıysa seri numarasını okur. Her ikisi de true değilse return ile işlem yapılmaz ve loop yeniden başlar.

 Kart Kimlik Doğrulama ve Kapı Açma

if (rfid.uid.uidByte[0] == ID[0] &&  rfid.uid.uidByte[1] == ID[1] &&  rfid.uid.uidByte[2] == ID[2] &&  rfid.uid.uidByte[3] == ID[3])
veya


else if (rfid.uid.uidByte[0] == ID2[0] &&  rfid.uid.uidByte[1] == ID2[1] &&  rfid.uid.uidByte[2] == ID2[2] &&  rfid.uid.uidByte[3] == ID2[3])
Açıklama:
Okunan kartın UID değeriyle ID veya ID2 karşılaştırılır.

Birebir uyuşuyorsa sistem o kartın yetkili olduğunu anlar.

UID, her RFID kartın eşsiz kimliğidir ve uidByte[] dizisi ile 4 baytlık kimlik elde edilir.

 Servo Motor ile Kapı Açma

Serial.println("Kapi acildi");
ekranaYazdir();
motor.write(180); 
delay(3000);
motor.write(0); 
delay(1000);
Açıklama:
Eşleşen kart varsa:

"Kapi acildi" mesajı seri ekrana yazdırılır.

ekranaYazdir() fonksiyonu çağrılır (kart UID bilgisi gösterilir).

Servo motor 180 derece döner → Kapı açılır.

3 saniye beklenir.

Servo 0 dereceye döner → Kapı kapanır.

 Yetkisiz Kart Kontrolü

else 
{ 
  Serial.println("Yetkisiz Kart");
  ekranaYazdir();
}
Açıklama:

Okunan kart ID veya ID2 ile eşleşmiyorsa:

"Yetkisiz Kart" mesajı yazılır.

Kartın UID bilgisi yine ekrana yazdırılır.

 Kartı Hafızadan Silme (Protokol gereği)

rfid.PICC_HaltA();

Açıklama:
Kartın işleminden sonra RC522 modülü ile olan iletişim kapatılır.

 ekranaYazdir() Fonksiyonu

void ekranaYazdir() {
  Serial.print("ID Numarasi: ");
  for (int sayac = 0; sayac < 4; sayac++) {
    Serial.print(rfid.uid.uidByte[sayac]);
    Serial.print(" ");
  }
  Serial.println("");
}

Açıklama:
Kartın UID (kimlik) numarası 4 baytlık olarak ekrana yazdırılır.

Bu değerler kullanılarak yeni kart tanımlamaları yapılabilir.

Örnek çıktı: ID Numarasi: 205 17 0 50

 Genel Sistem Akışı:
Sistem sürekli olarak yeni bir kartı bekler.

Kart okutulunca, UID alınır.

UID kontrol edilir:

Tanımlıysa kapı açılır, 3 saniye beklenir, sonra kapanır.

Tanımsızsa "Yetkisiz Kart" mesajı verilir.

Kartla iletişim durdurulur.

Süreç baştan başlar.

 Kullanılabilecek Donanım Listesi:
Arduino UNO veya Nano

RC522 RFID Modülü

RFID Kart veya Anahtarlıklar

Servo Motor SG90 

Jumper Kablolar

3.3V güç kaynağı RC522 için 

Harici güç kaynağı Servo'nun stabil çalışması için 
