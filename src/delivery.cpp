#include "read_yaml.h"

YAML_CONFIG_READER PD_gain_from_yaml; // 객체 선언

double a = 0;
double b = 0;
void SetJointPIDgain()
{
  
    PD_gain_from_yaml.getJoint_PD_gainFrom_yaml();

    a = PD_gain_from_yaml.get_Kp(LHY);

    ROS_INFO("a : %lf", a);

    b = PD_gain_from_yaml.get_Kd(LHY);

    ROS_INFO("b : %lf", b);
}


int main(int argc, char **argv)
{ 

  ros::init(argc, argv, "delivery");

  ros::NodeHandle nh;

  SetJointPIDgain();

  ros::Publisher chatter_pub = nh.advertise<std_msgs::Float64>("mytopic", 1000);

  ros::Rate loop_rate(1);

  while (ros::ok())
  {
    std_msgs::Float64 msg;
    msg.data = a;
    ROS_INFO("I speak : [%lf]", msg.data);
    chatter_pub.publish(msg); 
    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}