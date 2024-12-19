
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
using namespace std;

void ReadFile(vector<string>& towels, vector<string>& patterns)
{
    ifstream file("input.txt");
    string line;
    getline(file, line);
    while (line.find(',') != line.npos)
    {
        auto comma = line.find(',');
        auto towel = line.substr(0, comma);
        towels.push_back(towel);
        // Skip comma and space after
        line = line.substr(comma + 2);
    }
    // Consumed last comma. Only a towel remaining.
    towels.push_back(line);

    // Consume empty line
    getline(file, line);

    while (getline(file, line))
    {
        patterns.push_back(line);
    }
}

vector<int> FindAllOccurences(string sample, string findIt)
{
    vector<int> indexes;
    for (int i = 0; i < sample.length(); i++) {
        if (sample.substr(i, findIt.length()) == findIt) {
            indexes.push_back(i);
        }
    }
    return indexes;
}

long long CountWays(const string& goal, const vector<string>& towels, map<string, long long>& memo) {
    if (memo.find(goal) != memo.end())
    {
        return memo[goal];
    }

    if (goal.empty())
    {
        memo[goal] = 1;
        return 1;
    }

    long long result = 0;
    for (auto towel : towels)
    {
        auto goalStart = goal.substr(0, towel.size());
        if (goalStart == towel)
        {
            auto rest = goal.substr(towel.size());
            result += CountWays(rest, towels, memo);
        }
    }

    memo[goal] = result;
    return result;
}


int main(int argc, char* argv[])
{
    vector<string> towels, patterns;
    ReadFile(towels, patterns);

    // Sort by size
    sort(towels.begin(), towels.end(), [](string a, string b)
    {
        return a.length() > b.length();
    });
    
    long long result1 = 0, result2 = 0;
    for (const auto& p : patterns)
    {
        map<string, long long> memo;
        auto result = CountWays(p, towels, memo);
        if (result)
        {
            result1++;
        }
        // cout << p << ": " << result << endl;
        result2 += result;
    }

    // Part 1 - Answer: 206
    cout << "Part 1: " << result1 << endl;

    // Part 2 - Answer: 622121814629343
    cout << "Part 2: " << result2 << endl;

    return 0;
}
