#include "move_robot.h"

RobotController::RobotController(ros::NodeHandle& nh) : nh_(nh), loop_rate(LOOP_RATE){
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

    for(int i=0 ; i< STRAIGHT_CYCLE ; i+=STRAIGHT_STEP){
        left_rpm = STRAIGHT_LEFT_PRM; // no 255
        right_rpm = MAX_RPM;
        ROS_INFO("go_straight");
        ROS_INFO("i : %d", i); 

        rpmTopicPublisher();
        loop_rate.sleep();
    }
}

void RobotController::turnLeft(){
    robot_status = turn_left;

    for(double i=0 ; i<CURVE_CYCLE/2; i+=CURVE_CHANGE_STEP){
        left_rpm = (MAX_RPM - MIN_RPM) * ( ( 1+cos(i* 2*PI / CURVE_CYCLE) ) /2) + MIN_RPM;
        right_rpm = MAX_RPM;

        ROS_INFO("turn_left");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);          

        rpmTopicPublisher();
        loop_rate.sleep();
    }

    for(double i=CURVE_CYCLE/2 ; i<3*CURVE_CYCLE/2; i+=CURVE_DEFAULT_STEP){
        left_rpm = MIN_RPM;
        right_rpm = MAX_RPM;
        
        ROS_INFO("turn_left");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

        rpmTopicPublisher();
        loop_rate.sleep();

    }

    for(double i=3*CURVE_CYCLE/2 ; i<=2*CURVE_CYCLE; i+=CURVE_CHANGE_STEP){
        left_rpm = (MAX_RPM - MIN_RPM) * ( ( 1+cos(i* 2*PI / CURVE_CYCLE) ) /2) + MIN_RPM;
        right_rpm = MAX_RPM;
        
        ROS_INFO("turn_left");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

        rpmTopicPublisher();
        loop_rate.sleep();

    }
}

void RobotController::turnRight(){
    robot_status = turn_right;

    for(double i=0 ; i<CURVE_CYCLE/2; i+=CURVE_CHANGE_STEP){
        left_rpm = MAX_RPM;
        right_rpm = (MAX_RPM - MIN_RPM) * ( ( 1+cos(i* 2*PI / CURVE_CYCLE) ) /2) + MIN_RPM;

        ROS_INFO("turn_right");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

        rpmTopicPublisher();
        loop_rate.sleep();

    }

    for(double i=CURVE_CYCLE/2 ; i<3*CURVE_CYCLE/2; i+=CURVE_DEFAULT_STEP){
        left_rpm = MAX_RPM;
        right_rpm = MIN_RPM;

        ROS_INFO("turn_right");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

        rpmTopicPublisher();
        loop_rate.sleep();

    }

    for(double i=3*CURVE_CYCLE/2 ; i<=2*CURVE_CYCLE; i+=CURVE_CHANGE_STEP){
        left_rpm = MAX_RPM;
        right_rpm = (MAX_RPM - MIN_RPM) * ( ( 1+cos(i* 2*PI / CURVE_CYCLE) ) /2) + MIN_RPM;
        
        ROS_INFO("turn_right");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

        rpmTopicPublisher();
        loop_rate.sleep();

    }
}

void RobotController::defaultAction(){
    for(double i=0 ; i< DEFAULT_CYCLE; i+=DEFAULT_STEP){
        left_rpm = DEFAULT_RPM;
        right_rpm = DEFAULT_RPM;
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