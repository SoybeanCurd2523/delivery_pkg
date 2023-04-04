#ifndef READ_YAML_H
#define READ_YAML_H

#include "ros/ros.h"
#include <ros/package.h>

#include <map>
#include <fstream>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <string>


#include <std_msgs/String.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float64.h>

#define JOINT_PD_GAIN_FILEPATH "/config/pd_gain.yaml"

enum
{
    WST = 0, LHY, LHR, LHP, LKN, LAP, LAR, RHY, RHR, RHP, RKN, RAP, RAR
};

class YAML_CONFIG_READER
{
private:
  double apply_for_all_Kp;
  double apply_for_all_Kd;

  std::map<std::string, double> jointP_gain; //name:P gain   // Key : string형, value : double형
  std::map<std::string, double> jointD_gain; //name:D gain

  const char *file_path = JOINT_PD_GAIN_FILEPATH;
  const std::string pkg_path = ros::package::getPath("delivery_pkg"); // 루트부터 패키지까지의 경로


public:

  YAML_CONFIG_READER();
  ~YAML_CONFIG_READER();
  void getJoint_PD_gainFrom_yaml();
  double get_Kp(int joint_num);
  double get_Kd(int joint_num);




};

#endif // READ_YAML_H