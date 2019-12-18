#include <set>
#include <vector>
#include <string>
#include <unordered_map>

using std::vector, std::set, std::string, std::unordered_map;

class Index {
  vector<vector<bool>> matrix;
  vector<string> geneNames;
  vector<string> cellNames;
  unordered_map<string, int> geneNamesToIdx;
  unordered_map<string, int> cellNamesToIdx;  
  
public:
  Index(const vector<vector<bool>>& mat, const vector<string> geneNames, const vector<string> cellNames): matrix(mat), geneNames(geneNames), cellNames(cellNames) {
    
  }



}
