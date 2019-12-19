#include <fstream>
#include <vector>
#include <iostream>
#include <tuple>

#include "read_mat.hpp"

using std::vector, std::string;

int main(int argc, char* argv[]) {
  vector<string> colNames, rowNames;
  vector<vector<bool>> matrix;
  if (argc < 2) {
    string s("Apple Bat Cat\nA 0 1 0\nB 1 1 0");
    std::istringstream ss(s);
    auto ret = parseMatFile(ss);
    if (ret)
      std::tie(colNames, rowNames, matrix) = ret.value();
    else
      std::exit(1);
  } else {
    std::ifstream matFile(argv[1]);
    std::tie(colNames, rowNames, matrix) = parseMatFile(matFile).value();
  }
  std::for_each(colNames.cbegin(), colNames.cend() - 1, [](const string& s){std::cout << s << ' ';});
  std::cout << colNames.back() << '\n';
  std::for_each(rowNames.cbegin(), rowNames.cend() - 1, [](const string& s){std::cout << s << ' ';});
  std::cout << rowNames.back() << '\n';
  std::for_each(matrix.cbegin(), matrix.cend() - 1,
		[](vector<bool> row){
		  std::for_each(row.cbegin(), row.cend() - 1, [](bool s){std::cout << s << ' ';});
		  std::cout << row.back() << '\n';});
  std::for_each(matrix.back().cbegin(), matrix.back().cend() - 1, [](bool s){std::cout << s << ' ';});
  std::cout << matrix.back().back();
}
