#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <iterator>
#include <random>

#include "read_mat.hpp"

using namespace std;

int main(int argc, char** argv)
{
    string output = "forms.txt";
    default_random_engine gen;

    vector<vector<bool> > matrix; // genes are rows, cells are columns
    vector<string> cellNames, geneNames;
    ifstream file(argv[1]);
    auto ret = parseMatFile(file);
    if (std::get<0>(ret).size())
        std::tie(cellNames, geneNames, matrix) = ret;
    else
        std::exit(1);

    int num_forms = 10;
    int num_clauses = 5;
    int clause_high = 10;
    double not_prob = 0.5;
    int max_gene = geneNames.size();

    uniform_int_distribution clauseDist(0, clause_high);
    uniform_int_distribution geneDist(0, max_gene);
    uniform_real_distribution notDist(0.0, 1.0);

    ofstream output_file(output);

     for (int i=0; i<num_forms; i++) {
         for (int j=0; j<num_clauses; j++) {
	   int num_literals = (clauseDist(gen)) + 1;
             for (int k=0; k<num_literals; k++) {
	       if (notDist(gen) < not_prob) {
                     output_file << "!";
                 }
	       int next_lit = geneDist(gen);
                 output_file << geneNames[next_lit];
                 if (k!= num_literals-1) {output_file << "|";}
             }
             if (j!= num_clauses-1) {output_file << "^";}
         }
         if (i!=num_forms-1){output_file << "\n";}
     }
}
