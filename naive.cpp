#include <algorithm>
#include <cassert>

#include "naive.hpp"

using std::vector, std::string, std::unordered_map, std::pair;

Naive::Naive(const vector<vector<bool>>& mat, const vector<string>& geneNames, const vector<string>& cellNames): matrix(mat), geneNames(geneNames), cellNames(cellNames) {
  assert(matrix.size() == geneNames.size());
  for (int i = 0; i < geneNames.size(); ++i) {
    geneNamesToIdx.insert({geneNames[i], i});
  }

  assert(matrix[0].size() == cellNames.size());
  for (int i = 0; i < cellNames.size(); ++i) {
    cellNamesToIdx.insert({cellNames[i], i});
  }
}

/* Returns cell indices which satisfy the query. Checks each cell individually */
vector<int> Naive::query(const vector<vector<pair<bool, string>>>& query) const {
  vector<vector<pair<bool, int>>> query2(query.size());
  std::transform(query.cbegin(), query.cend(), query2.begin(), [this](vector<pair<bool, string>> clause) -> vector<pair<bool, int>> {
      vector<pair<bool, int>> clause2(clause.size());
      std::transform(clause.cbegin(), clause.cend(), clause2.begin(), [this](pair<bool, string> literal) -> pair<bool, int> {
	  return {literal.first, this->geneNamesToIdx.at(literal.second)};
	});
      return clause2;
    });
  vector<int> res;
  for (int i = 0; i < cellNames.size(); ++i) { // iterate over cells
    if (std::all_of(query2.cbegin(), query2.cend(),
		    [this, i](const vector<pair<bool, int>>& clause)-> bool {
		      return std::any_of(clause.cbegin(), clause.cend(),
					 [this, i](pair<bool, int> literal) {
					   return literal.first == this->matrix[literal.second][i];
					 });
		    }))
      res.push_back(i);
  }
  return res;
}
