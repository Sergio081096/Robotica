//>>>>>>>> LIBRERÍAS <<<<<<<<<<<<<
#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <termio.h>
#include <sys/ioctl.h>
#include <wiringPi.h> //librería para el uso de los GPIO
//----------------------------------------------------------------------------------------------------------------
//>>>>>> VARIABLES GLOBALES <<<<<<<<<<<
//Pines Raspberry en uso
#define TRIG 0 //Header 11
#define ECHO_1 1 //Header 12
#define ECHO_2 2 //Header 13
#define ECHO_3 3 //Header 15
#define ECHO_4 4 //Header 16
#define ECHO_5 5 //Header 18
#define ECHO_6 6 //Header 22
#define ECHO_7 7 //Header 07
#define ECHO_8 10 //Header 24
//Sensores
float ultraS[8]={0,0,0,0,0,0,0,0}; //Variables que guardan las distancias obtenidas de los sensores ultrasónicos -->RASPBERRY
//Movimientos del Carro con teclado
float cont =0.1; //Aumentos
float v_Der = 0.0, vel_Izq=0.0, v = 0.0;
// Vel_Der Vel_Izq Vel Gral
int j; //Contador auxiliar
//----------------------------------------------------------------------------------------------------------------
//>>> >FUNCIONES <<<<<<<<<<<<<<
//RASPERRY PI
//Configuración del los pines del la rasperry
void setup()
{
	wiringPiSetup();
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO_1, INPUT);
	pinMode(ECHO_2, INPUT);
	pinMode(ECHO_3, INPUT);
	pinMode(ECHO_4, INPUT);
	pinMode(ECHO_5, INPUT);
	pinMode(ECHO_6, INPUT);
	pinMode(ECHO_7, INPUT);
	pinMode(ECHO_8, INPUT);
	//Pin TRIG debe comenza en bajo
	digitalWrite(TRIG, LOW);
	delay(30);
} ///Fin del SETUP
//Medición de las distancias por medio de los sensores Ultrasónicos
float getCM(int i)
{
	//std::cout<<"Iniciando toma de distancias"<<std::endl;
	float distance=0;
	long travelTime=0,startTime=0;
	int wait=0.0;
	//Pulso de TRIG
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(20);
	digitalWrite(TRIG, LOW);
	switch(i)
	{
		case 1:
			//Espera a la respuesta del ECHO
			while(digitalRead(ECHO_1) == LOW)
			{
				if(wait>=10000)
				{
					std::cout<<"No se pudo obtener información del ultrasónico 1"<<std::endl;
					break;
				}
				wait++;
			}//Fin de la espera de la respuesta
			//Capta el ECHO y espera a que termine
			startTime = micros();
			while(digitalRead(ECHO_1) == HIGH);
			travelTime = micros() - startTime;
			break;
		case 2:
		//Espera a la respuesta del ECHO
			while(digitalRead(ECHO_1) == LOW)
			{
				if(wait>=10000)
				{
					std::cout<<"No se pudo obtener información del ultrasónico 2"<<std::endl;
					break;
				}
				wait++;
			}//Fin de la espera de la respuesta
			//Capta el ECHO y espera a que termine
			startTime = micros();
			while(digitalRead(ECHO_2) == HIGH);
			travelTime = micros() - startTime;
			break;
		case 3:
			//Espera a la respuesta del ECHO
			while(digitalRead(ECHO_1) == LOW)
			{
				if(wait>=10000)
				{
					std::cout<<"No se pudo obtener información del ultrasónico 3"<<std::endl;
					break;
				}
				wait++;
			}//Fin de la espera de la respuesta
			//Capta el ECHO y espera a que termine
			startTime = micros();
			while(digitalRead(ECHO_3) == HIGH);
			travelTime = micros() - startTime;
			break;
		case 4:
			//Espera a la respuesta del ECHO
			while(digitalRead(ECHO_1) == LOW)
			{
				if(wait>=10000)
				{
					std::cout<<"No se pudo obtener información del ultrasónico 4"<<std::endl;
					break;
				}
				wait++;
			}//Fin de la espera de la respuesta
			//Capta el ECHO y espera a que termine
			startTime = micros();
			while(digitalRead(ECHO_4) == HIGH);
			travelTime = micros() - startTime;
			break;
		case 5:
			//Espera a la respuesta del ECHO
			while(digitalRead(ECHO_1) == LOW)
			{
				if(wait>=10000)
				{
					std::cout<<"No se pudo obtener información del ultrasónico 5"<<std::endl;
					break;
				}
				wait++;
			}//Fin de la espera de la respuesta
			//Capta el ECHO y espera a que termine
			startTime = micros();
			while(digitalRead(ECHO_5) == HIGH);
			travelTime = micros() - startTime;
			break;
		case 6:
			//Espera a la respuesta del ECHO
			while(digitalRead(ECHO_1) == LOW)
			{
				if(wait>=10000)
				{
					std::cout<<"No se pudo obtener información del ultrasónico 6"<<std::endl;
					break;
				}
			wait++;
			}//Fin de la espera de la respuesta
			//Capta el ECHO y espera a que termine
			startTime = micros();
			while(digitalRead(ECHO_6) == HIGH);
			travelTime = micros() - startTime;
			break;
		case 7:
			//Espera a la respuesta del ECHO
			while(digitalRead(ECHO_1) == LOW)
			{
				if(wait>=10000)
				{
					std::cout<<"No se pudo obtener información del ultrasónico 7"<<std::endl;
					break;
				}
				wait++;
			}//Fin de la espera de la respuesta
			//Capta el ECHO y espera a que termine
			startTime = micros();
			while(digitalRead(ECHO_7) == HIGH);
			travelTime = micros() - startTime;
			break;
		case 8:
			//Espera a la respuesta del ECHO
			while(digitalRead(ECHO_1) == LOW)
			{
				if(wait>=10000)
				{
					std::cout<<"No se pudo obtener información del ultrasónico 8"<<std::endl;
					break;
				}
				wait++;
			}//Fin de la espera de la respuesta
			//Capta el ECHO y espera a que termine
			startTime = micros();
			while(digitalRead(ECHO_8) == HIGH);
			travelTime = micros() - startTime;
			break;
		default:
			printf("Error, no existe ese Sensor Ultrasónico\n");
		break;
	}//Fin de switch case
	//Obtiene la distancia en centimetros
	distance = travelTime / 58;
	return distance;
}//Find de la función getCM
	//ARDUINO -- ROS
	//Obtención de los datos recopilados por el arduino y trasnmitidos por serial

