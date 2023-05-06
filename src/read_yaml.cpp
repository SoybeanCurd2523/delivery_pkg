#include "read_yaml.h"

// 생성자 소멸자 넣어야됨  
// 안넣으면 delivery.cpp:(.text+0x8e1): undefined reference to `YAML_CONFIG_READER::YAML_CONFIG_READER()' 오류남

YAML_CONFIG_READER::YAML_CONFIG_READER(){ 

}

YAML_CONFIG_READER::~YAML_CONFIG_READER(){

}


void YAML_CONFIG_READER::get_XY_Coordinates_From_yaml(){
  std::string local_path(this->file_path); // 상대 경로
  std::string path = this->pkg_path+local_path; // 루트부터 패키지까지 + 상대경로 = 루트부터 끝까지 경로 : 절대경로
  YAML::Node doc = YAML::LoadFile(path);
  std::cout<<path<<std::endl;

    double x_distance;
    double y_distance;

  try {

      YAML::Node room_number = doc["room_512"];
      x_distance = room_number["x"].as<double>();
      y_distance = room_number["y"].as<double>();
    
      path_x.insert({"room_512", x_distance});
      path_y.insert({"room_512", y_distance});

  } catch (YAML::Exception &e) {
      std::cerr << "path_x_y YAML Exception: " << e.what() << std::endl;
      return;
  }
  std::ofstream fclose(file_path);
  return;
}

double YAML_CONFIG_READER::get_x(){ 
    return path_x["room_512"];
}

double YAML_CONFIG_READER::get_y(){ 
    return path_y["room_512"];
}
