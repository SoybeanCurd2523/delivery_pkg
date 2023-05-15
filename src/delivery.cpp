#include "read_yaml.h"
#include <time.h>

#define PI 3.141592
#define deg2rad PI/180
#define rad2deg 180/PI

#define T 100

YAML_CONFIG_READER XY_Coordinates_from_yaml; // 객체 선언

double left_rpm = 255; // ros topic data
double right_rpm = 0.0;
// double right_rpm = 255;

double x=0;
double y=0;

// void get_XY_data()
// {
  
//     XY_Coordinates_from_yaml.get_XY_Coordinates_From_yaml();

//     x = XY_Coordinates_from_yaml.get_x();

//     ROS_INFO("x : %lf", x);

//     y = XY_Coordinates_from_yaml.get_y();

//     ROS_INFO("y : %lf", y);
// }

// void go_straight(){
//   left_rpm = 255;
//   right_rpm = 255;
// }

// void turn_right(){
//   right_rpm = 255;
  
//   for(int t = 0 ; t<=5 ; t=t+0.1) // 주기 : 5초. time step : 0.1초
//     float degree = t*(PI/180);
//     left_rpm = 215 * (1+cos(degree* 2*PI/5)/2) +40
//     // rpm = fix_rpm*((1+cos(degree*2*PI/5))/2);
  
  
// }

// void turn_left(){

// }
void tof_callback(const std_msgs::Float64::ConstPtr& msg3)
{
  ROS_INFO("tof_data : [%lf] cm", msg3->data);
}

int main(int argc, char **argv)
{ 
  ros::init(argc, argv, "delivery");

  ros::NodeHandle nh;

  // get_XY_data();

  ros::Publisher left_pub = nh.advertise<std_msgs::Float64>("left_data", 1000); // 토픽 이름에 띄어쓰기 있으면 오류남
  ros::Publisher right_pub = nh.advertise<std_msgs::Float64>("right_data", 1000);
  ros::Subscriber tof_sub = nh.subscribe("tof_data", 1000, tof_callback);

  ros::Rate loop_rate(10); // 10Hz
  int i=0;
  while (ros::ok())
  {
    std_msgs::Float64 msg; // left_pub msg
    std_msgs::Float64 msg2; // right_pub msg

    msg.data = left_rpm;
    
    right_rpm = 215 * ( ( 1+cos(i*deg2rad * 2*PI / T) ) /2) + 40;
    // right_rpm = 80;
    msg2.data = right_rpm;

    ROS_INFO(" left_data: [%lf], right_data : [%lf]", msg.data, msg2.data);
    
    //  ROS_INFO("right_data : [%lf]", msg.data);
    
    left_pub.publish(msg); 
    right_pub.publish(msg2);

    ros::spinOnce();

    loop_rate.sleep();

    i=i+10; 
  }


  
  // switch (expression)
  // {
  // case 1: // 전진
  //   go_straight();
  //   break;

  // case 2: // 오른쪽 회전
  //   turn_right();
  //   break;

  // case 3: // 왼쪽 회전
  //   turn_left();
  //   break;

  // default;
  //   break;
  // }


  return 0;
}