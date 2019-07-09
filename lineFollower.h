  #include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;

int16_t lineSensorsValue[5] = {0, 0, 0, 0, 0};

void calibrateSensors(){
  delay(1000);
  for(uint16_t i = 0; i < 120; i++){
    lcd.gotoXY(0,1);
      lcd.print(i);
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200);
    }
    else
    {
      motors.setSpeeds(200, -200);
    }
    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
  lcd.clear();
}

void setup() {
 
  lineSensors.initFiveSensors(); // init the 5 Sensors
  buttonA.waitForButton();
  calibrateSensors();
  buttonA.waitForButton();

}

void loop() {  
    lineSensors.readCalibrated(lineSensorsValue);
  
      if(lineSensorsValue[2] >= 800 && lineSensorsValue[0] <= 800 && lineSensorsValue[4] <= 800){
        motors.setSpeeds(150, 150);
      }else if(lineSensorsValue[0] >= 800){ // turn left
        motors.setSpeeds(-150, 150);
        delay(200);
      }else if(lineSensorsValue[4] >= 800){ // turn right
        motors.setSpeeds(150, -150);
        delay(200);
      
    }
    
}
