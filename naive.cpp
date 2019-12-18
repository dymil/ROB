#include <vector>
#include <set>
#include <utility>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <cassert>

using std::set, std::vector, std::string, std::unordered_map, std::pair;

class Naive {
  vector<vector<bool>> matrix; // genes are rows, cells are columns
  vector<string> geneNames;
  vector<string> cellNames;
  unordered_map<string, int> geneNamesToIdx;
  unordered_map<string, int> cellNamesToIdx;

public:
  Naive(const vector<vector<bool>>& mat, const vector<string>& geneNames, const vector<string>& cellNames): matrix(mat), geneNames(geneNames), cellNames(cellNames) {
    assert(matrix.size() == geneNames.size());
    for (int i = 0; i < geneNames.size(); ++i) {
      geneNamesToIdx.insert({geneNames[i], i});
    }
    
    for (int i = 0; i < cellNames.size(); ++i) {
      cellNamesToIdx.insert({cellNames[i], i});
    }
  }
  
  /* Returns cell indices which satisfy the query. Checks each cell individually */
  vector<int> naive(vector<vector<pair<bool, string>>> query) {
    vector<vector<pair<bool, int>>> query2;
    std::transform(query.cbegin(), query.cend(), query2.begin(), [this](vector<pair<bool, string>> clause)->vector<pair<bool, int>> {
	vector<pair<bool, int>> clause2(clause.size());
	std::transform(clause.cbegin(), clause.cend(), clause2.begin(), [this](pair<bool, string> literal) -> pair<bool, int> {
	    return {literal.first, this->geneNamesToIdx[literal.second]};
	  });
	return clause2;
      });
    vector<int> res;
    for (int i = 0; i < matrix[0].size(); ++i) { // iterate over cells
      if (std::all_of(query2.cbegin(), query2.cend(),
		      [this, i](vector<pair<bool, int>> clause)-> bool {
			return std::any_of(clause.cbegin(), clause.cend(),
					   [this, i](pair<bool, int> literal) {
					     return literal.first == this->matrix[literal.second][i];				   
					   });
		      }))
	res.push_back(i);
    }
      return res;
      }
      
  };
