#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"

#include <sstream>

int main(int argc, char **argv)
{

  ros::init(argc, argv, "talker");


  ros::NodeHandle n;
  std::vector<int>   ruedas;
  std_msgs::Float32MultiArray msg_mov;

  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("/hardware/mobile base/speeds", 1000);

  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
  {
    ruedas.resize(2);;

    //std::stringstream ss;
    //ss << "hello world " << count;
    //msg.data = ss.str();
    ruedas[0]= 0.02; 
    ruedas[1]= 0.3;

    msg_sonars.data[i]

    //ROS_INFO("%s", msg.data.c_str());
    chatter_pub.publish(std_msgs);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}
