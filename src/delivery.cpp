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

    robot.goStraight(1000);
    // robot.turnLeft();
    // robot.goStraight(100);
    // robot.turnRight();
 
    robot.defaultAction();
  }

  return 0;
}


/*
5층 엘레베이터 앞 방화셔터 턱을 중심점으로 ⇒ 512호 
회전이 근처에 들어가면 직진에서 50cm를 뺀다. 양쪽이면 100

robot.goStraight(418);
robot.turnRight();
robot.goStraight(668);
robot.turnLeft();
robot.turnRight();
robot.goStraight(280);
robot.turnRight();
robot.turnLeft();
robot.goStraight(271);

*/