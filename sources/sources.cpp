// Copyright by sometosome

#include "header.hpp"
#include <iostream>
#include <algorithm>

Filesystem::Filesystem(const std::string path_to_file){
  _path_to_ftp = boost::filesystem::path(path_to_file);
  std::cout << _path_to_ftp << std::endl;
  if (is_symlink(_path_to_ftp))
    _path_to_ftp = read_symlink(_path_to_ftp);
  if (!is_directory(_path_to_ftp))
    throw std::runtime_error("This path is not directory");
}

void Filesystem::all_path(boost::filesystem::path p, std::ostream &out) {
  for (const boost::filesystem::directory_entry& x :
       boost::filesystem::directory_iterator{p}) {
    if (is_directory(x.path()))
      all_path(x.path(), out);
    else if (is_regular_file(x.path()))
      handler(x.path(), out);
  }
}

bool Filesystem::handler(boost::filesystem::path p, std::ostream &out) {
  if (check_fiilename(p)) {
    std::string file_name = p.filename().string();
    std::string account = what_account(file_name);
    std::string data = what_data(file_name);
    std::string broker = what_broker(p);

    if (account.size() == 0 || data.size() == 0 || broker.size() == 0)
      return false;
    out << broker << " " <<file_name << std::endl;
    insert_element(account, data, broker);
    return true;
  }else {
    return false;
  }
}

bool Filesystem::check_fiilename(boost::filesystem::path p){
  const std::string _txt = ".txt";
  const std::string _balance = "balance";

  if (p.extension() == _txt && p.filename().size() == len_namefile &&
      p.filename().string().substr(0, 7) == _balance)
    return true;
  else
    return false;
}

std::string Filesystem::what_account(std::string p){
  std::size_t iterator1 = p.find_first_of('_', 0);
  if (iterator1 == std::string::npos)  return std::string();
  ++iterator1;
  std::size_t iterator2 = p.find_last_of('_');
  if (iterator2 == std::string::npos) return std::string();
  std::string account = p.substr(iterator1, iterator2-iterator1);

  if (account.find_first_not_of(_numbers, 0) != std::string::npos)
    return std::string();

  return account;
}

std::string Filesystem::what_data(std::string p){
  std::size_t iterator1 = p.find_last_of('_');
  if (iterator1 == std::string::npos) return std::string();
  ++iterator1;
  std::size_t iterator2 = p.find_last_of('.');
  if (iterator2 == std::string::npos) return std::string();
  std::string data = p.substr(iterator1, iterator2-iterator1);

  if (data.find_first_not_of(_numbers, 0) != std::string::npos)
    return std::string();
  return data;
}

std::string Filesystem::what_broker(boost::filesystem::path p){
  p = absolute(p);
  std::size_t iterator2 = p.string().find_last_of('/');
  if (iterator2 == std::string::npos) return std::string();
  --iterator2;
  std::size_t iterator1 = p.string().find_last_of('/', iterator2);
  if (iterator1 == std::string::npos) return std::string();
  ++iterator1;
  ++iterator2;
  std::string broker = p.string().substr(iterator1, iterator2-iterator1);

  return broker;
}

void Filesystem::insert_element(std::string account, std::string data,
                                std::string broker){
  std::vector<std::string>::iterator it = std::find(_account.begin(),
                                                    _account.end(),
                                                    account);
  std::vector<std::string>::difference_type index = std::distance
      (_account.begin(), it);
  if (_account.size() == static_cast<size_t>(index)) {
    _account.push_back(account);
    _broker.push_back(broker);
    _files.push_back(1);
    _lastdate.push_back(std::stoi(data));
  }else if (_broker[index] == broker){
    ++_files[index];
    _lastdate[index] =
        (_lastdate[index] > std::stoi(data))?_lastdate[index]:std::stoi(data);
  }else{
    throw std::runtime_error("Two owners per account");
  }
}

void Filesystem::show_account(std::ostream &out){
  for (size_t i = 0; i < _account.size(); ++i){
    out << "broker:" << _broker[i] << " account:" << _account[i] <<
        " files:" << _files[i] << " lastdate:" << _lastdate[i] << std::endl;
  }
}

std::ostream& operator<<(std::ostream &out,  Filesystem& file_system) {
  file_system.all_path(file_system._path_to_ftp, out);
  file_system.show_account(out);
  return out;
}

