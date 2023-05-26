#ifndef MOVE_ROBOT_H
#define MOVE_ROBOT_H

#include "ros/ros.h"
#include <std_msgs/Int32.h>
#include <std_msgs/Float64.h>

#define PI 3.141592
#define deg2rad PI/180
#define rad2deg 180/PI

#define LOOP_RATE 50
#define MAX_RPM 255
#define MIN_RPM 70

#define STRAIGHT_CYCLE 650 //100000
#define STRAIGHT_STEP 1 
#define STRAIGHT_LEFT_PRM 244

#define CURVE_CYCLE 1000
#define CURVE_CHANGE_STEP 2.5
#define CURVE_DEFAULT_STEP 2.9

#define DEFAULT_CYCLE 500
#define DEFAULT_STEP 2
#define DEFAULT_RPM 0

enum{
  go_straight, turn_left, turn_right // 0:직진, 1:좌회전,  2:우회전
};


class RobotController{
// private:
public:
    ros::NodeHandle nh_; // nh 아님. nh_임
    ros::Publisher left_pub;
    ros::Publisher right_pub;
    ros::Subscriber tof_sub;
    ros::Subscriber robot_status_sub;
    
    ros::Rate loop_rate;

    std_msgs::Float64 msg; // left_pub msg
    std_msgs::Float64 msg2; // right_pub msg

    int robot_status;
    double i,j;
    double left_rpm; // ros topic data
    double right_rpm;

    RobotController(ros::NodeHandle& nh); 
    ~RobotController();
    
    void tofCallback(const std_msgs::Float64::ConstPtr& msg3);
    void robotStatusCallback(const std_msgs::Int32::ConstPtr& msg); // 외부에서 로봇의 이동 명령 제어 가능
  
    void goStraight(double);
    void turnLeft();
    void turnRight();
    void defaultAction();

    void rpmTopicPublisher();

    void init();
};

#endif // MOVE_ROBOT_H