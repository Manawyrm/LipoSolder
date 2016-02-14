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

