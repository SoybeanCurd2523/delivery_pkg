#include "read_yaml.h"

YAML_CONFIG_READER PD_gain_from_yaml; // 객체 선언

double x = 0.0;
double y = 0.0;

void SetJointPIDgain()
{
  
    PD_gain_from_yaml.getJoint_PD_gainFrom_yaml();

    x = PD_gain_from_yaml.get_x();

    ROS_INFO("x : %lf", x);

    y = PD_gain_from_yaml.get_y();

    ROS_INFO("y : %lf", y);
}


int main(int argc, char **argv)
{ 

  ros::init(argc, argv, "delivery");

  ros::NodeHandle nh;

  SetJointPIDgain();

  ros::Publisher x_pub = nh.advertise<std_msgs::Float64>("x_data", 1000); // 토픽 이름에 띄어쓰기 있으면 오류남
  ros::Publisher y_pub = nh.advertise<std_msgs::Float64>("y_data", 1000);

  ros::Rate loop_rate(1);

  while (ros::ok())
  {
    std_msgs::Float64 msg;
    std_msgs::Float64 msg2;

    msg.data = x;
    msg2.data = y;
    ROS_INFO("x : [%lf], y : [%lf]", msg.data, msg2.data);
    
    x_pub.publish(msg); 
    y_pub.publish(msg2);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}