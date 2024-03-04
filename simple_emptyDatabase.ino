/***************************************************
  This is an example sketch for our optical Fingerprint sensor
  Designed specifically to work with the Adafruit Fingerprint sensor
  ----> http://www.adafruit.com/products/751
  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

  This example was modified by @highonvoltage on 04.03.2024
 ****************************************************/
 
 #include <Adafruit_Fingerprint.h>

//Establish a software serial interface for communication with the sensor
SoftwareSerial mySerial(2, 3);
//Initialize the interface object 
Adafruit_Fingerprint sensor = Adafruit_Fingerprint(&mySerial);

void setup()
{
  //Start the serial port with baud rate 9600 for user interaction
  Serial.begin(9600);
  //Wait for initialization
  delay(100);

  //Inform user
  Serial.println("Deleting fingerprint templates!");
  //Wait
  delay(2000);
  //Set the baud rate for the communication with the sensor
  sensor.begin(57600);

  //Connect to the sensor using no password
  if (sensor.verifyPassword()) 
  {
    //A sensor was found
    Serial.println("Sensor found!");
  } 
  else 
  {
    //No sensor was found
    Serial.println("No sensor found!");
    //Wait indefinitely
    while (1) { delay(1); }
  }
  //Empty database
  sensor.emptyDatabase();
  //Inform user
  Serial.println("Database cleared");
}

void loop() 
{
}

