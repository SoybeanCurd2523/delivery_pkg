#include "read_yaml.h"

YAML_CONFIG_READER::YAML_CONFIG_READER(){

}

YAML_CONFIG_READER::~YAML_CONFIG_READER(){

}

void YAML_CONFIG_READER::getJoint_PD_gainFrom_yaml(){
  std::string local_path(this->file_path); // 상대 경로
  std::string path = this->pkg_path+local_path; // 루트부터 패키지까지 + 상대경로 = 루트부터 끝까지 경로 : 절대경로
  YAML::Node doc = YAML::LoadFile(path);
  std::cout<<path<<std::endl;
  double Pgain;
  double Dgain;
  try {

        apply_for_all_Kp = doc["apply_for_all_Kp"].as<double>();
        apply_for_all_Kd = doc["apply_for_all_Kd"].as<double>();


        YAML::Node each_joint = doc["hip_yaw"];
        Pgain=each_joint["Kp"].as<double>();
        Dgain=each_joint["Kd"].as<double>();
        Pgain *= apply_for_all_Kp;
        Dgain *= apply_for_all_Kd;
        jointP_gain.insert({"hip_yaw", Pgain});
        jointD_gain.insert({"hip_yaw", Dgain});

        each_joint = doc["hip_roll"];
        Pgain=each_joint["Kp"].as<double>();
        Dgain=each_joint["Kd"].as<double>();
        Pgain *= apply_for_all_Kp;
        Dgain *= apply_for_all_Kd;        
        jointP_gain.insert({"hip_roll", Pgain});
        jointD_gain.insert({"hip_pitch", Dgain});

        each_joint = doc["knee"];
        Pgain=each_joint["Kp"].as<double>();
        Dgain=each_joint["Kd"].as<double>();
        Pgain *= apply_for_all_Kp;
        Dgain *= apply_for_all_Kd;
        jointP_gain.insert({"knee", Pgain});
        jointD_gain.insert({"knee", Dgain});

        each_joint = doc["ankle_pitch"];
        Pgain=each_joint["Kp"].as<double>();
        Dgain=each_joint["Kd"].as<double>();
        Pgain *= apply_for_all_Kp;
        Dgain *= apply_for_all_Kd;
        jointP_gain.insert({"ankle_pitch", Pgain});
        jointD_gain.insert({"ankle_pitch", Dgain});

        each_joint = doc["ankle_roll"];
        Pgain=each_joint["Kp"].as<double>();
        Dgain=each_joint["Kd"].as<double>();
        Pgain *= apply_for_all_Kp;
        Dgain *= apply_for_all_Kd;
        jointP_gain.insert({"ankle_roll", Pgain});
        jointD_gain.insert({"ankle_roll", Dgain});



  } catch (YAML::Exception &e) {
        std::cerr << "joint_PDgain YAML Exception: " << e.what() << std::endl;
        return;
  }
  std::ofstream fclose(file_path);
  return;
}

double YAML_CONFIG_READER::get_Kp(int joint_num){ //LHY, LHR, LHP, LKN, LAP, LAR, RHY, RHR, RHP, RKN, RAP, RAR
  if(joint_num == LHY or joint_num == RHY){
    return jointP_gain["hip_yaw"];
  }
  else if(joint_num == LHR or joint_num == RHR){
    return jointP_gain["hip_roll"];
  }
  else if(joint_num == LHP or joint_num == RHP){
    return jointP_gain["hip_yaw"];
  }
  else if(joint_num == LKN or joint_num == RKN){
    return jointP_gain["knee"];
  }
  else if(joint_num == LAP or joint_num == RAP){
    return jointP_gain["ankle_pitch"];
  }
  else if(joint_num == LAR or joint_num == RAR){
    return jointP_gain["ankle_roll"];
  }
}

double YAML_CONFIG_READER::get_Kd(int joint_num){
  if(joint_num == LHY or joint_num == RHY){
    return jointD_gain["hip_yaw"];
  }
  else if(joint_num == LHR or joint_num == RHR){
    return jointD_gain["hip_roll"];
  }
  else if(joint_num == LHP or joint_num == RHP){
    return jointD_gain["hip_yaw"];
  }
  else if(joint_num == LKN or joint_num == RKN){
    return jointD_gain["knee"];
  }
  else if(joint_num == LAP or joint_num == RAP){
    return jointD_gain["ankle_pitch"];
  }
  else if(joint_num == LAR or joint_num == RAR){
    return jointD_gain["ankle_roll"];
  }
}