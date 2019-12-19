#include <sstream>
#include <fstream>
#include <optional>
#include <vector>
#include <tuple>
#include <string>
#include <iterator>
#include <algorithm>
#include <iostream>

using std::stringstream, std::string, std::optional, std::tuple, std::vector, std::istream;

/* Reads a mat file, a la from panglaodb.se; returns colnames. rowsnames, and finally the matrix */
optional<tuple<vector<string>, vector<string>, vector<vector<bool>>>> parseMatFile(istream& in) {
  string line;

  // read cell names
  if (!std::getline(in, line))
    return optional<tuple<vector<string>, vector<string>, vector<vector<bool>>>>();

  vector<string> colNames;
  std::istringstream ss(line);
  std::copy(std::istream_iterator<string>(ss), std::istream_iterator<string>(), std::back_inserter(colNames));

  vector<string> rowNames;
  vector<vector<bool>> matrix;
  while (std::getline(in, line)) {
    ss.str(line);
    ss.clear();
    string name;
    ss >> name;
    rowNames.push_back(name);
    matrix.push_back(vector<bool>());
    std::copy_n(std::istream_iterator<bool>(ss), colNames.size(), std::back_inserter(matrix.back()));
  }
  return optional<tuple<vector<string>, vector<string>, vector<vector<bool>>>>({colNames, rowNames, matrix});
}

int main(int argc, char* argv[]) {
  vector<string> colNames, rowNames;
  vector<vector<bool>> matrix;
  if (argc < 2) {
    string s("Apple Bat Cat\nA 0 1 0\nB 1 1 0");
    std::istringstream ss(s);
    std::tie(colNames, rowNames, matrix) = parseMatFile(ss).value();
  } else {
    std::ifstream matFile(argv[1]);
    std::tie(colNames, rowNames, matrix) = parseMatFile(matFile).value();
  }
  std::for_each(colNames.cbegin(), colNames.cend() - 1, [](string s){std::cout << s << ' ';});
  std::cout << colNames.back() << '\n';
  std::for_each(rowNames.cbegin(), rowNames.cend() - 1, [](string s){std::cout << s << ' ';});
  std::cout << rowNames.back() << '\n';
  std::for_each(matrix.cbegin(), matrix.cend() - 1,
		[](vector<bool> row){
		  std::for_each(row.cbegin(), row.cend() - 1, [](bool s){std::cout << s << ' ';});
		  std::cout << row.back() << '\n';});
  std::for_each(matrix.back().cbegin(), matrix.back().cend() - 1, [](bool s){std::cout << s << ' ';});
  std::cout << matrix.back().back();
}
