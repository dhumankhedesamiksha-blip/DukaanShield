#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <PZEM004Tv30.h>


// =====================================================
// OLED
// =====================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// =====================================================
// PZEM
// =====================================================

#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17

PZEM004Tv30 pzem(Serial2, PZEM_RX_PIN, PZEM_TX_PIN);


// =====================================================
// TEMPERATURE SENSOR - DS18B20
// =====================================================

#define TEMP_PIN 4

OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);


// =====================================================
// RELAY
// =====================================================

#define RELAY_PIN 26

// Most relay modules are ACTIVE LOW.
// If your relay works opposite, change LOW to HIGH.
#define RELAY_ON  LOW
#define RELAY_OFF HIGH


// =====================================================
// LEDs
// =====================================================

#define GREEN_LED  25
#define YELLOW_LED 27
#define RED_LED    32
#define BLUE_LED   33


// =====================================================
// BUZZER
// =====================================================

#define BUZZER_PIN 23


// =====================================================
// SAFETY LIMITS
// Change these according to your actual project/load.
// These are demonstration values.
// =====================================================

float CURRENT_LIMIT = 5.0;      // Ampere
float POWER_LIMIT   = 1000.0;   // Watt

float TEMP_WARNING  = 40.0;     // °C
float TEMP_DANGER   = 50.0;     // °C


// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);

  // ---------- OLED ----------
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED not found!");
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(15, 10);
  display.println("Dukaan");
  display.setCursor(20, 35);
  display.println("Shield");

  display.display();

  delay(2000);


  // ---------- Temperature ----------
  sensors.begin();


  // ---------- Relay ----------
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);


  // ---------- LEDs ----------
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);


  // ---------- Buzzer ----------
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);


  Serial.println("================================");
  Serial.println("     DUKAANSHIELD STARTED");
  Serial.println("================================");
}


// =====================================================
// LOOP
// =====================================================

void loop()
{

  // ===================================================
  // READ PZEM
  // ===================================================

  float voltage = pzem.voltage();
  float current = pzem.current();
  float power   = pzem.power();
  float energy  = pzem.energy();
  float freq    = pzem.frequency();
  float pf      = pzem.pf();


  // ===================================================
  // READ TEMPERATURE
  // ===================================================

  sensors.requestTemperatures();

  float temperature = sensors.getTempCByIndex(0);


  // ===================================================
  // CHECK SENSOR ERRORS
  // ===================================================

  if (isnan(voltage))
  {
    voltage = 0;
  }

  if (isnan(current))
  {
    current = 0;
  }

  if (isnan(power))
  {
    power = 0;
  }

  if (isnan(energy))
  {
    energy = 0;
  }

  if (isnan(freq))
  {
    freq = 0;
  }

  if (isnan(pf))
  {
    pf = 0;
  }


  if (temperature == DEVICE_DISCONNECTED_C)
  {
    temperature = 0;
  }


  // ===================================================
  // SAFETY CONDITIONS
  // ===================================================

  bool currentWarning = current >= CURRENT_LIMIT;
  bool powerWarning   = power >= POWER_LIMIT;

  bool tempWarning = temperature >= TEMP_WARNING;
  bool tempDanger  = temperature >= TEMP_DANGER;


  // ===================================================
  // FAN CONTROL
  // ===================================================

  // If temperature becomes high,
  // automatically turn ON exhaust fan.

  if (tempWarning)
  {
    digitalWrite(RELAY_PIN, RELAY_ON);
    digitalWrite(BLUE_LED, HIGH);
  }
  else
  {
    digitalWrite(RELAY_PIN, RELAY_OFF);
    digitalWrite(BLUE_LED, LOW);
  }


  // ===================================================
  // LED STATUS
  // ===================================================

  // Turn all status LEDs OFF first

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);


  // ---------- DANGER ----------
  
  if (tempDanger || currentWarning || powerWarning)
  {
    digitalWrite(RED_LED, HIGH);

    // Buzzer ON
    digitalWrite(BUZZER_PIN, HIGH);
  }


  // ---------- WARNING ----------

  else if (tempWarning)
  {
    digitalWrite(YELLOW_LED, HIGH);

    // Buzzer OFF
    digitalWrite(BUZZER_PIN, LOW);
  }


  // ---------- NORMAL ----------

  else
  {
    digitalWrite(GREEN_LED, HIGH);

    // Buzzer OFF
    digitalWrite(BUZZER_PIN, LOW);
  }


  // ===================================================
  // OLED DISPLAY
  // ===================================================

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);


  // Voltage
  display.setTextSize(1);
  display.setCursor(0, 0);

  display.print("V:");
  display.print(voltage, 1);
  display.print("V");


  // Current
  display.setCursor(65, 0);

  display.print("I:");
  display.print(current, 2);
  display.print("A");


  // Power
  display.setCursor(0, 12);

  display.print("Power:");
  display.print(power, 1);
  display.print("W");


  // Temperature
  display.setCursor(0, 24);

  display.print("Temp:");
  display.print(temperature, 1);
  display.print("C");


  // Frequency
  display.setCursor(65, 24);

  display.print("F:");
  display.print(freq, 1);
  display.print("Hz");


  // Power factor
  display.setCursor(0, 36);

  display.print("PF:");
  display.print(pf, 2);


  // Fan status
  display.setCursor(65, 36);

  display.print("Fan:");

  if (tempWarning)
  {
    display.print("ON");
  }
  else
  {
    display.print("OFF");
  }


  // System status
  display.setCursor(0, 50);

  display.print("Status:");

  if (tempDanger || currentWarning || powerWarning)
  {
    display.print("DANGER");
  }

  else if (tempWarning)
  {
    display.print("WARNING");
  }

  else
  {
    display.print("NORMAL");
  }


  display.display();


  // ===================================================
  // SERIAL MONITOR
  // ===================================================

  Serial.println("--------------------------------");

  Serial.print("Voltage     : ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.print("Current     : ");
  Serial.print(current);
  Serial.println(" A");

  Serial.print("Power       : ");
  Serial.print(power);
  Serial.println(" W");

  Serial.print("Energy      : ");
  Serial.print(energy);
  Serial.println(" Wh");

  Serial.print("Frequency   : ");
  Serial.print(freq);
  Serial.println(" Hz");

  Serial.print("Power Factor: ");
  Serial.println(pf);

  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println(" C");


  if (tempDanger || currentWarning || powerWarning)
  {
    Serial.println("STATUS      : DANGER");
  }

  else if (tempWarning)
  {
    Serial.println("STATUS      : WARNING");
  }

  else
  {
    Serial.println("STATUS      : NORMAL");
  }


  delay(1000);
}