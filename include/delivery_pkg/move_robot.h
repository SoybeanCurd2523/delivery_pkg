#ifndef MOVE_ROBOT_H
#define MOVE_ROBOT_H

#include "ros/ros.h"
#include <std_msgs/Int32.h>
#include <std_msgs/Float64.h>
#include <sensor_msgs/Imu.h>

#define PI 3.141592
#define deg2rad PI/180
#define rad2deg 180/PI

#define LOOP_RATE 100 // Hz
#define MAX_RPM 200
#define MIN_RPM 70

#define STRAIGHT_CYCLE 749 // 650 //100000
#define STRAIGHT_STEP 1 
#define STRAIGHT_LEFT_PRM 206

#define CURVE_CYCLE 1000
#define CURVE_CHANGE_STEP 2.5
#define CURVE_DEFAULT_STEP 2.9

#define DEFAULT_CYCLE 500
#define DEFAULT_STEP 2
#define DEFAULT_RPM 0

enum{
  go_straight, turn_left, turn_right // 0:직진, 1:좌회전,  2:우회전
};

// #define DELTA_S_GAIN 0.169756
class PDController
{
// private:
public:
    ros::NodeHandle nh_; // nh 아님. nh_임
    ros::Subscriber x_dist_sub;
    ros::Publisher rpm_control_signal_pub;

    ros::Rate loop_rate;
    std_msgs::Float64 msg; // x_dist_sub msg
    std_msgs::Float64 msg2; // control_signal_pub

    // Set control gains
    double Kp = 1.1;  // P gain 값
    double Kd = Kp*0.05; // D gain 값
    double max = 55;  // 최대 출력 값
    double min = -55;    // 최소 출력 값

    double setpoint = 640.0;  // 목표값, 이미지 픽셀의 중심 640, 우측 통행 시 벽부터 왼쪽 바퀴 50cm 기준으로는 660 
    double current_value  = 0.0;  // 현재 값, hough transform으로 검출한 교차점의 x 좌표
    double control_signal = 0.0; // PD control 결과 값  

    double P;
    double derivative;
    double output;
    double error;
    double pre_error;

    PDController(ros::NodeHandle& nh);
    ~PDController();

    void x_point_callback(const std_msgs::Float64::ConstPtr& msg);

    double calculate(double setpoint, double pv);
};

// RobotController 클래스에서 PDController 클래스를 상속받음
// RobotController 클래스의 생성자에서 PDController 클래스의 생성자를 명시적으로 호출해야 합니다
// RobotController::RobotController(ros::NodeHandle& nh) : PDController(nh), nh_(nh), loop_rate(LOOP_RATE)

class RobotController : public PDController {
// private:
public:
    ros::NodeHandle nh_; // nh 아님. nh_임
    ros::Publisher left_pub;
    ros::Publisher right_pub;
    ros::Subscriber tof_sub;
    ros::Subscriber robot_status_sub;
    
    ros::Subscriber imu_sub;
    ros::Subscriber left_encoder_sub;
    ros::Subscriber right_encoder_sub;

    ros::Rate loop_rate;

    std_msgs::Float64 msg; // left_pub msg
    std_msgs::Float64 msg2; // right_pub msg

    int robot_status;
    double i,j;
    double left_rpm; // ros topic data
    double right_rpm;
    
    float dt = 0.01; // 10ms
    double th, left_encoder_diff, right_encoder_diff;
    double prev_left_encoder_diff, prev_right_encoder_diff, prev_imu_th, degree, degree_rad;
    float delta_s, delta_x, delta_y;
    float x, y, s;

    RobotController(ros::NodeHandle& nh); 
    ~RobotController();
    
    void tofCallback(const std_msgs::Float64::ConstPtr& msg3);
    void robotStatusCallback(const std_msgs::Int32::ConstPtr& msg); // 외부에서 로봇의 이동 명령 제어 가능


    void imuCallback(const sensor_msgs::Imu::ConstPtr& msg);
    void leftEncoderDiffCallback(const std_msgs::Float64::ConstPtr& msg4);
    void rightEncoderDiffCallback(const std_msgs::Float64::ConstPtr& msg5);



    void goStraight(double);
    void turnLeft();
    void turnRight();
    void defaultAction();

    void rpmTopicPublisher();
};

#endif // MOVE_ROBOT_H
