#include <vector>
#include <utility>
#include <string>
#include <unordered_map>

class Naive {
  std::vector<std::vector<bool>> matrix; // genes are rows, cells are columns
  std::vector<std::string> geneNames;
  std::vector<std::string> cellNames;
  std::unordered_map<std::string, int> geneNamesToIdx;
  std::unordered_map<std::string, int> cellNamesToIdx;

public:
  Naive(const std::vector<std::vector<bool>>& mat, const std::vector<std::string>& geneNames, const std::vector<std::string>& cellNames);
  
  /* Returns cell indices which satisfy the query. Checks each cell individually */
  std::vector<int> naive(std::vector<std::vector<std::pair<bool, std::string>>> query);
};
