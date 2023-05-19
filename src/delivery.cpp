#include "move_robot.h"
// #include "read_yaml.h"

int main(int argc, char **argv)
{ 
  ros::init(argc, argv, "delivery");

  ros::NodeHandle nh;

  RobotController robot(nh);
  // YAML_CONFIG_READER XY_Coordinates_from_yaml; // 객체 선언

  bool flag = true;

  while (ros::ok() && flag)
  {
    ros::spinOnce();

    robot.goStraight();
    robot.turnLeft();
    robot.turnRight();

    flag = false;

    if(flag) break;

  }

  return 0;
}
