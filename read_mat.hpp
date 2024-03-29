#include <sstream>
#include <vector>
#include <tuple>
#include <string>
#include <iterator>
#include <algorithm>
#include <ostream>
#include <iostream>

using std::string, std::tuple, std::vector, std::istream, std::ostream;

/* Reads a mat file, a la from panglaodb.se; returns colnames, rowsnames, and finally the matrix */

template<class BidirIt>
void printWithDelim(const BidirIt& cbegin, const BidirIt& cend, char delim, ostream& out) {
  std::for_each(cbegin, cend - 1, [&out, delim](const typename std::iterator_traits<BidirIt>::value_type& s){out << s << delim;});
  out << *(cend - 1);
}

tuple<vector<string>, vector<string>, vector<vector<bool>>> parseMatFile(istream& in) {
  string line;

  // read col names
  if (!std::getline(in, line))
    return tuple<vector<string>, vector<string>, vector<vector<bool>>>{{}, {}, {}};

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
    auto n_copied = std::copy(std::istream_iterator<int>(ss), std::istream_iterator<int>(), matrix.back().begin()) - matrix.back().begin(); // converts int to bool
    if (n_copied < colNames.size()) {
      std::cerr << "Copied: " << n_copied << " of expected: " << colNames.size() << " for line:\n" << line << "\nGot:\n";
      printWithDelim(matrix.back().cbegin(), matrix.back().cbegin() + n_copied, '\t', std::cerr);
      std::cerr << std::endl;
      return tuple<vector<string>, vector<string>, vector<vector<bool>>>{{}, {}, {}};
    }
  }
  return tuple<vector<string>, vector<string>, vector<vector<bool>>>{colNames, rowNames, matrix};
}
