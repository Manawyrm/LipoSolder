
// Renders the first page - Status informations
void statusPage()
{
	if (leftButtonActive)
	{
		setNewTemperature(setTemperature + 10);
		leftButtonActive = 0;
	}
	if (middleButtonActive)
	{
		setNewTemperature(setTemperature - 10);
		middleButtonActive = 0;
	}

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

uint8_t currentPreset = 0; 
uint16_t presets[4] = {20, 320, 360, 420};

void presetPage()
{
	if (!leftButtonActive || !middleButtonActive)
	{
		if (leftButtonActive)
		{
			currentPreset++; 
			if (currentPreset > sizeof(presets) / sizeof(uint16_t) - 1)
				currentPreset = 0; 

			leftButtonActive = 0;
		}
		if (middleButtonActive)
		{
			if (currentPreset != 0)
			{
				currentPreset--;
			}
			else
			{
				currentPreset = sizeof(presets) / sizeof(uint16_t) - 1;
			}
			middleButtonActive = 0;
		}
	}
	else
	{
		setNewTemperature(presets[currentPreset]);
		leftButtonActive = 0;
		middleButtonActive = 0;
		currentPage = 0;
		return; 
	}
	display.clearDisplay();

	renderHeader(0);

	display.setTextColor(BLACK, WHITE);
	display.setCursor(0,0);
	display.setTextSize(1);
	display.println(F(" Presets"));
	display.setTextColor(WHITE);

	for(int i=0; i<sizeof(presets) / sizeof(uint16_t); i++)
	{
		display.setCursor(0, 15 + (i * 10));
	    if (i == currentPreset)
	    {
	    	clearLine(1);
	    }
	    else
	    {
	    	clearLine(0);
	    }
	    display.setCursor(0, 15 + (i * 10));
	    display.print(F("   "));

	   	printIntWithLeadingZeroes(presets[i], 3);
	   	display.write(0xF7);display.println("C");
	}

	display.display();
}