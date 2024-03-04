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

//Id for each next fingerprint
int act_id;
//This code runs once each time the controller starts
void setup()
{
  //Start the serial port with baud rate 9600 for user interaction
  Serial.begin(9600);
  //Wait for initialization
  delay(100);
  //Greet the user
  Serial.println("Add new fingerprints");

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

  //Get the sensor parameters
  sensor.getParameters();
  act_id = sensor.templateCount + 1;
}

//This function is called repetitively during controller operation
void loop()             
{
  Serial.println("Place a new finger");
  Serial.print("Storing as #");
  Serial.println(act_id);
  while (! addTemplate() );
  act_id++;
}

int addTemplate() {

  int p = -1;
  Serial.print("Waiting for finger to add as #"); Serial.println(act_id);
  while (p != FINGERPRINT_OK) {
    p = sensor.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = sensor.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = sensor.getImage();
  }
  Serial.print("ID "); Serial.println(act_id);
  p = -1;
  Serial.println("Place the finger again");
  while (p != FINGERPRINT_OK) {
    p = sensor.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = sensor.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating template for #");  Serial.println(act_id);

  p = sensor.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(act_id);
  p = sensor.storeModel(act_id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}
