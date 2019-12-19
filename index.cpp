#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>
#include <utility>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstring>
#include "index.hpp"

using namespace std;

Index::Index(const vector<vector<bool> >& mat, const vector<string>& geneNames, const vector<string>& cellNames): matrix(mat), geneNames(geneNames), cellNames(cellNames), expressed(geneNames.size()) {
    assert(matrix.size() == geneNames.size());
    for (int i = 0; i < geneNames.size(); ++i) {
      geneNamesToIdx.insert({geneNames[i], i});
    }

    for (int i = 0; i < cellNames.size(); ++i) {
      cellNamesToIdx.insert({cellNames[i], i});
    }

    for (int i = 0; i < expressed.size(); ++i) {
      for (int j = 0; j < matrix[i].size(); ++j)
	(matrix[i][j] ? expressed[i].first : expressed[i].second).push_back(j);
    }
 }

vector<int> Index::clause_helper(const vector<pair<bool,string> >& clause, vector<int>& satisfied,
  const vector<int>& not_satisfied, int curr_index) {
  // if we have iterated through all literals, then we are done
  if (curr_index >= clause.size()) {
      return satisfied;
  }
  else {
      vector<int> still_unsatisfied;
      int curr_gene = geneNamesToIdx[clause[curr_index].second];
      for (int i=0; i < not_satisfied.size(); i++){ //iterate over each cell
          bool flag = clause[curr_index].first;
          bool cell = matrix[curr_gene][not_satisfied[i]];
          if ((cell&flag)|(!cell&!flag)) {
              satisfied.push_back(not_satisfied[i]);
          } else {
              still_unsatisfied.push_back(not_satisfied[i]);
          }
      }
      return clause_helper(clause, satisfied, still_unsatisfied, curr_index+1);
  }
}

vector<int> Index::next_clause(const vector<vector<pair<bool,string> > >&formula, const vector<int>& prev_set, int curr) {
  // if we have iterated through all clauses, then we are done
  if (curr >= formula.size()) {
      return prev_set;
  }
  else {
      vector<int> temp;
      vector<int> satisfied = clause_helper(formula[curr], temp, prev_set, 0);
      return next_clause(formula, satisfied, curr+1);
  }
}

vector<int> Index::first_clause(const vector<vector<pair<bool,string> > >& formula) {
  int gene = geneNamesToIdx[formula[0][0].second];
  vector<int> curr_sat = (formula[0][0].first ? expressed[gene].first : expressed[gene].second);
  vector<int> curr_unsat = (formula[0][0].first ? expressed[gene].second : expressed[gene].first);
  vector<int> temp;
  curr_sat = clause_helper(formula[0], curr_sat, curr_unsat, 1);
  return next_clause(formula, curr_sat, 1);
}
