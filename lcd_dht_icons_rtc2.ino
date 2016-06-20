#include <Time.h>
#include <TimeLib.h>

// Einbinden des DHT22
#include "DHT.h"                            // Library für DHT Sensor aufgerufen
#define DHTPIN 9                            // DHT Sensor wird aus PIN 9 ausgelesen
#define DHTTYPE DHT22                       // DHT Type wird auf DHT22 festgelegt
DHT dht22(DHTPIN, DHTTYPE);                 // DHTPIN und DHTTYE in dht22 definiert
#define DHTPIN2 10
#define DHTTYPE2 DHT22
DHT dht222(DHTPIN2, DHTTYPE2);

// Einbinden des LCD
#include "Wire.h"                           // Library für I2C aufgerufen
#include "LiquidCrystal_I2C.h"              // Library für LCD aufgerufen
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);         // LCD Adresse auf 0x27 gesetzt, 16 Zeichen / 2 Zeilen LCD

#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};

// Time Variablen

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

byte thermometer[8] = //icon für thermometer
{
    B00100,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110
};

byte tropfen[8] = //icon für tropfen
{
    B00100,
    B00100,
    B01010,
    B01010,
    B10001,
    B10001,
    B10001,
    B01110,
};




void setup()
{
  Serial.begin(9600);                         // Serielle Ausgabe beginnen mit 9600baud
  Serial.println("DHT22 Serielle Ausgabe");   // Begrüßung beim Aufruf vom seriellen Monitor
  dht22.begin();                              // Auslesen des DHT22 beginnt
  dht222.begin();                             // Auslesen des DHT22 2 beginnt
  //----lcd.init();                           // Initialisierung des LCD                                            
  lcd.begin(20,4);                            // initialize the lcd for 20 chars 4 lines, turn on backlight
  lcd.backlight();                            // LCD Hintergrundbeleuchtung aktivieren
  lcd.createChar(1,thermometer);
  lcd.createChar(2,tropfen);

  //--------RTC SETUP ------------
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }



}
void loop()
{
  float t1 = dht22.readTemperature();        // Die Temperatur wird vom DHT22 ausgelesen, Wert in "t" schreiben
  float h1 = dht22.readHumidity();           // Die Luftfeuchte wird vom DHT22 ausgelesen, Wert in "h" schreiben
  float t2 = dht222.readTemperature();        // Die Temperatur wird vom DHT22 ausgelesen, Wert in "t" schreiben
  float h2 = dht222.readHumidity();           // Die Luftfeuchte wird vom DHT22 ausgelesen, Wert in "h" schreiben
 // int h = now.hour();
 // int m = now.minute();
 // int s = now.second();
  
  Serial.print("Temperatur: ");             // Ausgabe im Seriellen Monitor
  Serial.print(t1);                          // Ausgabe im Seriellen Monitor
  Serial.print(" C\t");                     // Ausgabe im Seriellen Monitor  
  Serial.print("Luftfeuchte: ");            // Ausgabe im Seriellen Monitor
  Serial.print(h1);                          // Ausgabe im Seriellen Monitor
  Serial.println(" %");                     // Ausgabe im Seriellen Monitor

  Serial.print("Temperatur: ");             // Ausgabe im Seriellen Monitor
  Serial.print(t2);                          // Ausgabe im Seriellen Monitor
  Serial.print(" C\t");                     // Ausgabe im Seriellen Monitor  
  Serial.print("Luftfeuchte: ");            // Ausgabe im Seriellen Monitor
  Serial.print(h2);                          // Ausgabe im Seriellen Monitor
  Serial.println(" %");                     // Ausgabe im Seriellen Monitor

// RTC STuff

    DateTime now = rtc.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    
    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));
    
    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
    
    Serial.println();

    
  //  RtcTemperature temp = Rtc.GetTemperature();
  //  Serial.print(temp.AsFloat());
  //  Serial.println("C");
    

    delay(10000); // ten seconds

// RTC Stuff end


  
// Uhrzeit Zeile 0


  lcd.setCursor(0, 0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.setCursor(2, 0);
  lcd.print(".");
  lcd.setCursor(3, 0);
  if (now.day() <10){
    lcd.print("0");
  }
  lcd.print(now.day());
  lcd.setCursor(5, 0);
  lcd.print(".");
  lcd.setCursor(6,0);
  if (now.month() <10){
    lcd.print("0");
  }
  lcd.print(now.month());
  lcd.setCursor(8, 0);
  lcd.print(".");
  lcd.setCursor(9, 0);
  lcd.print(now.year());
  lcd.setCursor(15, 0);
  if (now.hour() <10){
    lcd.print("0");
  }
  lcd.print(now.hour());
  lcd.setCursor(17, 0);
  lcd.print(":");
  lcd.setCursor(18, 0);
  if (now.minute() <10){
    lcd.print("0");
  }
  lcd.print(now.minute());
  
  // Temp RTC Zeile 0
//  lcd.setCursor(11, 0);
//  lcd.print(temp.AsFloat());
//  lcd.setCursor(17, 0);                        // Anfang auf Stelle 15, Zeile 0 setzen
//  lcd.print("C");

// Thermometer Temp 1
  lcd.setCursor(0, 2);                        // Thermometer auf Display
  lcd.write(1);
  lcd.setCursor(2, 2);
  lcd.print(t1);                               // Wert aus "t" ausgeben (Temperatur)
  lcd.setCursor(8, 2);                        // Anfang auf Stelle 15, Zeile 0 setzen
  lcd.print("C");       

// Feuchtigkeit 1
  lcd.setCursor(0, 3);                       // Tropfen auf Display
  lcd.write(2);
  lcd.setCursor(2, 3);
  lcd.print(h1);
  lcd.setCursor(8, 4);
  lcd.print("%");

// Thermometer Temp 2
  lcd.setCursor(10, 2);
  lcd.print(t2);                               // Wert aus "t" ausgeben (Temperatur)
  lcd.setCursor(16, 2);                        // Anfang auf Stelle 15, Zeile 0 setzen
  lcd.print("C");       

// Feuchtigkeit 2
  lcd.setCursor(10, 3);
  lcd.print(h2);
  lcd.setCursor(16, 4);
  lcd.print("%");

}

#define countof(a) (sizeof(a) / sizeof(a[0]))

