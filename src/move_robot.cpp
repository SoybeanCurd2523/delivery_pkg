#include "move_robot.h"

RobotController::RobotController(ros::NodeHandle& nh) : nh_(nh), loop_rate(100){
    robot_status = -1;
    left_pub = nh_.advertise<std_msgs::Float64>("left_data", 1000);
    right_pub = nh_.advertise<std_msgs::Float64>("right_data", 1000);
    tof_sub = nh_.subscribe("tof_data", 1000, &RobotController::tofCallback, this);
    robot_status_sub = nh_.subscribe("robot_status_data", 1000, &RobotController::robotStatusCallback, this);
    
}

RobotController::~RobotController(){

}

void RobotController::tofCallback(const std_msgs::Float64::ConstPtr& msg3)
{
 // ROS_INFO("tof_data : [%lf] cm", msg3->data);
}

void RobotController::robotStatusCallback(const std_msgs::Int32::ConstPtr& msg) // 외부에서 로봇의 이동 명령 제어 가능
{
  robot_status = msg->data;
  ROS_INFO("robot_status : [%d] ", msg->data);
}


void RobotController::goStraight(){
    robot_status = go_straight;

    for(int i=0 ; i< TT ; i++){
        left_rpm = 247; // MAX_RPM - 8
        right_rpm = MAX_RPM;
        ROS_INFO("go_straight");
        ROS_INFO("i : %d", i); 

        rpmTopicPublisher();
        loop_rate.sleep();
    }
}

void RobotController::turnLeft(){
    robot_status = turn_left;

    for(double i=0 ;  i<T/2; i+=0.4){
        left_rpm = (MAX_RPM - MIN_RPM) * ( ( 1+cos(i* 2*PI / T) ) /2) + MIN_RPM;
        right_rpm = MAX_RPM;

        ROS_INFO("turn_left");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);          

        rpmTopicPublisher();
        loop_rate.sleep();
    }

    for(double i=T/2 ;  i<3*T/2; i+=0.8){
        left_rpm = MIN_RPM;
        right_rpm = MAX_RPM;
        
        ROS_INFO("turn_left");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

        rpmTopicPublisher();
        loop_rate.sleep();

    }

    for(double i=3*T/2 ;  i<=2*T; i+=0.4){
        left_rpm = (MAX_RPM - MIN_RPM) * ( ( 1+cos(i* 2*PI / T) ) /2) + MIN_RPM;
        right_rpm = MAX_RPM;
        
        ROS_INFO("turn_left");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

        rpmTopicPublisher();
        loop_rate.sleep();

    }
}

void RobotController::turnRight(){
    robot_status = turn_right;

    for(double j=0 ;  j<T/2; j+=0.4){
        left_rpm = MAX_RPM;
        right_rpm = (MAX_RPM - MIN_RPM) * ( ( 1+cos(j* 2*PI / T) ) /2) + MIN_RPM;

        ROS_INFO("turn_right");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

        rpmTopicPublisher();
        loop_rate.sleep();

    }

    for(double j=T/2 ;  j<3*T/2; j+=0.8){
        left_rpm = MAX_RPM;
        right_rpm = MIN_RPM;

        ROS_INFO("turn_right");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

        rpmTopicPublisher();
        loop_rate.sleep();

    }

    for(double j=3*T/2 ;  j<=2*T; j+=0.4){
        left_rpm = MAX_RPM;
        right_rpm = (MAX_RPM - MIN_RPM) * ( ( 1+cos(j* 2*PI / T) ) /2) + MIN_RPM;
        
        ROS_INFO("turn_right");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

        rpmTopicPublisher();
        loop_rate.sleep();

    }
}

void RobotController::defaultAction(){
    for(int i=0 ; i< 1000; i++){
        left_rpm = 0;
        right_rpm = 0;
        ROS_INFO("defaultAction");

        rpmTopicPublisher();
        loop_rate.sleep(); 
    }
}

void RobotController::rpmTopicPublisher( ){
        
    msg.data = left_rpm;
    msg2.data = right_rpm;
    left_pub.publish(msg); 
    right_pub.publish(msg2);
}