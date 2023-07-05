
#include <QMC5883LCompass.h>
#include <FastLED.h>
#define NUM_LEDS 78
#define DATA_PIN 13

CRGBArray<NUM_LEDS> leds;

QMC5883LCompass compass;

void setup() {
  
  Serial.begin(9600);
  compass.init();
  /*
   *   call setSmoothing(STEPS, ADVANCED);
   *   
   *   STEPS     = int   The number of steps to smooth the results by. Valid 1 to 10.
   *                     Higher steps equals more smoothing but longer process time.
   *                     
   *   ADVANCED  = bool  Turn advanced smmothing on or off. True will remove the max and min values from each step and then process as normal.
   *                     Turning this feature on will results in even more smoothing but will take longer to process.
   *                     
   */
  compass.setSmoothing(1,true);  
  compass.setCalibration(-1332, 1050, 0, 2566, -551, 1822);

  FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS); 

}

float getHeading(){

  int x, y, z;
  
  // Read compass values
  compass.read();

  // Return XYZ readings
  x = compass.getX();
  y = compass.getY();
  z = compass.getZ();
  
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.print(y);
  Serial.print(" Z: ");
  Serial.print(z);
  Serial.println();

  float heading = atan2(y, x);

    // Setting the declination angle for Bytom 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;
 
  // cat. correction
  if (heading < 0)
  {
    heading += 2 * PI;
  }
 
  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }

    // Convert radians to degrees
  float headingDegrees = heading * 180/M_PI;
  Serial.print(heading);
  Serial.println();
  Serial.print(headingDegrees);
  Serial.println();
  
  return headingDegrees;
}

void loop() {
  float headingDegrees = getHeading();
  //normalize to about 0 for north 254 for north again.

  float headingDegreesNorm = 255 * headingDegrees/365;
  Serial.print(headingDegreesNorm);
  Serial.println();
  
  int hue = round(headingDegreesNorm);
  Serial.print(hue);
  Serial.println();
  
//  static_cast<hue>(headingDegrees);

//hue is normally a uint8 type. I'll try just an int?
  
  for(int i = 0; i < NUM_LEDS; i++) {   
    // let's set an led value
    leds[i] = CHSV(hue,255,255);
    FastLED.delay(3);
  }
  delay(250);
  
}