int main(int argc, char** argv)
{
	setup(); ///Configuración de los pines de la RaspberryPi para el uso de los ultrasónicos
	std::cout<<"Marcosoft ROTOM-BOT en linea"<<std::endl;
	ros::init(argc,argv,"fotoresistencias");//********************************************
	ros:: NodeHandle n;
	//Obtención de los datos transmitidos del ARDUINO
	ros::Subscriber sub = n.subscribe("/rotombot/hardware/sensors", 1000, datosObtenidos);
	std_msgs::Float32MultiArray D_Motor; //Velocidades del motor
	D_Motor.data.resize(2);
	std_msgs::Float32MultiArray dist_U; //Distancias de los ultrasónicos
	dist_U.data.resize(8);
	//Publicación de las velocidades de los motores al ArduinoLaboratorio de Biorobótica

	ros::Publisher pubCount=n.advertise <std_msgs::Float32MultiArray>("/rotombot/hardware/motor_speeds",1);
	//Publicación de los datos (distancias) obtenidos de los sensores ultrasónicos
	ros::Publisher pubDist= n.advertise <std_msgs::Float32MultiArray>("/rotombot/hardware/dist_ultra",1);
	ros::Rate loop(10);
	ros::Rate r(1000);
	int tecla= 32; //Obtener el valor en ASCII de la tecla presionada
	while(ros::ok())
	{
		for(j=1; j<=8;j++)
		{ //Toma de datos de todos los sensores ultrasónicos -- descomentar linea
			ultraS[j-1]=getCM(j);
			dist_U.data[j-1]=ultraS[j-1]; //Se copian los datos para ser publicados en un tópico
			//printf("Distance_%i: %f [cm]\n",j, ultraS[j-1]);
		}//fin del FOR
		//Toma de datos de la tecla
		tecla=(int)getchar();
		switch(tecla)
		{
			case 97: //[A]--ACELERACIÓN UNIFORME
				if(v>=1.0)
				{
					std::cout<<"[!] VELOCIDAD MÁXIMA ALCANZADA[!]"<<v<<std::endl;
				}
				else
				{
					v=v+0.1;
				}
				break;
			case 100: //[D] -- DESACELERACIÓN UNIFORME
				if(v<=-1.0)
				{
					std::cout<<"[!] VELOCIDAD MÍNIMA ALCANZADA[!]"<<v<<std::endl;
				}
				else
				{
					v=v-0.1;
				}
				break;
			case 119: //[W] -- AVANZAR UNIFORME
				D_Motor.data[0]=v;
				D_Motor.data[1]=v;
				break;
			case 115: //[S] -- RETROCEDER UNIFORME
				D_Motor.data[0]=-v;
				D_Motor.data[1]=-v;
				break;
			case 113: //[Q] -- GIRO IZQUIERDA UNIFORME
				D_Motor.data[0]=v;
				D_Motor.data[1]=-v;
				break;
			case 101: //[E] -- GIRO DERECHA UNIFORME
				D_Motor.data[0]=-v;
				D_Motor.data[1]=v;
				break;
			case 32: //[ESPACIO] -- ALTO
				D_Motor.data[0]=0.0;
				D_Motor.data[1]=0.0;
				break;
			case 10: //[ENTER] :: Envió de los datos y realización de las acciones
				std::cout<<"Enviando instrucciones"<<std::endl;
				std::cout<<"Vel:_ "<<v<<std::endl;
				std::cout<<"Vel_Der:_ "<<D_Motor.data[0]<<std::endl;
				std::cout<<"Vel_Izq:_ "<<D_Motor.data[1]<<std::endl;
				break;
			default: //[CUALQUIER OTRA TECLA] -- ALTO
				std::cout<<"[!] TECLA NO DEFINIDA [!]"<<std::endl;
				D_Motor.data[0]=0.0;
				D_Motor.data[1]=0.0;
			break;
		}//Fin del Switch cases
		pubCount.publish(D_Motor); //Envió de datos al arduino
		pubDist.publish(dist_U); //Se publican los datos de los sensores ultrasónicos
		ros::spinOnce();
		loop.sleep();
		std::cout<<""<<std::endl;
	} //Fin del while(ROS)
	return 0;
} //Fin del main
