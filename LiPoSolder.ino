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

#define LEFT_BUTTON		7
#define MIDDLE_BUTTON	6
#define RIGHT_BUTTON	5

#define HEATER_PIN		9

#define BATTERY_PIN		A0

#define THERMO_DO		10
#define THERMO_CS		11
#define THERMO_CLK		12

#define USING_WRONG_THERMOCOUPLE 1

#define IS_NAN(x) (((x) < 0) == ((x) >= 0))

double setTemperature 		= 20;
double currentTemperature 	= 25;
double outputDutyCycle 		= 0;

float  batteryVoltage 		= 0;

unsigned long lastThermocoupleConversion = 0;

// PID Parameters
double Kp=2, Ki=5, Kd=1;

volatile uint8_t currentPage = 0;

volatile uint8_t leftButtonPressed = 0;
volatile uint8_t middleButtonPressed = 0;
volatile uint8_t rightButtonPressed = 0;


// sizeof(pages) / sizeof(void*)
void statusPage();
void presetPage();
void statusPageButton();
void presetPageButton();
void (*pages[])(void) = { statusPage, presetPage };
void (*pagesBtnISR[])(void) = { statusPageButton, presetPageButton };

PID heaterPID(&currentTemperature, &outputDutyCycle, &setTemperature, Kp, Ki, Kd, DIRECT);
MAX6675 thermocouple(THERMO_CLK, THERMO_CS, THERMO_DO);
Adafruit_SSD1306 display(4);

void setup()
{
	wdt_reset();
	wdt_disable();

	// Debug: Initialize serial console
	Serial.begin(9600);

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

/*	enableInterrupt(LEFT_BUTTON, leftButtonRising, RISING);
	enableInterrupt(MIDDLE_BUTTON, middleButtonRising, RISING);
	enableInterrupt(RIGHT_BUTTON, rightButtonRising, RISING);
	enableInterrupt(LEFT_BUTTON, leftButtonFalling, FALLING);
	enableInterrupt(MIDDLE_BUTTON, middleButtonFalling, FALLING);
	enableInterrupt(RIGHT_BUTTON, rightButtonFalling, FALLING);
*/
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
	// enable PID controller (enables heating element!)
	heaterPID.SetMode(AUTOMATIC);
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

	if (!digitalRead(RIGHT_BUTTON))
	{
		rightButtonPressed = 1;
	}
	if (digitalRead(RIGHT_BUTTON))
	{
		if (rightButtonPressed == 1)
		{
			rightButtonPressed = 0;
			currentPage += 1;
			if (currentPage >= sizeof(pages) / sizeof(void*))
				currentPage = 0;
		}
	}

	pages[currentPage]();
}


float getBatteryVoltage()
{
	return map(analogRead(BATTERY_PIN), 0, 970, 0, 1600) / 100.0f;

	uint16_t voltage = 0;
	
	for(int i=0; i<5; i++)
	{
	    voltage += analogRead(BATTERY_PIN);
	    delay(10);
	}

	voltage = voltage / 5;
	return map(voltage, 0, 970, 0, 1600) / 100.0f;
}

