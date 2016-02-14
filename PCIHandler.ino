void leftButtonChanged()
{
	buttonPressed(0, digitalRead(LEFT_BUTTON));
}
void middleButtonChanged()
{
	buttonPressed(1, digitalRead(MIDDLE_BUTTON));
}
void rightButtonChanged()
{
	buttonPressed(2, digitalRead(RIGHT_BUTTON));
}

void buttonPressed(uint8_t button, uint8_t state)
{
	if (state == 0)
	{
		switch (button)
		{
			case 0:
				leftButtonPressed = 1; 
				break;
			case 1:
				middleButtonPressed = 1; 
				break;
			case 2:
				rightButtonPressed = 1; 
				break;
			default:
				break;
		}
	}
	if (state == 1)
	{
		switch (button)
		{
			case 0:
				leftButtonPressed = 255; 
				break;
			case 1:
				middleButtonPressed = 255; 
				break;
			case 2:
				rightButtonPressed = 255; 
				break;
			default:
				break;
		}
	}
}
void buttonRepeatISR()
{
	if (leftButtonPressed != 255)
	{
		if (leftButtonPressed % 10 == 1)
		{
			leftButtonAction();
			leftButtonPressed = 4; 
		}
		leftButtonPressed--;
	}
	if (middleButtonPressed != 255)
	{
		if (middleButtonPressed % 10 == 1)
		{
			middleButtonPressed = 4; 
		}
		middleButtonPressed--;
	}
	if (rightButtonPressed != 255)
	{
		if (rightButtonPressed % 10 == 1)
		{
			changePageButton();
			rightButtonPressed = 4; 
		}
		rightButtonPressed--;
	}
}

void leftButtonAction()
{
	Serial.println("left!");
}