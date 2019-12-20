#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <iterator>
#include <random>
#include <getopt.h>

#include "read_mat.hpp"

using namespace std;

int main(int argc, char** argv) {
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

  while (true) {
    static int c;
    static struct option long_options[] =
      {
       {"output", required_argument,  0, 'o'},
       {"not_prob", required_argument, 0, 'p'},
       {"max_genes",  required_argument, 0, 'g'},
       {"num_forms",  required_argument, 0, 'n'},
       {"num_literals",    required_argument, 0, 'l'},
       {"num_clauses",    required_argument, 0, 'c'},
       {0, 0, 0, 0}
      };
    int option_index = 0;

    c = getopt_long (argc, argv, "c:o:p:g:n:l:",
		     long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
        break;

    switch (c) {
    case 'o':
      output = string(optarg);
      break;
    case 'p':
      not_prob = atof(optarg);
      break;
    case 'g':
      max_gene = atoi(optarg);
      break;
    case 'n':
      num_forms = atoi(optarg);
      break;
    case 'l':
      clause_high = atoi(optarg);
      break;
    case 'c':
      num_clauses = atoi(optarg);
      break;
    case '?':
      /* getopt_long already printed an error message. */
      break;
    default:
      std::exit(2);
    }
  }
  cout << "num_forms: " << num_forms << "\nmax_gene: " << max_gene
       << "\nnot_prob: " << not_prob << "\nclause_high: " << clause_high <<'\n';
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
