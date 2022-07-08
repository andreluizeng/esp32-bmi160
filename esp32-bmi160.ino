#include <BMI160Gen.h>
#include <HardwareSerial.h>
#include <KalmanFilter.h>
#include <MovingAvgFilter.h>

MovingAvgFilter filter_gx;
MovingAvgFilter filter_gy;
MovingAvgFilter filter_gz;

MovingAvgFilter filter_roll;
MovingAvgFilter filter_pitch;
MovingAvgFilter filter_yaw;

//--------------------------
// BMI160 CS (not used in this project)
//--------------------------
const int select_pin = 10;
const int i2c_addr = 0x69;
//--------------------------

//--------------------------
// filtro de kalman
//--------------------------
KalmanFilter kalmanX(0.001, 0.003, 0.03);
KalmanFilter kalmanY(0.001, 0.003, 0.03);

float accPitch = 0;
float accRoll = 0;
float kalPitch = 0;
float kalRoll = 0;
float yaw = 0;

float gyroXZero = 0;
float gyroYZero = 0;
float gyroZZero = 0;

float accPitchZero = 0;
float accRollZero = 0;

int gx, gy, gz;
float gyroX, gyroY, gyroZ;
int accX, accY, accZ;

float elapsedTime, currentTime, previousTime;

//--------------------------

//--------------------------
// hardware serial
//--------------------------
HardwareSerial serial1 (1);

#define RXD 16
#define TXD 17
//--------------------------

void setup() 
{
  Serial.begin(115200); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open
 
  // initialize device
  //BMI160.begin(BMI160GenClass::SPI_MODE, select_pin);
  BMI160.begin(BMI160GenClass::I2C_MODE, i2c_addr);
  BMI160.setGyroRange(250);
  serial1.begin (115200, SERIAL_8N1, RXD, TXD);

  filter_gx.begin (10);
  filter_gy.begin (10);
  filter_gz.begin (10);

  filter_pitch.begin (10);
  filter_roll.begin (10);
  filter_yaw.begin (10);
  
  delay (500);
  for (int i = 0; i < 30; i++)
  {
    BMI160.readGyro(gx, gy, gz);
    BMI160.readAccelerometer(accX, accY, accZ);

    accPitch = -(atan2(accX, sqrt(accY*accY + accZ*accZ))*180.0)/M_PI;
    accRoll  = (atan2(accY, accZ)*180.0)/M_PI;
    
    gyroX = convertRawGyro(gx);
    gyroY = convertRawGyro(gy);
    gyroZ = convertRawGyro(gz);

    filter_gx.calculateAvg (gyroX);
    filter_gy.calculateAvg (gyroY);
    filter_gz.calculateAvg (gyroZ);

    filter_pitch.calculateAvg (accPitch);
    filter_roll.calculateAvg (accRoll);
    
    delay (50);
  }

  gyroXZero = filter_gx.getAvg ();
  gyroYZero = filter_gy.getAvg ();
  gyroZZero = filter_gz.getAvg ();

  accPitchZero = filter_pitch.getAvg ();
  accRollZero = filter_roll.getAvg ();

  filter_gx.reset ();
  filter_gy.reset ();
  filter_gz.reset ();

  filter_pitch.reset ();
  filter_roll.reset ();
  filter_yaw.reset ();

  return;
}

void loop() 
{
  static float accumulator = 0;
  BMI160.readGyro(gx, gy, gz);
  BMI160.readAccelerometer(accX, accY, accZ);


  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds

  
  gyroX = convertRawGyro(gx);
  gyroY = convertRawGyro(gy);
  gyroZ = convertRawGyro(gz);

  // remove o erro inicial
  gyroX  = gyroX - gyroXZero; // remove o drift
  gyroY  = gyroY - gyroYZero; // remove o drift
  gyroZ  = gyroZ - gyroZZero; // remove o drift

  if ((gyroX < 0.05) && (gyroX > -0.05))
     gyroX = 0;

  if ((gyroY < 0.05) && (gyroY > -0.05))
    gyroY = 0;

  if ((gyroZ < 0.03) && (gyroZ > -0.03))
    gyroZ = 0;


  gyroX = filter_gx.calculateAvg (gyroX);
  gyroY = filter_gy.calculateAvg (gyroY);
  gyroZ = filter_gz.calculateAvg (gyroZ);

  accPitch = -(atan2(accX, sqrt(accY*accY + accZ*accZ))*180.0)/M_PI;
  accRoll  = (atan2(accY, accZ)*180.0)/M_PI;
  yaw =  yaw + gyroZ * elapsedTime;

  accPitch -= accPitchZero;
  accRoll -= accRollZero;

  if ((accPitch < 0.05) && (accPitch > -0.05))
     gyroX = 0;

  if ((accRoll < 0.05) && (accRoll > -0.05))
    accRoll = 0;

  // Kalman filter
  kalPitch = kalmanY.update(accPitch, gyroY);
  kalRoll = kalmanX.update(accRoll, gyroX);

  Serial.print(kalPitch);
  Serial.print(",");
  Serial.print(kalRoll);
  Serial.print(",");
  Serial.print(yaw);
  Serial.println();
  
  delay(1);
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767

  float g = (gRaw * 250.0) / 32768.0;

  return g;
}
