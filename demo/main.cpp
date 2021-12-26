#include <header.hpp>
#include <iostream>


int main() {
  // path  менять в зависимости от системы
  std::string path("/home/noname/CLionProjects/lab4/misc/ftp");
  Filesystem test(path);
  std::cout << test;
}