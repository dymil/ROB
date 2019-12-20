#include <vector>
#include <string>
#include <cassert>
#include <utility>
#include <fstream>
#include <iostream>
#include <cstring>
#include <iterator>
#include <sstream>
#include <chrono>

#include "index.hpp"
#include "read_mat.hpp"
#include "naive.hpp"

using namespace std;

int parse_formula(vector<string>& vec, string line, const char * delim) {
    char cstr[line.size()+1];
    strcpy(cstr, line.c_str());
    char *token = strtok(cstr, delim);

    short count = 0;
    short size = 8;
    vec.resize(size);

    while (token != NULL) {
        vec[count] = token;
        token = strtok(NULL, delim);
        count++;
        if (count == size) {
            size = size*2;
            vec.resize(size);
        }
    }

    return count;
}

int parse_clause(vector<pair<bool,string> >& vec, string line, const char * delim) {
    char cstr[line.size()+1];
    strcpy(cstr, line.c_str());
    char *token = strtok(cstr, delim);

    short count = 0;
    short size = 8;
    vec.resize(size);

    while (token != NULL) {
        if (token[0] == '!') {
            vec[count].first = false;
            vec[count].second = &token[1];
        } else {
            vec[count].second = token;
            vec[count].first = true;
        }
        token = strtok(NULL, delim);
        count++;
        if (count == size) {
            size *= 2;
            vec.resize(size);
        }
    }

    return count;
}

vector<vector<pair<bool,string> > > build_formula(const string line) {
    vector<string> clauses;
    vector<vector<pair<bool,string> > > formula;

    short num_clauses = parse_formula(clauses, line, "^");
    formula.resize(num_clauses);

    // parse each clause into  literals
    short num_literals;
    for (int i=0; i < num_clauses; i++) {
        num_literals = parse_clause(formula[i], clauses[i], "|");
        formula[i].resize(num_literals);
    }

    return formula;
}

int main(int argc, char** argv)
{
    vector<vector<bool> > matrix; // genes are rows, cells are columns
    vector<string> cellNames, geneNames;

    if (argc < 4) {
        matrix.resize(7);
        for (int i=0; i<7; i++) {
            for (int j=0; j<5; j++) {
                matrix[i].resize(5);
            }
        }
        matrix[0][0] = 1;
        matrix[2][0] = 1;
        matrix[3][0] = 1;
        matrix[5][0] = 1;

        matrix[0][1] = 1;
        matrix[2][1] = 1;
        matrix[3][1] = 1;

        matrix[0][2] = 1;

        cellNames.push_back("a");
        cellNames.push_back("b");
        cellNames.push_back("c");
        cellNames.push_back("d");
        cellNames.push_back("e");

        geneNames.push_back("a");
        geneNames.push_back("b");
        geneNames.push_back("c");
        geneNames.push_back("d");
        geneNames.push_back("e");
        geneNames.push_back("f");
        geneNames.push_back("gg");
    } else {
      ifstream file(argv[3]);
      auto ret = parseMatFile(file);
      if (std::get<0>(ret).size())
	std::tie(cellNames, geneNames, matrix) = ret;
      else
	std::exit(1);
    }

    if (argc >= 2) {
      if (string("query").compare(argv[1]) == 0) {
          ifstream input(argv[2]);
          string line;
          vector<string> clauses;
          vector<vector<pair<bool,string> > > formula;
	  chrono::high_resolution_clock::duration sum1(0);
	  chrono::high_resolution_clock::duration sum2(0);

	  Index ind(matrix, geneNames, cellNames);
	  Naive naive(matrix, geneNames, cellNames);

          while (getline(input, line)) {
              vector<vector<pair<bool,string> > > formula = build_formula(line);

              auto exp1 = chrono::high_resolution_clock::now();
              vector<int> answer1 = ind.first_clause(formula);
              sum1 += chrono::high_resolution_clock::now() - exp1;
	      
              exp1 = chrono::high_resolution_clock::now();
              vector<int> answer2 = naive.query(formula);
              sum2 += chrono::high_resolution_clock::now() - exp1;
	      
	      std::sort(answer1.begin(), answer1.end());
	      std::sort(answer2.begin(), answer2.end());
	      cerr << "num set ours: " << answer1.size() << '\n';
	      cerr << "num set theirs: " << answer2.size() << '\n';
	      if (answer1 != answer2) {
		cerr << "answers didn't match for formula:" << line << endl;
		exit(1);
	      }
          }
          cout << "time ours: " << chrono::duration_cast<chrono::microseconds>(sum1).count() << "μs\n";
          cout << "time naive: " << chrono::duration_cast<chrono::microseconds>(sum2).count() << "μs\n";
        } else {
            cout << "Unknown command. Please use query.";
        }
    }
}
