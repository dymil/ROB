#include <sstream>
#include <optional>
#include <vector>
#include <tuple>
#include <string>
#include <iterator>
#include <algorithm>

using std::string, std::optional, std::tuple, std::vector, std::istream;

/* Reads a mat file, a la from panglaodb.se; returns colnames, rowsnames, and finally the matrix */
optional<tuple<vector<string>, vector<string>, vector<vector<bool>>>> parseMatFile(istream& in) {
  string line;

  // read col names
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
    matrix.push_back(vector<bool>(colNames.size()));
    if (std::copy(std::istream_iterator<bool>(ss), std::istream_iterator<bool>(), matrix.back().begin()) - matrix.back().begin() < colNames.size())
      return optional<tuple<vector<string>, vector<string>, vector<vector<bool>>>>();
  }
  return optional<tuple<vector<string>, vector<string>, vector<vector<bool>>>>({colNames, rowNames, matrix});
}
