#include <fstream>
#include <vector>
#include <iostream>
#include <tuple>

#include "read_mat.hpp"

using std::vector, std::string;

int main(int argc, char* argv[]) {
  vector<string> colNames, rowNames;
  vector<vector<bool>> matrix;
  tuple<vector<string>, vector<string>, vector<vector<bool>>> ret;
  if (argc < 2) {
    string s("Apple Bat Cat\nA 0 4 0\nB 1 1 0");
    std::istringstream ss(s);
    ret = parseMatFile(ss);
  } else {
    std::ifstream matFile(argv[1]);
    ret = parseMatFile(matFile);
  }
  if (std::get<0>(ret).size())
    std::tie(colNames, rowNames, matrix) = ret;
  else
    std::exit(1);

  printWithDelim(colNames.cbegin(), colNames.cend(), ' ', std::cout);
  std::cout << '\n';
  printWithDelim(rowNames.cbegin(), rowNames.cend(), ' ', std::cout);
  std::cout << '\n';
  std::for_each(matrix.cbegin(), matrix.cend() - 1,
		[](vector<bool> row){
		  printWithDelim(row.cbegin(), row.cend(), ' ', std::cout);
		  std::cout << '\n';});
  std::for_each(matrix.back().cbegin(), matrix.back().cend() - 1, [](bool s){std::cout << s << ' ';});
  std::cout << matrix.back().back();
}
