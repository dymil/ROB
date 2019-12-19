#include <sstream>
#include <optional>
#include <vector>
#include <tuple>
#include <string>
#include <iterator>
#include <algorithm>
#include <ostream>

using std::string, std::optional, std::tuple, std::vector, std::istream, std::ostream;

/* Reads a mat file, a la from panglaodb.se; returns colnames, rowsnames, and finally the matrix */

template<class BidirIt>
void printWithDelim(const BidirIt& cbegin, const BidirIt& cend, char delim, ostream& out) {
  std::for_each(cbegin, cend - 1, [delim](const typename std::iterator_traits<BidirIt>::value_type& s){std::cout << s << delim;});
  std::cout << *(cend - 1);
}

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
    auto n_copied = std::copy(std::istream_iterator<bool>(ss), std::istream_iterator<bool>(), matrix.back().begin()) - matrix.back().begin();
    if (n_copied < colNames.size()) {
      std::cerr << "Copied: " << n_copied << " of expected: " << colNames.size() << " for line:\n" << line << "\nGot:\n";
      printWithDelim(matrix.back().cbegin(), matrix.back().cbegin() + n_copied, '\t', std::cerr);
      std::cerr << std::endl;
      return optional<tuple<vector<string>, vector<string>, vector<vector<bool>>>>();
    }
  }
  return optional<tuple<vector<string>, vector<string>, vector<vector<bool>>>>({colNames, rowNames, matrix});
}
