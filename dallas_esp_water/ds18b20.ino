


void setupDs18b20()
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();
  
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, 0))
	{
	
		Serial.print("Setting resolution to ");
		Serial.println(TEMPERATURE_PRECISION, DEC);
		
		// set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
		sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
		
		 Serial.print("Resolution actually set to: ");
		Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
		Serial.println();
	}else{
		Serial.print("Found ghost device at ");
		Serial.print(0, DEC);
		Serial.print(" but could not detect address. Check power and cabling");
	}
 
}

// function to print the temperature for a device
float printTemperature()
{
   // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
    sensors.requestTemperatures(); // Send the command to get temperatures
  
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, 0))
  {
    // It responds almost immediately. Let's print out the data
   // Use a simple function to print out the data
     float tempC = sensors.getTempC(tempDeviceAddress);
     Serial.print("Temp C: ");
     Serial.println(tempC);
     return tempC;
  } 
  //else ghost device! Check your power requirements and cabling
  
 
}



