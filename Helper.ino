void printIntWithLeadingZeroes(uint16_t value, uint8_t digits)
{
	uint8_t valueLength = getIntLength(value);
	if ((digits - valueLength) > 0)
	{
		for(int i=0; i<(digits - valueLength); i++)
		{
		    display.write(0x20);
		}
	}
	display.print(value);
}
uint8_t getIntLength(uint16_t value)
{
	for(int i=1;; i++)
	{
		value = value / 10;
		if (value == 0)
			return i;
	}
}
