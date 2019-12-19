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
    for (unsigned int i = 0; i < geneNames.size(); ++i) {
      geneNamesToIdx.insert({geneNames[i], i});
    }

    for (unsigned int i = 0; i < cellNames.size(); ++i) {
      cellNamesToIdx.insert({cellNames[i], i});
    }

    for (unsigned int i = 0; i < expressed.size(); ++i) {
      for (unsigned int j = 0; j < matrix[i].size(); ++j)
	(matrix[i][j] ? expressed[i].first : expressed[i].second).push_back(j);
    }
 }

vector<int> Index::clause_helper(const vector<pair<bool,string> >& clause, vector<int>& satisfied,
  const vector<int>& not_satisfied, int curr_index) const {
  // if we have iterated through all literals, then we are done
  if (curr_index >= clause.size()) {
      return satisfied;
  }
  else {
      vector<int> still_unsatisfied;
      int curr_gene = geneNamesToIdx.at(clause[curr_index].second);
      for (int cell : not_satisfied){
          if (clause[curr_index].first == matrix[curr_gene][cell]) {
              satisfied.push_back(cell);
          } else {
              still_unsatisfied.push_back(cell);
          }
      }
      return clause_helper(clause, satisfied, still_unsatisfied, curr_index+1);
  }
}

vector<int> Index::next_clause(const vector<vector<pair<bool,string> > >&formula, const vector<int>& prev_set, int curr) const {
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

bool Index::gene_comparator(const pair<bool,string>& gene1, const pair<bool,string>& gene2) const {
  int ind1 = geneNamesToIdx.at(gene1.second);
  int ind2 = geneNamesToIdx.at(gene2.second);
    int count1 = (gene1.first ? expressed[ind1].first : expressed[ind1].second).size();
    int count2 = (gene1.first ? expressed[ind2].first : expressed[ind2].second).size();
    return count1 > count2;
}

void Index::sort_clause(vector<pair<bool,string> >& clause) const {
  sort(clause.begin(), clause.end(), [this](const auto& l, const auto& r){return gene_comparator(l, r);});
}

bool Index::clause_comparator(const vector<pair<bool,string> >& c1, const vector<pair<bool,string> >& c2) const {
  int sizeC1 = 0;
  int sizeC2 = 0;
  int curr_gene;
  for (int i=0; i<c1.size(); i++) {
    curr_gene = geneNamesToIdx.at(c1[i].second);
    sizeC1 += (c1[i].first ? expressed[curr_gene].first : expressed[curr_gene].second).size();
  }
  for (int i=0; i<c2.size(); i++) {
    curr_gene = geneNamesToIdx.at(c2[i].second);
    sizeC2 += (c2[i].first ? expressed[curr_gene].first : expressed[curr_gene].second).size();
  }
  return sizeC1 < sizeC2;
}

void Index::sort_formula(vector<vector<pair<bool,string> > >& formula) const {
    sort(formula.begin(), formula.end(), [this](const auto& l, const auto& r){return clause_comparator(l, r);});
}

vector<int> Index::first_clause(vector<vector<pair<bool,string> > >& formula) const {
    sort_formula(formula);
    for (auto& clause : formula) {
        sort_clause(clause);
    }
  int gene = geneNamesToIdx.at(formula[0][0].second);
  vector<int> curr_sat = (formula[0][0].first ? expressed[gene].first : expressed[gene].second);
  vector<int> curr_unsat = (formula[0][0].first ? expressed[gene].second : expressed[gene].first);
  vector<int> temp;
  curr_sat = clause_helper(formula[0], curr_sat, curr_unsat, 1);
  return next_clause(formula, curr_sat, 1);
}
