/*********************************************************************
LiPoSolder
by Tobias Mädel (t.maedel@alfeld.de)
http://tbspace.de

* 02.01.2016 - Written initial version
* 13.02.2016 - fixed thermocouple conversion
* 14.02.2016 - added EEPROM saving
* 14.02.2016 - splitted project into files

 -- TODO: 
 Graph-display of temperature history
*********************************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PID_v1.h>
#include <avr/wdt.h>
#include <max6675.h>
#include <EEPROM.h>
#include <EnableInterrupt.h>
#include <TimerOne.h>

#define LEFT_BUTTON		7
#define MIDDLE_BUTTON	6
#define RIGHT_BUTTON	5

#define HEATER_PIN		9

#define BATTERY_PIN		A0

#define THERMO_DO		10
#define THERMO_CS		11
#define THERMO_CLK		12

#define USING_WRONG_THERMOCOUPLE 1

#define SERIAL_BAUDRATE 115200

#define IS_NAN(x) (((x) < 0) == ((x) >= 0))

double setTemperature 		= 0;
double currentTemperature 	= 25;
double outputDutyCycle 		= 0;

float  batteryVoltage 		= 0;

unsigned long lastThermocoupleConversion = 0;

// PID Parameters
double Kp=2, Ki=5, Kd=1;

volatile uint8_t currentPage = 0;

volatile uint8_t leftButtonPressed = 255;
volatile uint8_t middleButtonPressed = 255;
volatile uint8_t rightButtonPressed = 255;

// sizeof(pages) / sizeof(void*)
void statusPage();
void presetPage();
void statusPageButton();
void presetPageButton();

void (*pagesBtnISR[])(void) = { statusPageButton, presetPageButton };
void (*pages[])(void) = { statusPage, presetPage };

void leftButtonChanged();
void middleButtonChanged();
void rightButtonChanged();

void buttonRepeatISR();

PID heaterPID(&currentTemperature, &outputDutyCycle, &setTemperature, Kp, Ki, Kd, DIRECT);
MAX6675 thermocouple(THERMO_CLK, THERMO_CS, THERMO_DO);
Adafruit_SSD1306 display(4);

void setup()
{
	wdt_reset();
	wdt_disable();

	// Debug: Initialize serial console
	Serial.begin(SERIAL_BAUDRATE);

	// Configure header output - n-channel MOSFET, needs to be pulled low
	pinMode(HEATER_PIN, OUTPUT);
	digitalWrite(HEATER_PIN, LOW);

	// Configure push buttons and appropriate pull-ups
	pinMode(LEFT_BUTTON, INPUT);
	digitalWrite(LEFT_BUTTON, HIGH);
	pinMode(MIDDLE_BUTTON, INPUT);
	digitalWrite(MIDDLE_BUTTON, HIGH);
	pinMode(RIGHT_BUTTON, INPUT);
	digitalWrite(RIGHT_BUTTON, HIGH);

	// Change ADC voltage reference to 1.1V to get a stable reading from 
	// the voltage divider (to measure the battery voltage)
	analogReference(INTERNAL);
	
	// initialize the display with internal contrast voltage generation
	// warning: 0x3c is specific to some types of chinese OLED displays
	// (usually 128x64)
	display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  

	// read initial temperature
	setTemperature = (double)(EEPROM.read(1) | uint16_t(EEPROM.read(2)) << 8);

	// Show boot-up-screen
	renderLogo();

	// for a given time. (1 second)
	delay(1000);

	wdt_enable(WDTO_8S);
	wdt_reset();

	enableInterrupt(LEFT_BUTTON, leftButtonChanged, CHANGE);
	enableInterrupt(MIDDLE_BUTTON, middleButtonChanged, CHANGE);
	enableInterrupt(RIGHT_BUTTON, rightButtonChanged, CHANGE);

	Timer1.initialize(100000);
	Timer1.attachInterrupt( buttonRepeatISR );

	// enable PID controller (enables heating element!)
	heaterPID.SetMode(AUTOMATIC);

	wdt_reset();
}

void loop()
{	
	// read current thermocouple temperature
	// only sample every 250 ms to avoid stopping running ADC conversions
	if (millis() - lastThermocoupleConversion > 250)
	{
		double typeKtemp = thermocouple.readCelsius();

		if (IS_NAN(typeKtemp))
		{
			tripThermocoupleAlert();
		}

		currentTemperature = typeKtemp;

		// convert max6675K readings to correct temperatures
		#ifdef USING_WRONG_THERMOCOUPLE 
		currentTemperature = TypeEVoltageToTemperature(TypeKTemperatureToVoltage(typeKtemp));
		#endif

		//Serial.println(currentTemperature);
		lastThermocoupleConversion = millis();
	}

	// calculate PID offset and set output dutycycle 
	// to appropriate value
	heaterPID.Compute();
	analogWrite(HEATER_PIN, outputDutyCycle);
	wdt_reset();

	batteryVoltage = getBatteryVoltage();
	if (batteryVoltage < 8)
	{
		tripLowVoltage();
	}

	pages[currentPage]();
}


