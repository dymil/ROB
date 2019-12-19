#include <vector>
#include <string>
#include <cassert>
#include <utility>
#include <fstream>
#include <iostream>
#include <cstring>
#include <iterator>
#include <sstream>
#include <time.h>

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

/*
vector<vector<pair<bool,string> > > build_formula(const string& input_file) {
    ifstream input(input_file);
    string line;
    char *token;
    vector<string> clauses;
    vector<vector<pair<bool,string> > > formula;

    getline(input, line);
    short num_clauses = parse_formula(clauses, line, "^");
    formula.resize(num_clauses);

    // parse each clause into  literals
    short num_literals;
    for (int i=0; i < num_clauses; i++) {
        num_literals = parse_clause(formula[i], clauses[i], "|");
        formula[i].resize(num_literals);
        for (int j=0; j < num_literals; j++) {
        }
    }

    input.close();
    return formula;
}*/

vector<vector<pair<bool,string> > > build_formula(const string line) {
    char *token;
    vector<string> clauses;
    vector<vector<pair<bool,string> > > formula;

    short num_clauses = parse_formula(clauses, line, "^");
    formula.resize(num_clauses);

    // parse each clause into  literals
    short num_literals;
    for (int i=0; i < num_clauses; i++) {
        num_literals = parse_clause(formula[i], clauses[i], "|");
        formula[i].resize(num_literals);
        for (int j=0; j < num_literals; j++) {
        }
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
          clock_t sum1;
          clock_t sum2;

          while (getline(input, line)) {
              vector<vector<pair<bool,string> > > formula = build_formula(line);
              Index ind(matrix, geneNames, cellNames);
              clock_t exp1 = clock();
              vector<int> answer = ind.first_clause(formula);
              sum1 += clock() - exp1;
              //cout << "time ours: " << ((float)exp1)/CLOCKS_PER_SEC/1000 << "\n";
              cout << "num sat: " << answer.size() << "\n";
              Naive naive(matrix, geneNames, cellNames);
              exp1 = clock();
              answer = naive.query(formula);
              sum2 += clock() - exp1;
              //cout << "time naive: " << ((float)exp1)/CLOCKS_PER_SEC/1000 << "\n";
 	          cout << "num sat: " << answer.size() << "\n";
          }
          cout << "time ours: " << ((float)sum1)/CLOCKS_PER_SEC/1000 << "\n";
          cout << "time naive: " << ((float)sum2)/CLOCKS_PER_SEC/1000 << "\n";
        } else {
            cout << "Unknown command. Please use query.";
        }
    }
}
