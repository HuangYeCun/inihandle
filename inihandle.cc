#include "inihandle.h"

#include <fstream>

IniHandle::IniHandle(const std::string& filename)
    : filename_(filename), group_("") {}

IniHandle::~IniHandle() {}

std::vector<IniNode> IniHandle::readINI() const {
  std::ifstream ifile;
  ifile.open(filename_);

  std::vector<IniNode> ini_vec;
  std::string root_name("");
  std::string line_data("");
  while (getline(ifile, line_data)) {
    if (line_data.npos != line_data.find("[") &&
        line_data.npos != line_data.find("]")) {
      std::size_t left_pos = line_data.find("[");
      std::size_t right_pos = line_data.find("]");
      root_name = line_data.substr(left_pos + 1, right_pos - 1);
    }

    std::string key("");
    std::string value("");
    if (line_data.npos != line_data.find("=")) {
      std::size_t equal_sign_pos = line_data.find("=");
      key = line_data.substr(0, equal_sign_pos);
      value = line_data.substr(equal_sign_pos + 1);
    }

    if (!root_name.empty() && !key.empty() && !value.empty()) {
      IniNode ini_node(root_name, key, value);
      ini_vec.push_back(ini_node);
    }
  }
  ifile.close();

  return ini_vec;
}

void IniHandle::beginGroup(const std::string& prefix) {
  if (group_.empty()) {
    group_ = prefix;
  } else {
    group_ += "/";
    group_ += prefix;
  }
}

void IniHandle::endGroup() {
  if (group_.empty()) {
    return;
  }
  std::string::size_type last_left_div = group_.find_last_of("/");
  if (std::string::npos != last_left_div)
    group_ = group_.substr(0, last_left_div);
  else
    group_ = "";

  writeINI();
}

std::string IniHandle::group() const { return group_; }

void IniHandle::setValue(const std::string& key, const std::string& value) {
  std::string prefix = group_;
  std::string::size_type first_left_div = key.find_first_of("/");
  if (key.npos != first_left_div) {
    prefix = key.substr(0, first_left_div);
  }
  std::string ini_key = key.substr(first_left_div + 1);

  IniNode ini_node(prefix, ini_key, value);
  pending_set_vec_.push_back(ini_node);

  if ("" != group_) {
    //如果group_为空，说明没有使用beginGroup和endGroup
    return;
  }

  writeINI();
}

std::string IniHandle::value(const std::string& key) const {
  std::vector<IniNode> node_vec = readINI();

  std::string prefix("");
  std::string::size_type first_left_div = key.find_first_of("/");
  if (key.npos != first_left_div) {
    prefix = key.substr(0, first_left_div);
  } else {
    prefix = group_;
  }

  std::string ini_key = key.substr(first_left_div + 1);
  for (std::size_t i = 0; i != node_vec.size(); ++i) {
    if (node_vec[i].get_group() == prefix && node_vec[i].get_key() == ini_key) {
      return node_vec[i].get_value();
    }
  }

  return "";
}

// void IniHandle::writeINI() {
//  //    pending_set_vec_
//  //
//  只能有一个(已有的key，在group中没有的key，没有group)，或者都在一个Group中的（有group和没有group）。
//  //写入，如果已经有该Key，修改;如果没有该Key，添加
//  if (pending_set_vec_.empty()) {
//    return;
//  }

//  std::string file_str("");

//  std::ifstream ifile;
//  ifile.open(filename_);

//  ifile.close();

//  pending_set_vec_.clear();
//}

void IniHandle::writeINI() {
  //写入，如果已经有该Key，修改;如果没有该Key，添加
  if (pending_set_vec_.empty()) {
    return;
  }

  //处理第一个和最后一个Group

  std::string file_str("");

  std::ifstream ifile;
  ifile.open(filename_);

  std::string old_root_name("");
  std::string new_root_name("");
  std::string line_data("");
  std::vector<IniNode>::const_iterator iter;
  //遍历文件中已有的数据，把pending_set_vec_中数据放入相应的位置
  while (getline(ifile, line_data)) {
    if (line_data.npos != line_data.find("[") &&
        line_data.npos != line_data.find("]")) {
      std::size_t left_pos = line_data.find("[");
      std::size_t right_pos = line_data.find("]");
      new_root_name = line_data.substr(left_pos + 1, right_pos - 1);
      //第一个root,new_root_name=old_root_name
      if ("" == old_root_name) {
        old_root_name = new_root_name;
      }
    }

    if (old_root_name != new_root_name) {
      // 文件中要进行下一个模块Group，pending_set_vec_中还有上一个Group数据，
      // 说明ini文件中没有，需要添加新的
      for (iter = pending_set_vec_.begin(); iter != pending_set_vec_.end();) {
        if (old_root_name == iter->get_group()) {
          std::string line_data_new("");
          line_data_new = iter->get_key();
          line_data_new += "=";
          line_data_new += iter->get_value();
          line_data_new += "\n";

          file_str += line_data_new;
          iter = pending_set_vec_.erase(iter);
        } else {
          ++iter;
        }
      }
      old_root_name = new_root_name;

      if ("" != file_str) {
        file_str += "\n";
      }

      file_str += "[";
      file_str += new_root_name;
      file_str += "]";
      file_str += "\n";

    } else {
      //更新有key的
      std::string key("");
      if (line_data.npos != line_data.find("=")) {
        std::size_t equal_sign_pos = line_data.find("=");
        key = line_data.substr(0, equal_sign_pos);
      }
      std::string line_data_new("");
      int update_key = 0;
      for (iter = pending_set_vec_.begin(); iter != pending_set_vec_.end();) {
        if (old_root_name == iter->get_group() && key == iter->get_key()) {
          line_data_new = key;
          line_data_new += "=";
          line_data_new += iter->get_value();
          line_data_new += "\n";
          update_key = 1;
          pending_set_vec_.erase(iter);
        } else {
          ++iter;
        }
      }
      if (0 == update_key) {
        if (!line_data.empty()) {
          file_str += line_data;
          file_str += "\n";
        }

      } else {
        file_str += line_data_new;
      }
    }
  }
  //最后剩下的
  if (!pending_set_vec_.empty()) {
    for (iter = pending_set_vec_.begin(); iter != pending_set_vec_.end();) {
      if (old_root_name == iter->get_group()) {
        std::string line_data_new("");
        line_data_new = iter->get_key();
        line_data_new += "=";
        line_data_new += iter->get_value();
        line_data_new += "\n";

        file_str += line_data_new;
        iter = pending_set_vec_.erase(iter);
      } else {
        //新Group
        file_str += "\n";

        std::string line_data_new("");
        line_data_new += "[";
        line_data_new += iter->get_group();
        line_data_new += "]";
        line_data_new += "\n";
        file_str += line_data_new;

        old_root_name = iter->get_group();

        line_data_new = "";
        line_data_new = iter->get_key();
        line_data_new += "=";
        line_data_new += iter->get_value();
        line_data_new += "\n";

        file_str += line_data_new;

        ++iter;
      }
    }
  }

  ifile.close();
  //写入文件
  std::ofstream ofile;
  ofile.open(filename_);
  ofile.flush();
  ofile << file_str;
  ofile.close();

  pending_set_vec_.clear();
}
