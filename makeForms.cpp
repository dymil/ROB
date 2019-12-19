#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <iterator>
#include <ctime>

#include "read_mat.hpp"

using namespace std;

int main(int argc, char** argv)
{
    srand(time(NULL));

    string output = "forms.txt";
    int num_forms = 10;
    int num_clauses = 5;
    //int clause_low = 1;
    int clause_high = 10;
    int max_gene = 2000; //not inclusive
    double not_prob = 0.5;

    vector<vector<bool> > matrix; // genes are rows, cells are columns
    vector<string> cellNames, geneNames;
    ifstream file(argv[1]);
    auto ret = parseMatFile(file);
    if (std::get<0>(ret).size())
        std::tie(cellNames, geneNames, matrix) = ret;
    else
        std::exit(1);

    ofstream output_file(output);

     for (int i=0; i<num_forms; i++) {
         for (int j=0; j<num_clauses; j++) {
             int num_literals = (rand() % clause_high) + 1;
             for (int k=0; k<num_literals; k++) {
                 if (rand()%2 == 1) {
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
