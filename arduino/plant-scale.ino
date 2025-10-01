// plant-scale.ino
// nicole scribner 2025-09-24

// LCD display of DHT11 temp/humidity sensor output.
// Based on Arduino and Elegoo tutorials for LCD screen and DHT11.

#include <DHT.h>
#include <LiquidCrystal.h>
#include "HX711.h"

#define DHTPIN 2
#define DHTTYPE DHT11
#define DOUTPIN 14
#define SCKPIN 15

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Initialize HX711 controller
HX711 scale(DOUTPIN, SCKPIN); //DOUT, SCK


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const char	tempmsg[] = "Temp:   ", 
						hummsg[] = 	"Hum:     ", 
						himsg[] = 	"HI:     ",
						degc[] =		"C",
						degf[] =		"F";

void setup() {
	// DHT
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();

  // LCD

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

	// SCALE
	scale.set_scale(2280.f);
	scale.tare();
}


void loop() {



	// DHT LOOP


  // Wait a few seconds between measurements.
	delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));


	


  // LCD LOOP
	// ### ### ### ###
	// Temp:   00.00CC
	// 00.00 F    Hum:
	// 00.00 C  00.00%

	// Temp:   00.00 C
	// Hum:     00.00%
	// HI: 00.00/00.00
	if(millis() % 2 == 0)
	{
		lcd.setCursor(0,0);
		lcd.print(tempmsg);
		lcd.print(t);
		lcd.print(degc);
		lcd.setCursor(0,1);
		lcd.print(hummsg);
		lcd.print(h);
		lcd.print('%');
	}
	else
	{
		lcd.setCursor(0,0);
		lcd.print(tempmsg);
		lcd.print(f);
		lcd.print(degf);
		lcd.setCursor(0,1);
		lcd.print(himsg);
		lcd.print(hif);
		lcd.print(degf);
	}

	// SCALE
	Serial.print("one reading:\t");
	Serial.print(scale.get_units(), 1);
	scale.power_down();
	delay(5000);
	scale.power_up();
}


