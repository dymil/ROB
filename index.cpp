#include <set>
#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>
#include <utility>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstring>

using std::vector, std::set, std::string, std::unordered_map, std::pair, std::ifstream, std::cout;

class Index {
  vector<vector<bool>> matrix; // genes are rows, cells are columns
  vector<string> geneNames;
  vector<string> cellNames;
  unordered_map<string, int> geneNamesToIdx;
  unordered_map<string, int> cellNamesToIdx;
  vector<pair<set<int>, set<int>>> expressed; // stores which cells express a given gene or don't; TODO: should they be ints?

public:
  Index(const vector<vector<bool>>& mat, const vector<string>& geneNames, const vector<string>& cellNames): matrix(mat), geneNames(geneNames), cellNames(cellNames), expressed(geneNames.size()) {
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

  static int parse_helper(vector<string>& vec, string line, const char * delim) {
      char cstr[line.size()+1];
      strcpy(cstr, line.c_str());
      char *token = strtok(cstr, delim);

      short count = 0;
      short size = 8;
      vec.resize(size);

      while (token != NULL) {
          vec[count] = token;
          token = strtok(NULL, delim);
          ++count;
          if (count == size) {
              size *= 2;
              vec.resize(size);
          }
      }

      return count;
  }

  static vector<vector<string> > build_formula(string input_file) {
      ifstream input(input_file);
      string line;
      char *token;
      vector<string> clauses;
      vector<vector<string> > formula;

      getline(input, line);
      short num_clauses = parse_helper(clauses, line, "^");
      cout << "num clauses: " << num_clauses << "\n";
      formula.resize(num_clauses);

      // parse each clause into literals
      short num_literals;
      for (int i=0; i < num_clauses; i++) {
          num_literals = parse_helper(formula[i], clauses[i], "|");
          formula[i].resize(num_literals);
      }

      input.close();
      return formula;
  }
};

int main(int argc, char** argv)
{
    if (argc >= 2) {
        if (((string)"query").compare(argv[1]) == 0) {
	  Index::build_formula(argv[2]);
        } else {
            cout << "Unknown command. Please use query.";
        }
    }

}
