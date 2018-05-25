#include <ros.h>
#include <std_msgs/Float32MultiArray.h>

std_msgs::Float32MultiArray data_robot;

ros::NodeHandle n;

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

float sens[4]={A0,A1,A2,A3}; //Arreglo de los pines de los sensores
float lect_sens[4] = {0,0,0,0}; //Arreglos para guardar los datos de los sensores

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

ros::Publisher p_sensors("/rotombot/hardware/arduino_sensors", &data_robot);

ros::Subscriber<std_msgs::Float32MultiArray> robot_Speeds("/rotombot/hardware/motor_speeds",&SpeedsCallback);


void setup()
{
  n.getHardware()->setBaud(500000);
  n.initNode();
  n.advertise(p_sensors);
  n.subscribe(robot_Speeds);
  data_robot.data_length = 4;
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
  for(int j=0; j<4; j++)
  {
    lect_sens[j] = analogRead(sens[j]);
    delay(10);
  }
  data_robot.data = lect_sens;
  p_sensors.publish(&data_robot);
  n.spinOnce();
  digitalWrite(m_M1A,M1A);
  digitalWrite(m_M1B,M1B);
  analogWrite(M1_pwm, pwm_1);
  digitalWrite(m_M2A,M2A);
  digitalWrite(m_M2B,M2B);
  analogWrite(M2_pwm, pwm_2);
}
