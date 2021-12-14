// Copyright by sometosome

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_


#include <boost/filesystem.hpp>
#include <vector>
#include <string>

class Filesystem {
 public:
  explicit Filesystem(const std::string path_to_file);
  void all_path(boost::filesystem::path p, std::ostream &out);
  bool handler(boost::filesystem::path p, std::ostream &out);
  bool check_fiilename(boost::filesystem::path p);
  std::string what_account(std::string p);
  std::string what_data(std::string p);
  std::string what_broker(boost::filesystem::path p);
  void show_account(std::ostream &out);
  void insert_element(std::string account,
                      std::string data,
                      std::string broker);
  friend std::ostream& operator<<(std::ostream &out, Filesystem& file_system);
 private:
  boost::filesystem::path _path_to_ftp;
  std::vector<std::string> _broker;
  std::vector<std::string> _account;
  std::vector<int64_t> _files;
  std::vector<int64_t> _lastdate;

  const size_t len_namefile = 29;
  const std::string _numbers = "0123456789";
};

#endif  //  INCLUDE_HEADER_HPP_


