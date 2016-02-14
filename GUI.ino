// Renders initial startup logo
void renderLogo()
{
	display.clearDisplay();

	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.println(F("LiPoSolder"));
	
	display.setCursor(0, 40);
	display.setTextSize(1);
	display.println(F("FW-Revision 0.1"));

	display.setCursor(0, 50);
	display.setTextSize(1);
	display.print(F("by Tobias M")); display.write(0x84); display.println(F("del"));

	display.display();
}

// Renders display header
void renderHeader(uint8_t displayTitle)
{
	display.setTextColor(BLACK, WHITE);
	display.setCursor(0,0);
	display.setTextSize(1);
	if (displayTitle == 1)
	{
		// render title line
		display.println(F(" LiPoSolder         "));
	}
	else
	{
		display.println(F("                    "));
	}
	// display battery voltage
	display.setCursor(85,0);
	display.print(batteryVoltage);
	display.setCursor(120,0);
	display.print("V");

	display.setTextColor(WHITE);
}
