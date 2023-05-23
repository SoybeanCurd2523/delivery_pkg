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

// 실험1
// # 1. 직진 418cm -> 368
// # 2, 오른쪽으로 90도 회전
// # 3. 직진 718cm -> 618
// # 4. 왼쪽으로 90도 회전
// # 5. 직진 89cm -> 39

// 실험2
// # 3. 직진 718cm // 668
// # 4. 왼쪽으로 90도 회전
// # 6. 오른쪽으로 회전
// # 7. 직진 380cm // 280
// # 8. 오른쪽으로 회전
// # 10. 왼쪽으로 회전
// # 11. 직진 321cm // 271
// 문제점 : 로봇이 왼쪽으로 좀 휜다.
// 그리고 양 바퀴 고정이 달라서, 좌회전과 우회전의 step을 다르게 해야겠다.
// 배터리 풀충.
// 왼쪽 모터 실리콘 재결합?

    robot.defaultAction();
    robot.goStraight(668);
    robot.turnLeft();
    robot.turnRight();
    robot.goStraight(280);
    robot.turnRight();
    robot.turnLeft();
    robot.goStraight(271);
    robot.defaultAction();

    flag = false;

    if(flag) break;

  }

  return 0;
}
