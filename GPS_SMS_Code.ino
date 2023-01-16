#include <RobonioGSM.h>
RobonioGSM Robonio;
String smstext, smsNumber;
/* Konumun yollanacağı numarayı tanımlıyoruz*/
#define TargetNumber  "+90" 
#define LED_pin 13

/*GPS kütüphanesini ekliyoruz*/
#define GPS Serial
#include <TinyGPS.h>
TinyGPS gps;

double enl;
double boy;
String location;
char location2[255];

void setup() {
  Robonio.basla();
  delay(2000);
  pinMode(LED_pin, OUTPUT);
  GPS.begin(9600);
  delay(2000);
  Robonio.tumSmsSil();
  delay(2000);
  Robonio.smsGonder(TargetNumber, "Sistem acildi.");
}

void loop() 
/*Komutları tanımlıyoruz*/
{
  smstext = Robonio.smsOku(1);
  if (smstext.indexOf("OK") != -1) {
    if (smstext.length() > 7) {
      smsNumber = Robonio.numaraliSmsOku(1);
      smstext.toUpperCase();

      if (smstext.indexOf("LEDAC") != -1) {
        digitalWrite(LED_pin, HIGH);
        Robonio.smsGonder(TargetNumber, "LED Acildi");
      }
      else if (smstext.indexOf("LEDKAPAT") != -1) {
        digitalWrite(LED_pin, LOW);
        Robonio.smsGonder(TargetNumber, "LED Kapatildi");
      }
      else if (smstext.indexOf("NERDESIN") != -1 || smstext.indexOf("NEREDESIN") != -1 || smstext.indexOf("KONUM") != -1) {
        delay(1000);

        bool yeniVeri = false;
        for (unsigned long start = millis(); millis() - start < 1000;) {
          while (GPS.available()) {
            char c = GPS.read();
            if (gps.encode(c))
              yeniVeri = true;
          }
        }

        if (yeniVeri) {
          float flat, flon;
          unsigned long age;
          gps.f_get_position(&flat, &flon, &age);
          enl = flat;
          boy = flon;
          location = "www.google.com.tr/maps/place/" + String(enl, 6) + "," + String(boy, 6);
          location.toCharArray(location2, 100);
          delay(1000);
        }
        delay(1000);
        char location2[255];
        konum.toCharArray(konum2, 100);
        Robonio.smsGonder(TargetNumber, konum2);
      }
      else {
      }
      Robonio.tumSmsSil();
    }
  }
  }
