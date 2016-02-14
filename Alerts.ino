void tripThermocoupleAlert()
{
	Timer1.detachInterrupt();
	disableInterrupt(LEFT_BUTTON);
	disableInterrupt(MIDDLE_BUTTON);
	disableInterrupt(RIGHT_BUTTON);

	// Locks processor when a thermocouple error occurs. 
	display.clearDisplay();
	display.setCursor(0, 0);
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.println(F("LiPoSolder"));
	
	display.setCursor(0, 20);
	display.setTextSize(1);
	display.println(F("No thermocouple"));

	display.setCursor(0, 30);
	display.setTextSize(1);
	display.print(F("or open circuit!"));

	display.setCursor(0, 40);
	display.setTextSize(1);
	display.print(F("Fix and reset!"));

	display.display();

	// disable heater, trigger watchdog and blink L-LED.
	analogWrite(HEATER_PIN, 0);
	pinMode(13, OUTPUT);
	while (true)
	{
		wdt_reset();
		digitalWrite(13, HIGH);
		delay(500);
		wdt_reset();
		digitalWrite(13, LOW);
		delay(500);
	}
}
void tripLowVoltage()
{
	Timer1.detachInterrupt();
	disableInterrupt(LEFT_BUTTON);
	disableInterrupt(MIDDLE_BUTTON);
	disableInterrupt(RIGHT_BUTTON);

	// Locks processor when undervoltage occurs. 
	display.clearDisplay();
	display.setCursor(0, 0);
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.println(F("LiPoSolder"));
	
	display.setCursor(0, 20);
	display.setTextSize(1);
	display.println(F("Undervolt. protection!"));

	display.setCursor(0, 40);
	display.setTextSize(1);
	display.print(F("Battery voltage is \nunder 10V!"));

	display.display();

	// disable heater, trigger watchdog and blink L-LED.
	analogWrite(HEATER_PIN, 0);
	pinMode(13, OUTPUT);
	while (true)
	{
		wdt_reset();
		digitalWrite(13, HIGH);
		delay(500);
		wdt_reset();
		digitalWrite(13, LOW);
		delay(500);
	}
}
