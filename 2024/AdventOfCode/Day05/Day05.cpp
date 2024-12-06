#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void ReadFile(map<int, set<int>>& rulesDict, vector<vector<int>>& updates)
{
    ifstream file("input.txt");
    string line;
    bool foundEmptyLine = false;
    while (getline(file, line))
    {
        if (line.empty())
        {
            foundEmptyLine = true;
            continue;
        }

        if (foundEmptyLine == false)
        {
            int first, second;
            int delimiterIndex = line.find('|');
            first = stoi(line.substr(0, delimiterIndex));
            second = stoi(line.substr(delimiterIndex + 1));
            rulesDict[first].insert(second);
        }
        else
        {
            stringstream ss(line);
            string separated;
            vector<int> update;
            while (getline(ss, separated, ','))
            {
                update.push_back(stoi(separated));
            }
            updates.push_back(update);
        }
    }
}

ostream& operator<<(ostream& os, vector<int>& a)
{
    for (int i = 0; i < a.size(); i++)
    {
        cout << a[i] << " ";
    }
    return os;
}

vector<int> Correct(vector<int>& update, map<int, set<int>>& rulesDict)
{
    vector<int> corrected = update;
    // Sort based on the dict comparison. If b is in a's dict, a should come first
    sort(corrected.begin(), corrected.end(), [rulesDict](int a, int b)
    {
        auto beforeCharIndex = rulesDict.at(a).find(b);
        // If not found, b can be after a
        return beforeCharIndex != rulesDict.at(a).end();
    });
    cout << "Fixed " << update << "\nwith " << corrected << endl << endl;
    return corrected;
}

int main(int argc, char* argv[])
{
    map<int, set<int>> rulesDict;
    vector<vector<int>> updates;
    ReadFile(rulesDict, updates);
    
    int result1 = 0, result2 = 0;
    for (auto& update : updates)
    {
        map<int, int> indexes;
        for (int i = 0; i < update.size(); i++)
        {
            indexes[update[i]] = max(indexes[update[i]], i);
        }

        bool isValid = true;
        for (int i = 0; i < update.size(); i++)
        {
            for (auto number : rulesDict[update[i]])
            {
                if (indexes.find(number) != indexes.end() && indexes[number] < i)
                {
                    // Found a forbidden number before this one. Stop.
                    isValid = false;
                    auto corrected = Correct(update, rulesDict);
                    result2 += corrected[corrected.size()/2];
                    break;
                }
            }
            if (!isValid)
            {
                break;
            }
        }

        if (isValid)
        {
            result1 += update[update.size()/2];
        }
    }

    // Part 1
    cout << result1 << endl;

    // Part 2
    cout << result2 << endl;
    return 0;
}
