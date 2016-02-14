void setNewTemperature(double newTemperature)
{
	if (newTemperature < 0) return; 
	if (newTemperature > 500) return; 

	setTemperature = newTemperature;

	EEPROM.write(1, ((uint16_t)newTemperature) & 0xFF);
 	EEPROM.write(2, ((uint16_t)newTemperature) >> 8);
} 
