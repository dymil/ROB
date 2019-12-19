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

#include "read_mat.hpp"

using namespace std;

int main(int argc, char** argv)
{
    srand(time(NULL));

    string output = "forms.txt";
    int num_forms = 10;
    int num_clauses = 5;
    int clause_high = 10;
    int not_prob = 2; //not probability is 1 / this number

    vector<vector<bool> > matrix; // genes are rows, cells are columns
    vector<string> cellNames, geneNames;
    ifstream file(argv[1]);
    auto ret = parseMatFile(file);
    if (std::get<0>(ret).size())
        std::tie(cellNames, geneNames, matrix) = ret;
    else
        std::exit(1);

    int max_gene = geneNames.size();

    ofstream output_file(output);

     for (int i=0; i<num_forms; i++) {
         for (int j=0; j<num_clauses; j++) {
             int num_literals = (rand() % clause_high) + 1;
             for (int k=0; k<num_literals; k++) {
                 if (rand()%not_prob == 0) {
                     output_file << "!";
                 }
                 int next_lit = (rand() % max_gene);
                 output_file << geneNames[next_lit];
                 if (k!= num_literals-1) {output_file << "|";}
             }
             if (j!= num_clauses-1) {output_file << "^";}
         }
         if (i!=num_forms-1){output_file << "\n";}
     }
}
