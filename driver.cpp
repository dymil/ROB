#include <set>
#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>
#include <utility>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "index.h"

using namespace std;

int parse_formula(vector<string> *vec, string line, const char * delim) {
    char cstr[line.size()+1];
    strcpy(cstr, line.c_str());
    char *token = strtok(cstr, delim);

    short count = 0;
    short size = 8;
    (*vec).resize(size);

    while (token != NULL) {
        (*vec)[count] = token;
        token = strtok(NULL, delim);
        count++;
        if (count == size) {
            size = size*2;
            (*vec).resize(size);
        }
    }

    return count;
}

int parse_clause(vector<pair<bool,string> > *vec, string line, const char * delim) {
    char cstr[line.size()+1];
    strcpy(cstr, line.c_str());
    char *token = strtok(cstr, delim);

    short count = 0;
    short size = 8;
    (*vec).resize(size);

    while (token != NULL) {
        if (token[0] == '!') {
            (*vec)[count].first = false;
            (*vec)[count].second = &token[1];
        } else {
            (*vec)[count].second = token;
            (*vec)[count].first = true;
        }
        token = strtok(NULL, delim);
        count++;
        if (count == size) {
            size = size*2;
            (*vec).resize(size);
        }
    }

    return count;
}

vector<vector<pair<bool,string> > > build_formula(string input_file) {
    ifstream input(input_file);
    string line;
    char *token;
    vector<string> clauses;
    vector<vector<pair<bool,string> > > formula;

    getline(input, line);
    short num_clauses = parse_formula(&clauses, line, "^");
    formula.resize(num_clauses);

    // parse each clause into  literals
    short num_literals;
    for (int i=0; i < num_clauses; i++) {
        num_literals = parse_clause(&(formula[i]), clauses[i], "|");
        formula[i].resize(num_literals);
        for (int j=0; j < num_literals; j++) {
        }
    }

    input.close();
    return formula;
}

int main(int argc, char** argv)
{

    vector<vector<bool> > matrix; // genes are rows, cells are columns

    //vector<vector<bool> > matrix;
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

    vector<string> cellnames;
    cellnames.push_back("a");
    cellnames.push_back("b");
    cellnames.push_back("c");
    cellnames.push_back("d");
    cellnames.push_back("e");

    vector<string> genenames;
    genenames.push_back("a");
    genenames.push_back("b");
    genenames.push_back("c");
    genenames.push_back("d");
    genenames.push_back("e");
    genenames.push_back("f");
    genenames.push_back("gg");

    const vector<vector<bool> >& matrix1 = matrix;
    const vector<string>& cellnames1 = cellnames;
    const vector<string>& genenames1 = genenames;


    if (argc >= 2) {
        if (((string)"query").compare(argv[1]) == 0) {
            vector<vector<pair<bool,string> > > formula = build_formula(argv[2]);
            Index ind(matrix1, genenames1, cellnames1);
            vector<int> answer = ind.first_clause(&formula);
            cout << "num sat: " << answer.size() << "\n";
        } else {
            cout << "Unknown command. Please use query.";
        }
    }

}
