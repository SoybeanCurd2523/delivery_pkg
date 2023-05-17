#include "read_yaml.h"

#define PI 3.141592
#define deg2rad PI/180
#define rad2deg 180/PI
#define T 1000

YAML_CONFIG_READER XY_Coordinates_from_yaml; // 객체 선언

enum{
  go_straight, turn_left, turn_right
};

int robot_status = -1;
double i,j;
double left_rpm = 0; // ros topic data
double right_rpm = 0;

void tof_callback(const std_msgs::Float64::ConstPtr& msg3)
{
  ROS_INFO("tof_data : [%lf] cm", msg3->data);
}

int main(int argc, char **argv)
{ 
  ros::init(argc, argv, "delivery");

  ros::NodeHandle nh;

  ros::Publisher left_pub = nh.advertise<std_msgs::Float64>("left_data", 1000); // 토픽 이름에 띄어쓰기 있으면 오류남
  ros::Publisher right_pub = nh.advertise<std_msgs::Float64>("right_data", 1000);
  ros::Subscriber tof_sub = nh.subscribe("tof_data", 1000, tof_callback);
  
  std_msgs::Float64 msg; // left_pub msg
  std_msgs::Float64 msg2; // right_pub msg

  ros::Rate loop_rate(100); // 10Hz

  while (ros::ok())
  {
    robot_status = turn_left;
    
    switch (robot_status)
    {
      case go_straight:
      {
        left_rpm = right_rpm = 255;
        ROS_INFO("go_straight");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

        msg.data = left_rpm;
        msg2.data = right_rpm;
        left_pub.publish(msg); 
        right_pub.publish(msg2);
        loop_rate.sleep();
        break;
      }
        
      
      case turn_left:
      {
        for(double i=0 ;  i<T/2; i+=0.2){
          left_rpm = 185 * ( ( 1+cos(i* 2*PI / T) ) /2) + 70;
          right_rpm = 255;
          
          ROS_INFO("turn_left");
          ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

          msg.data = left_rpm;
          msg2.data = right_rpm;
          left_pub.publish(msg); 
          right_pub.publish(msg2);
          loop_rate.sleep();

        }
        for(double i=T/2 ;  i<3*T/2; i+=0.2){
          left_rpm = 70;
          right_rpm = 255;
          
          ROS_INFO("turn_left");
          ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

          msg.data = left_rpm;
          msg2.data = right_rpm;
          left_pub.publish(msg); 
          right_pub.publish(msg2);
          loop_rate.sleep();

        }
        for(double i=3*T/2 ;  i<=2*T; i+=0.2){
          left_rpm = 185 * ( ( 1+cos(i* 2*PI / T) ) /2) + 70;
          right_rpm = 255;
          
          ROS_INFO("turn_left");
          ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

          msg.data = left_rpm;
          msg2.data = right_rpm;
          left_pub.publish(msg); 
          right_pub.publish(msg2);
          loop_rate.sleep();

        }
        break;
      }
        

      case turn_right:
      {
        for(double j=0 ;  j<=T ; j++){
          left_rpm = 255;
          right_rpm = 215 * ( ( 1+cos(j * 2*PI / T) ) /2) + 40;
          
          ROS_INFO("turn_right");
          ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

          msg.data = left_rpm;
          msg2.data = right_rpm;
          left_pub.publish(msg); 
          right_pub.publish(msg2);
          loop_rate.sleep();
        }
        break;
      }
    }
    
    ros::spinOnce();
    
  }

  return 0;
}