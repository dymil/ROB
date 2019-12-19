#include <vector>
#include <string>
#include <set>
#include <unordered_map>

class Index {
public:
    Index(const vector<vector<bool>>&, const vector<string>&, const vector<string>&);
    vector<int>* clause_helper(vector<pair<bool,string> >*, vector<int>*,vector<int>*, int);
    vector<int> next_clause(vector<vector<pair<bool,string> > >*, vector<int> *, int);
    vector<int> first_clause(vector<vector<pair<bool,string> > >*)

private:
    vector<vector<bool> > matrix;
    vector<string> geneNames;
    vector<string> cellNames;
    unordered_map<string, int> geneNamesToIdx;
    unordered_map<string, int> cellNamesToIdx;
    vector<pair<set<int>, set<int> > > expressed;
};
