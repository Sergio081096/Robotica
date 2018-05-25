//Librerías
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
//Crea un arreglo de tipo multif32 de la libreria std_msgs
std_msgs::Float32MultiArray data_robot;
//Crea un nodo en ros llamado n
ros::NodeHandle n;
//----------------------------------------
//Asignación de los pines del ARDUINO-UNO
//Motores
int m_M1A = 2; //Motor1_A
int M1_pwm = 3; //Motor1_pwm
int m_M1B = 4; //Motor1_B
int m_M2A = 5; //Motor2_A
int M2_pwm = 6; //Motor2_pwm
int m_M2B = 7; //Motor2_B
//Sensores
/* Sensores analógicos
A0 -- Sensor de Luz derecho
A1 -- Sensor de Luz izquierdo
A2 -- Sensor de temperatura
A4 -- Divisor de voltaje para la medición de la carga de la batería
A5 -- Acelerómetro SCL
A6 -- Acelerómetro SDA */
//--------------------------------------
//Variables globales
int M1A=0; //Valor de dirección del Motor1_A
int M1B=0; //Valor de dirección del Motor1_B
int M2A=0; //Valor de dirección del Motor2_A
int M2B=0; //Valor de dirección del Motor2_B
int pwm_1 = 0; //Valor de dirección del Motor_1
int pwm_2 = 0; //Valor de dirección del Motor_2
//Arreglo para los sensores
float sens[4]={A0,A1,A2,A3}; //Arreglo de los pines de los sensores
float lect_sens[4] = {0,0,0,0}; //Arreglos para guardar los datos de los sensores
// >>>>>>>>>>>>>>>>>> FUNCIONES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void SpeedsCallback(const std_msgs::Float32MultiArray&msg){
if (msg.data[0] > 0 && msg.data[1]>0){
M1A = 0;
M1B = 1;
pwm_1 = int(255 * msg.data[0]);
M2A = 0;
M2B = 1;
pwm_2 = int(255 * msg.data[1]);
}
else if (msg.data[0] < 0 && msg.data[1] < 0){
M1A = 1;
M1B = 0;
pwm_1 = int(-255 * msg.data[0]);
M2A = 1;
M2B = 0;
pwm_2 = int(-255 * msg.data[1]);
}
else if (msg.data[0] > 0 && msg.data[1] < 0){
M1A = 0;
M1B = 1;
pwm_1 = int(255 * msg.data[0]);
M2A = 1;
M2B = 0;
pwm_2 = int(-255 * msg.data[1]);
}
else if (msg.data[0] < 0 && msg.data[1]>0){
M1A = 1;
M1B = 0;
pwm_1 = int(-255 * msg.data[0]);
M2A = 0;
M2B = 1;
pwm_2 = int(255 * msg.data[1]);
}
else { //ALTO
M1A = 0;
M1B = 0;
M2A = 0;
M2B = 0; }
}// Fin de SpeedsCallBack
//--------------------------------------------------------------------------
//Indica los tópicos que utilizará y su función
//Publica los datos de los sensores obtenidos por medio del arduino
ros::Publisher p_sensors("/rotombot/hardware/sensors", &data_robot);
//Se subscribe al tópico publicado en el Raspberry para indicar la dirreción y velocidad del robot
ros::Subscriber<std_msgs::Float32MultiArray> robot_Speeds("/rotombot/hardware/motor_speeds",&SpeedsCallback);

//SETUP
void setup(){
n.getHardware()->setBaud(500000);
//Iniciar nodo en ros
n.initNode();
//Advertir a nodo que voy a publicar
n.advertise(p_sensors);
n.subscribe(robot_Speeds);
//Declara tamaño del arreglo a publicar
data_robot.data_length = 4;
//Configuracion de los sensores de entrada
for(int i=0; i<4; i++){
pinMode(sens[i], INPUT);}
//Configuracion de los pines de salida -- Control Motores
pinMode(m_M1A, OUTPUT);
pinMode(m_M1B, OUTPUT);
pinMode(m_M2A, OUTPUT);
pinMode(m_M2B, OUTPUT);
pinMode(M1_pwm, OUTPUT);
pinMode(M2_pwm, OUTPUT);
} //Fin del SETUP
//******************************************************************************************************
void loop(){
//Lee sensores de temperatura, bateria y dos foto resistencias
for(int j=0; j<4; j++){
lect_sens[j] = analogRead(sens[j]);
delay(10);
}
//Paso datos de lect_sens al arreglo a publicar
data_robot.data = lect_sens;
//Publica los datos de mi arreglo a mi topico
p_sensors.publish(&data_robot);
//Lo haga repetidamente
n.spinOnce();
digitalWrite(m_M1A,M1A);
digitalWrite(m_M1B,M1B);
analogWrite(M1_pwm, pwm_1);
digitalWrite(m_M2A,M2A);
digitalWrite(m_M2B,M2B);
analogWrite(M2_pwm, pwm_2);
} /// FIN DEL LOOP
