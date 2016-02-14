
// Renders the first page - Status informations
void statusPage()
{
	/*if (!digitalRead(LEFT_BUTTON))
	{
		setNewTemperature(setTemperature + 10);
	}
	if (!digitalRead(MIDDLE_BUTTON))
	{
		setNewTemperature(setTemperature - 10);
	}*/

	display.clearDisplay();

	renderHeader(0);

	display.setTextColor(BLACK, WHITE);
	display.setCursor(0,0);
	display.setTextSize(1);
	display.println(F(" Status"));
	display.setTextColor(WHITE);


	display.fillRect(0, 15, map(outputDutyCycle, 0, 255, 0, 128), 4, 1);

	display.setCursor(0, 30);
	display.setTextSize(2);
	display.println(F("soll: "));

	display.setCursor(100, 30);
	display.setTextSize(2);
	display.write(0xF7); display.println("C");

	display.setCursor(65, 30);
	display.setTextSize(2);
	printIntWithLeadingZeroes(setTemperature, 3);


	display.setCursor(0, 50);
	display.setTextSize(2);
	display.println(F(" ist: "));

	display.setCursor(100, 50);
	display.setTextSize(2);
	display.write(0xF7);display.println("C");

	display.setCursor(65, 50);
	display.setTextSize(2);
	printIntWithLeadingZeroes(currentTemperature, 3);

	display.display();
}
void presetPage()
{
	display.clearDisplay();

	renderHeader(1);

	display.setCursor(0,20);
	display.setTextSize(1);
	display.println(F("I will make my own renderer with blackjack and hookers."));
	display.display();
}
