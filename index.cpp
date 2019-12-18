#include <set>
#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>
#include <utility>
#include <algorithm>

using std::vector, std::set, std::string, std::unordered_map, std::pair;

class Index {
  vector<vector<bool>> matrix; // genes are rows, cells are columns
  vector<string> geneNames;
  vector<string> cellNames;
  unordered_map<string, int> geneNamesToIdx;
  unordered_map<string, int> cellNamesToIdx;
  vector<pair<set<int>, set<int>>> expressed; // stores which cells express a given gene or don't; TODO: should they be ints?
  
public:
  Index(const vector<vector<bool>>& mat, const vector<string> geneNames, const vector<string> cellNames): matrix(mat), geneNames(geneNames), cellNames(cellNames), expressed(geneNames.size()) {
    assert(matrix.size() == geneNames.size());
    for (int i = 0; i < geneNames.size(); ++i) {
      geneNamesToIdx.insert({geneNames[i], i});
    }

    for (int i = 0; i < cellNames.size(); ++i) {
      cellNamesToIdx.insert({cellNames[i], i});
    }

    for (int i = 0; i < expressed.size(); ++i) {
      for (int j = 0; j < matrix[i].size(); ++j)
	(matrix[i][j] ? expressed[i].first : expressed[i].second).insert(j);
    }
  }
};
