/*
 * rosserial Subscriber Example
 * 
 */

#include <ros.h>
#include <std_msgs/Int16.h>

ros::NodeHandle nh;
std_msgs::Int16 msg;

void messageCb( const std_msgs::Int16& toggle_msg){

//std_msgs::Int16 msg;
msg.data= toggle_msg.data;
 
}

ros::Subscriber<std_msgs::Int16> sub("period_ms", &messageCb );

void setup()
{
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{

  if(msg.data>= 0){  // argumentos recibidos se guardan en toggle

  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(msg.data);                     // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(msg.data);
  }
  nh.spinOnce();
  delay(1);
}
