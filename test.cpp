#include <vector>
#include <iostream>

#include "index.cpp"
using std::vector, std::cout;

int main(void) {
  vector<vector<bool>> mat{{true, false, true}, {true, true, true}};
  vector<string> cellNames{"a", "b", "c"};
  vector<string> geneNames{"1", "2"};
  Index x(mat, geneNames, cellNames);
  cout << "cell names\n";
  for (const auto& p : x.cellNamesToIdx)
    cout << p.first << ' ' << p.second << '\t';
  cout << "\ngene names:\n";
  for (const auto& p : x.geneNamesToIdx)
    cout << p.first << ' ' << p.second << '\t';
  for (const auto& g : x.expressed) {
    cout << "\nexpressed\n";
    for (const auto& cell : g.first)
      cout << cell << ' ';
    cout << "\nnot expressed\n";
    for (const auto& cell : g.second)
      cout << cell << ' ';    
  }
}
