#ifndef INIHANDLE_H
#define INIHANDLE_H

#include <string>
#include <vector>

class IniNode {
 public:
  IniNode(const std::string& group, const std::string& key,
          const std::string& value)
      : group_(group), key_(key), value_(value) {}

  std::string get_group() const { return this->group_; }
  std::string get_key() const { return this->key_; }
  std::string get_value() const { return this->value_; }

 private:
  std::string group_;
  std::string key_;
  std::string value_;
};

class IniHandle {
 public:
  explicit IniHandle(const std::string& filename);
  ~IniHandle();

  std::vector<IniNode> readINI() const;

  void beginGroup(const std::string& prefix);
  void endGroup();
  std::string group() const;

  void setValue(const std::string& key, const std::string& value);
  std::string value(const std::string& key) const;

 private:
  void writeINI();

 private:
  std::string filename_;
  std::string group_;
  std::vector<IniNode> pending_set_vec_;
};

#endif  // INIHANDLE_H
