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

//This code runs once each time the controller starts
void setup()
{
  //Start the serial port with baud rate 9600 for user interaction
  Serial.begin(9600);
  //Wait for initialization
  delay(100);
  //Greet the user
  Serial.println("Beginner fingerprint tutorial :)");

  //Set the baud rate for the communication with the sensor
  sensor.begin(57600);
  //Wait for initialization
  delay(5);
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

  //Get the sensor memory positions
  Serial.println("Reading sensor storage capacity");
  sensor.getParameters();
  Serial.print("Capacity: "); 
  Serial.println(sensor.capacity);

  //Get the number of stored templates
  sensor.getTemplateCount();

  //Check if templates are stored
  if (sensor.templateCount == 0) 
  {
    //No template found
    Serial.print("No fingerprint data!");
  }
  else 
  {
    //Show the number of available templates
    Serial.print("The sensor contains "); 
    Serial.print(sensor.templateCount); 
    Serial.println(" stored templates");
  }
}

//This function is called repetitively during controller operation
void loop()                   
{
  //Check if finger is placed 
  int recognizedID = readPrint();
  if(recognizedID != -1)
  {
    //Finger recognized
    Serial.print("Recognized ID ");
    Serial.println(recognizedID);
  }
  delay(10);            
}


int readPrint() {
  //Capture image
  uint8_t p = sensor.getImage();
  if (p != FINGERPRINT_OK)  return -1;
  //Convert to feature template
  p = sensor.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  //Check if finger matches
  p = sensor.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  //Check sufficient confidence
  if(sensor.confidence < 100) return -1;
  return sensor.fingerID;
}
