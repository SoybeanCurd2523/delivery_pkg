#include "move_robot.h"
#include "read_yaml.h"

// Set control gains
double Kp = 0.8;  // P gain 값
double Kd = Kp*0.05; // D gain 값
double maxOutput = 55;  // 최대 출력 값
double minOutput = -55;    // 최소 출력 값

double setpoint = 640.0;  // 목표값
double current_value  = 0.0;  // 현재 값
double control_signal = 0.0; // PD control 결과 값

float x_point_; // hough transform으로 검출한 교차점의 x 좌표

char room[] = "room_512"; // x와 y 좌표를 yaml파일에서 가져오고 싶은 방 호수

void x_point_callback(const std_msgs::Float64::ConstPtr& msg)
{ 
  x_point_ = msg->data;
  current_value = x_point_;
  ROS_INFO("x_point_: %f", x_point_);
}

int main(int argc, char **argv)
{ 
  ros::init(argc, argv, "delivery");

  ros::NodeHandle nh;
  ros::Subscriber x_dist_sub = nh.subscribe("x_point_data", 1000, x_point_callback);

  RobotController robot(nh);
  PDController pd(Kp, Kd, maxOutput, minOutput);
  YAML_CONFIG_READER yaml;
  
  yaml.get_XY_Coordinates_From_yaml(room);
  double x = yaml.get_x(room);
  double y = yaml.get_y(room);

  while (ros::ok())
  {
    ros::spinOnce();

    control_signal = pd.calculate(setpoint, current_value);
    ROS_INFO("x : %lf, y : %lf", x, y);

    robot.goStraight(668, control_signal);

  }

  return 0;
}
