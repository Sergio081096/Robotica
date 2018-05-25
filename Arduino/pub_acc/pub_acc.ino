#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>


ros::NodeHandle nh;

std_msgs::Float32MultiArray msg;
ros::Publisher pub_acc("/rotombot/hardware/arduino_sensors", &msg);

LSM9DS1 imu;

#define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

#define PRINT_CALCULATED

static unsigned long lastPrint = 0; // Keep track of

#define DECLINATION -8.58 // Declination (degrees) in Boulder, CO.

int m_M1A = 2; //Motor1_A
int M1_pwm = 3; //Motor1_pwm
int m_M1B = 4; //Motor1_B
int m_M2A = 5; //Motor2_A
int M2_pwm = 6; //Motor2_pwm
int m_M2B = 7; /*Motor2_B
A0 -- Sensor de Luz derecho
A1 -- Sensor de Luz izquierdo
A2 -- Sensor de temperatura
A4 -- Divisor de voltaje para la medición de la carga de la batería*/
int M1A=0; //Valor de dirección del Motor1_A
int M1B=0; //Valor de dirección del Motor1_B
int M2A=0; //Valor de dirección del Motor2_A
int M2B=0; //Valor de dirección del Motor2_B
int pwm_1 = 0; //Valor de dirección del Motor_1
int pwm_2 = 0; //Valor de dirección del Motor_2


float lect_acc[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
float sens[4]={A0,A1,A2,A3}; 

void SpeedsCallback(const std_msgs::Float32MultiArray&msg)
{
  if (msg.data[0] > 0 && msg.data[1]>0)
  {
    M1A = 0;
    M1B = 1;
    pwm_1 = int(500 * msg.data[0]);
    M2A = 1;
    M2B = 0;
    pwm_2 = int(500 * msg.data[1]);
  }
  else if (msg.data[0] < 0 && msg.data[1] < 0)
  {
    M1A = 1;
    M1B = 0;
    pwm_1 = int(-500 * msg.data[0]);
    M2A = 0;
    M2B = 1;
    pwm_2 = int(-500 * msg.data[1]);
  }
  else if (msg.data[0] > 0 && msg.data[1] < 0)
  {
    M1A = 0;
    M1B = 1;
    pwm_1 = int(500 * msg.data[0]);
    M2A = 0;
    M2B = 1;
    pwm_2 = int(-500 * msg.data[1]);
  }
  else if (msg.data[0] < 0 && msg.data[1]>0)
  {
    M1A = 1;
    M1B = 0;
    pwm_1 = int(-500 * msg.data[0]);
    M2A = 1;
    M2B = 0;
    pwm_2 = int(500 * msg.data[1]);
  }
  else 
  { //ALTO
    M1A = 0;
    M1B = 0;
    M2A = 0;
    M2B = 0;
  }
}

ros::Subscriber<std_msgs::Float32MultiArray> robot_Speeds("/rotombot/hardware/motor_speeds",&SpeedsCallback);

void setup()
{ 
  nh.getHardware()->setBaud(500000);
  nh.initNode();
  nh.advertise(pub_acc);
  nh.subscribe(robot_Speeds);
  msg.data_length = 13;

  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;

  imu.begin();
  
  for(int i=0; i<4; i++)
  {
    pinMode(sens[i], INPUT);
  }
  pinMode(m_M1A, OUTPUT);
  pinMode(m_M1B, OUTPUT);
  pinMode(m_M2A, OUTPUT);
  pinMode(m_M2B, OUTPUT);
  pinMode(M1_pwm, OUTPUT);
  pinMode(M2_pwm, OUTPUT);
 
}


void loop()
{
 if ( imu.gyroAvailable() )
  {
    imu.readGyro();
  }
  if ( imu.accelAvailable() )
  {
    imu.readAccel();    
  }
  if ( imu.magAvailable() )
  {
     imu.readMag();
  } 
  msg.data = lect_acc;
  for(int j=0; j<4; j++)
  {
    if(j==2)
    {
      msg.data[j]=analogRead(sens[j])-140.12;
    }
    else
    {
      msg.data[j]=analogRead(sens[j]);
    }
    delay(50);
  }
  /*#ifdef PRINT_CALCULATED
    msg.data[4] = imu.calcGyro(imu.gx);
    msg.data[5] = imu.calcGyro(imu.gy);
    msg.data[6] = imu.calcGyro(imu.gz);
    msg.data[7] = imu.calcAccel(imu.ax);
    msg.data[8] = imu.calcAccel(imu.ay);
    msg.data[9] = imu.calcAccel(imu.az);
    msg.data[10] =imu.calcMag(imu.mx);
    msg.data[11] =imu.calcMag(imu.my);
    msg.data[12] =imu.calcMag(imu.mz);
  #elif defined PRINT_RAW*/
    msg.data[4]= (imu.gx);
    msg.data[5]= (imu.gy);
    msg.data[6]= (imu.gz);
    msg.data[7]= (imu.ax);
    msg.data[8]= (imu.ay);
    msg.data[9]= (imu.az);
    msg.data[10]= (imu.mx);
    msg.data[11]= (imu.my);
    msg.data[12]= (imu.mz);
  
     
  pub_acc.publish( &msg );
  nh.spinOnce();
  digitalWrite(m_M1A,M1A);
  digitalWrite(m_M1B,M1B);
  analogWrite(M1_pwm, pwm_1);
  digitalWrite(m_M2A,M2A);
  digitalWrite(m_M2B,M2B);
  analogWrite(M2_pwm, pwm_2);
}









