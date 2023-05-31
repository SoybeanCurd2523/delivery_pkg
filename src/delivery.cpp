#include "move_robot.h"
#include "read_yaml.h"

char room[] = "room_512"; // x와 y 좌표를 yaml파일에서 가져오고 싶은 방 호수

int main(int argc, char **argv)
{ 
  ros::init(argc, argv, "delivery");

  ros::NodeHandle nh;

  RobotController robot(nh);
  PDController pd(nh);
  YAML_CONFIG_READER yaml;
  
  yaml.get_XY_Coordinates_From_yaml(room);
  double room_x = yaml.get_x(room);
  double room_y = yaml.get_y(room);

  while (ros::ok())
  {
    // ros::spinOnce();

    robot.defaultAction();
    // robot.goStraight(100);

    // robot.turnLeft();
    robot.turnRight();
 
    robot.defaultAction();
  }

  return 0;
}
