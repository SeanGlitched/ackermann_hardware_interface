#include "ackermann_esp/esp_comms.hpp"
#include <sstream>
#include <cstdlib>

void ESPComms::setup(const std::string &serial_device, int32_t baud_rate, int32_t timeout_ms){
  serial_conn_.setPort(serial_device);
  serial_conn_.setBaudrate(baud_rate);
  serial::Timeout tt = serial::Timeout::simpleTimeout(timeout_ms);
  serial_conn_.setTimeout(tt); // This should be inline except setTimeout takes a reference and so needs a variable
  serial_conn_.open();
}

void ESPComms::sendEmptyMsg(){
      std::string response = sendMsg("\r");
}

void ESPComms::readEncoderValue(int &val_1){
  std::string response = sendMsg("e\r");

  std::string delimiter = " ";
  size_t del_pos = response.find(delimiter);
  std::string token_1 = response.substr(0, del_pos);
  std::string token_2 = response.substr(del_pos + delimiter.length());
  
  val_1 = std::atoi(token_1.c_str());
}
// void ESPComms::setMotorValues(int val_1, int val_2);

void ESPComms::setMotorValue(int val_1, int val_2){
  std::stringstream ss;
  ss << "m " << val_1  << " " << val_2 << "\r";
  sendMsg(ss.str(), false);
}
// void ESPComms::setPidValues(float k_p, float k_d, float k_i, float k_o);


std::string ESPComms::sendMsg(const std::string &msg_to_send, bool print_output){
  serial_conn_.write(msg_to_send);
  std::string response = serial_conn_.readline();

  if (print_output)
  {
      // RCLCPP_INFO_STREAM(logger_,"Sent: " << msg_to_send);
      // RCLCPP_INFO_STREAM(logger_,"Received: " << response);
  }

  return response;
}