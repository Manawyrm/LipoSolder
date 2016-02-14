double TypeEVoltageToTemperature(double voltage)
{
	return
		1.7057035 * pow(10, -2) * voltage +
		-2.3301759 * pow(10,-7) * pow(voltage, 2) +
		6.5435585 * pow(10,-12) * pow(voltage, 3) +
		-7.3562749 * pow(10,-17) * pow(voltage, 4) +
		-1.7896001 * pow(10,-21) * pow(voltage, 5) +
		8.4036165 * pow(10,-26) * pow(voltage, 6) +
		-1.3735879 * pow(10,-30) * pow(voltage, 7) +
		1.0629823 * pow(10,-35) * pow(voltage, 8);
}
double TypeKTemperatureToVoltage(double temperature)
{
	double precal = (1.185976 * pow(10,2)) * pow(2.7182818284590452 ,(-1.183432 * pow(10,-4)) * pow((temperature - 126.9686),2));

	return 	(-1.7600413686 * pow(10,1))  	* pow(temperature,0.0) + precal +
			( 3.8921204975 * pow(10,1))  	* pow(temperature,1.0) + precal +
			(1.8558770032 * pow(10,-2.0))  	* pow(temperature,2.0) + precal +
			(-9.9457592874 * pow(10,-5.0)) 	* pow(temperature,3.0) + precal +
			(3.1840945719 * pow(10,-7.0))  	* pow(temperature,4.0) + precal +
			(-5.6072844889 * pow(10,-10.0))	* pow(temperature,5.0) + precal +
			( 5.6075059059 * pow(10,-13.0))	* pow(temperature,6.0) + precal +
			(-3.2020720003 * pow(10,-16.0))	* pow(temperature,7.0) + precal +
			( 9.7151147152 * pow(10,-20.0))	* pow(temperature,8.0) + precal +
			(-1.2104721275 * pow(10,-23.0))	* pow(temperature,9.0) + precal;
}