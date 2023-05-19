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

#define XY_COORDINATES_FILEPATH "/config/xy_coordinates.yaml"
// const char* const XY_COORDINATES_FILEPATH = "/config/xy_coordinates.yaml";


class YAML_CONFIG_READER
{
private:

  std::map<std::string, double> path_x; // Key : string형, value : double형  x: 538.6
  std::map<std::string, double> path_y; 

  const char *file_path = XY_COORDINATES_FILEPATH;
  const std::string pkg_path = ros::package::getPath("delivery_pkg"); // 루트부터 패키지까지의 경로 #include <ros/package.h> 해야 ros::package() 사용가능


public:

  YAML_CONFIG_READER();
  ~YAML_CONFIG_READER();
  void get_XY_Coordinates_From_yaml();

  double get_x();
  double get_y();

};

#endif // READ_YAML_H