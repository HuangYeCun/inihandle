#include <iostream>

#include "common.h"
#include "inihandle.h"

using namespace std;

int main() {
  cout << "-------- INI File Test --------" << endl;

  Common comm;
  std::string current_dir = comm.GetCurrentDir();

  std::string filename = current_dir + "/inibook.ini";

  IniHandle ini_handle(filename);
  std::vector<IniNode> vec;
  vec = ini_handle.readINI();
  std::cout << "原始数据：" << std::endl;
  for (std::size_t i = 0; i != vec.size(); ++i) {
    std::cout << "[" << vec[i].get_group() << "] " << vec[i].get_key() << "="
              << vec[i].get_value() << std::endl;
  }

  std::cout << " ----------- 设置INI文件--------------- " << std::endl;
  //第一种设置方式
  ini_handle.beginGroup("Book");
  ini_handle.setValue("Name", "DDDDD");
  ini_handle.setValue("Line", "11");
  ini_handle.setValue("Test1", "11");
  ini_handle.setValue("Test2", "11");
  ini_handle.endGroup();

  ini_handle.beginGroup("Person");
  ini_handle.setValue("Sex", "Boy");
  ini_handle.setValue("School", "Beiwai");
  ini_handle.endGroup();

  ini_handle.beginGroup("Dog");
  ini_handle.setValue("Name", "xiaohei");
  ini_handle.setValue("Color", "black");
  ini_handle.endGroup();

  ini_handle.beginGroup("Car");
  ini_handle.setValue("Name", "tesla");
  ini_handle.setValue("Model", "Model S");
  ini_handle.endGroup();

  ini_handle.beginGroup("Car");
  ini_handle.setValue("Model", "Model X");
  ini_handle.setValue("Color", "Black ");
  ini_handle.setValue("Test", "1 ");
  ini_handle.endGroup();

  ini_handle.beginGroup("Dog");
  ini_handle.setValue("Name", "Beifang");
  ini_handle.setValue("Type", "tianyuan");
  ini_handle.endGroup();

  //第二种设置方式
  //  ini_handle.setValue("Book/Name", "DDDDD");
  //  ini_handle.setValue("Book/Line", "11");
  //  ini_handle.setValue("Person/Sex", "Boy");
  //  ini_handle.setValue("Person/School", "Beiwai");

  ini_handle.setValue("Dog/Name", "xiaohei");

  vec = ini_handle.readINI();
  std::cout << "最终数据：" << std::endl;
  for (std::size_t i = 0; i != vec.size(); ++i) {
    std::cout << "[" << vec[i].get_group() << "] " << vec[i].get_key() << "="
              << vec[i].get_value() << std::endl;
  }

  //  std::cout << " ----------- 获取INI文件中的值 --------------- " <<
  //  std::endl;
  //  //获取INI文件中的值
  //  std::string value = ini_handle.value("Book/Title");
  //  std::cout << "Book/Title: " << value << std::endl;

  //  ini_handle.beginGroup("Book");
  //  std::cout << "current group: " << ini_handle.group() << std::endl;
  //  std::cout << "Title : " << ini_handle.value("Title") << std::endl;
  //  ini_handle.endGroup();

  //  std::cout << "Car/Model: " << ini_handle.value("Car/Model") << std::endl;

  return 0;
}
