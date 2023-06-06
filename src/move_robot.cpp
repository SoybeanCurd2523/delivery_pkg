#include "move_robot.h"

PDController::PDController(ros::NodeHandle& nh) : nh_(nh), loop_rate(LOOP_RATE){
    
    x_dist_sub = nh_.subscribe("x_point_data", 1000, &PDController::x_point_callback, this); // 허프변환을 통한 라인들의 교차점의 x좌표
    rpm_control_signal_pub = nh_.advertise<std_msgs::Float64>("rpm_control_signal_data", 1000); // pd제어의 결과값이 더해진 우측 모터의 rpm
}

PDController::~PDController(){

}

void PDController::x_point_callback(const std_msgs::Float64::ConstPtr& msg)
{ 
    // current_value = msg->data;
//   ROS_INFO("current_value: %lf", current_value);
}

double PDController::calculate(double setpoint, double pv){
    // Calculate error
    error = setpoint - pv;

    // Proportional term
    P = Kp * error;

    // Derivative term
    derivative = (error - pre_error) * Kd;

    // Calculate total output
    output = P + derivative;

    // Restrict to max/min
    if (output < min) output = min;
    else if (output > max) output = max;
    
    // Save error to previous error
    pre_error = error;

    msg2.data = 200 + output;
    rpm_control_signal_pub.publish(msg2);   

    return output;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

RobotController::RobotController(ros::NodeHandle& nh) : PDController(nh), nh_(nh), loop_rate(LOOP_RATE){
    robot_status = -1;
    left_pub = nh_.advertise<std_msgs::Float64>("left_data", 1000);
    right_pub = nh_.advertise<std_msgs::Float64>("right_data", 1000);
    tof_sub = nh_.subscribe("tof_data", 1000, &RobotController::tofCallback, this);
    robot_status_sub = nh_.subscribe("robot_status_data", 1000, &RobotController::robotStatusCallback, this);

    imu_sub = nh_.subscribe("/camera/imu", 1000, &RobotController::imuCallback, this);
    left_encoder_sub = nh_.subscribe("left_encoder_diff_data", 1000, &RobotController::leftEncoderDiffCallback, this);
    right_encoder_sub = nh_.subscribe("right_encoder_diff_data", 1000, &RobotController::rightEncoderDiffCallback, this);
}

RobotController::~RobotController(){

}

void RobotController::imuCallback(const sensor_msgs::Imu::ConstPtr& msg)
{
    th = msg->angular_velocity.y*cos(12/180*PI);
    degree_rad = th * dt;
    degree += degree_rad * 180.0 / M_PI / 2.0;
}

void RobotController::leftEncoderDiffCallback(const std_msgs::Float64::ConstPtr& msg4)
{
    left_encoder_diff = msg4->data;
}

void RobotController::rightEncoderDiffCallback(const std_msgs::Float64::ConstPtr& msg5)
{
    right_encoder_diff = msg5->data;
}

void RobotController::tofCallback(const std_msgs::Float64::ConstPtr& msg3)
{
    ROS_INFO("tof_data : [%lf] cm", msg3->data);
    current_value = msg3->data;
}

void RobotController::robotStatusCallback(const std_msgs::Int32::ConstPtr& msg) // 외부에서 로봇의 이동 명령 제어 가능
{
  robot_status = msg->data;
  ROS_INFO("robot_status : [%d] ", msg->data);
}

void RobotController::goStraight(double distance){
    robot_status = go_straight;

    // for(int i=0 ; i< STRAIGHT_CYCLE*distance/100 ; i+=STRAIGHT_STEP){
    while(1){
        ros::spinOnce();
        control_signal = calculate(setpoint, current_value);
        // ROS_INFO("current value : %f", current_value);

        // ROS_INFO("control_signal : %lf", control_signal);

        left_rpm = 220; //STRAIGHT_LEFT_PRM ; // 204
        right_rpm = 200 + control_signal;

        ROS_INFO("right_rpm : %lf", right_rpm);

        // ROS_INFO("go_straight");
        // ROS_INFO("i : %d", i); 

        rpmTopicPublisher();
        loop_rate.sleep();

    }
}

void RobotController::turnLeft(){
    robot_status = turn_left;

    for(double i=0 ; i<CURVE_CYCLE/2; i+=CURVE_CHANGE_STEP_L){
        left_rpm = (MAX_RPM - MIN_RPM) * ( ( 1+cos(i* 2*PI / CURVE_CYCLE) ) /2) + MIN_RPM;
        right_rpm = MAX_RPM;

        ROS_INFO("turn_left");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);          

        rpmTopicPublisher();
        loop_rate.sleep();
    }

    for(double i=CURVE_CYCLE/2 ; i<3*CURVE_CYCLE/2; i+=CURVE_DEFAULT_STEP_L){
        left_rpm = MIN_RPM;
        right_rpm = MAX_RPM;
        
        ROS_INFO("turn_left");
        ROS_INFO("left_rpm : %lf, right_rpm : %lf", left_rpm, right_rpm);

        rpmTopicPublisher();
        loop_rate.sleep();

    }

    for(double i=3*CURVE_CYCLE/2 ; i<=2*CURVE_CYCLE; i+=CURVE_CHANGE_STEP_L){
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
        ROS_INFO("i : %f", i);

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



