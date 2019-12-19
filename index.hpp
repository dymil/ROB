#include <vector>
#include <string>
#include <set>
#include <unordered_map>
using namespace std;

class Index {
public:
    Index(const vector<vector<bool> >&, const vector<string>&, const vector<string>&);
    vector<int> clause_helper(const vector<pair<bool,string> >&, vector<int>&, const vector<int>&, int) const;
    vector<int> next_clause(const vector<vector<pair<bool,string> > >&, const vector<int> &, int) const;
    vector<int> first_clause(const vector<vector<pair<bool,string> > >&) const;

private:
    vector<vector<bool> > matrix;
    vector<string> geneNames;
    vector<string> cellNames;
    unordered_map<string, int> geneNamesToIdx;
    unordered_map<string, int> cellNamesToIdx;
    vector<pair<vector<int>, vector<int> > > expressed;
    bool gene_comparator(const pair<bool,string>&, const pair<bool,string>&) const;
    bool clause_comparator(const vector<pair<bool,string>>&, const vector<pair<bool,string>>&) const;
    void sort_clause(vector<pair<bool,string> >&) const;
    void sort_formula(vector<vector<pair<bool,string> > >&) const;
};
