
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

void ReadFile(vector<pair<string, string>>& connections)
{
    ifstream file("input.txt");
    string line;
    while (getline(file, line))
    {
        connections.emplace_back(line.substr(0, 2), line.substr(line.length() - 2));
    }
}

void BuildGraph(vector<pair<string, string>>& connections, map<string, set<string>>& graph)
{
    for (pair<string, string> connection : connections)
    {
        graph[connection.first].insert(connection.second);
        graph[connection.second].insert(connection.first);
    }
}

// BronKerbosch: Find all cliques (fully connected components) in a graph
void GetAllCliques(set<string> currentClique, set<string> potentialNodes, set<string> excludedNodes, map<string, set<string>>& graph, vector<set<string>>& cliques)
{
    if (potentialNodes.empty() && excludedNodes.empty())
    {
        cliques.push_back(currentClique);
        return;
    }

    auto potentialNodesCopy = potentialNodes;
    for (auto node : potentialNodesCopy)
    {
        auto newClique = currentClique;
        newClique.insert(node);

        auto neighbors = graph[node];
        set<string> newPotentialNodes, newExcludedNodes;
        for (auto neighbor : graph[node])
        {
            if (potentialNodes.find(neighbor) != potentialNodes.end())
            {
                newPotentialNodes.insert(neighbor);
            }
            if (excludedNodes.find(neighbor) != excludedNodes.end())
            {
                newExcludedNodes.insert(neighbor);
            }
        }
        
        GetAllCliques(newClique, newPotentialNodes, newExcludedNodes, graph, cliques);
        potentialNodes.erase(node);
        excludedNodes.insert(node);
    }
}

int Factorial(int n)
{
    int result = 1;
    while (n > 0)
    {
        result *= n;
        n--;
    }
    return result;
}

int Combination(int n, int k)
{
    return Factorial(n) / (Factorial(k) * Factorial(n - k)); 
}

// Generate all subsets of size 3 with at least one node starting with 't' from a given clique
vector<set<string>> GenerateSubsetsOfSize3WithT(const set<string>& clique) {
    vector<set<string>> subsets;
    vector<string> nodes(clique.begin(), clique.end());

    int n = nodes.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            for (int k = j + 1; k < n; ++k) {
                if (nodes[i][0] == 't' || nodes[j][0] == 't' || nodes[k][0] == 't')
                {
                    subsets.push_back({nodes[i], nodes[j], nodes[k]});
                }
            }
        }
    }
    return subsets;
}

int main(int argc, char* argv[])
{
    vector<pair<string, string>> connections;
    ReadFile(connections);
    
    map<string, set<string>> graph;
    BuildGraph(connections, graph);

    // Run BronKerbosch to find all cliques in graph
    vector<set<string>> cliques;
    set<string> currentClique, potentialNodes, excludedNodes;
    for (auto kvp : graph)
    {
        potentialNodes.insert(kvp.first);
    }
    GetAllCliques(currentClique, potentialNodes, excludedNodes, graph, cliques);

    // For each clique with at least 3 elements and one starting with 't', generate all permutations that satisfy
    set<set<string>> size3CliquesWithT;
    size_t maxSize = 0;
    for (auto clique : cliques)
    {
        auto size = clique.size();
        maxSize = max(maxSize, size);
        if (size < 3)
        {
            continue;
        }
        
        bool hasNodesStartingWithT = false;
        for (auto node : clique)
        {
            if (node[0] == 't')
            {
                hasNodesStartingWithT = true;
            }
        }

        if (!hasNodesStartingWithT)
        {
            continue;
        }

        auto allSize3CliquesWithT = GenerateSubsetsOfSize3WithT(clique);
        for (const auto& size3CliqueWithT : allSize3CliquesWithT)
        {
            size3CliquesWithT.insert(size3CliqueWithT);
        }
    }

    // Part 1 - Answer: 1240
    cout << "Part 1: " << size3CliquesWithT.size() << endl;
    
    set<string> maxClique;
    for (const auto& clique : cliques)
    {
        if (clique.size() == maxSize)
        {
            maxClique = clique;
            break;
        }
    }

    // Part 2 - Answer: am,aq,by,ge,gf,ie,mr,mt,rw,sn,te,yi,zb
    vector<string> clique(maxClique.begin(), maxClique.end());
    sort(clique.begin(), clique.end());
    cout << "Part 2: ";
    for (int i = 0; i < clique.size(); i++)
    {
        cout << clique[i] << (i < clique.size() - 1 ? "," : "\n");
    }
    
    return 0;
}
