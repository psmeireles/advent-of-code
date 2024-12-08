
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void ReadFile(vector<tuple<long long, vector<long long>>>& operations)
{
    ifstream file("input.txt");
    string line;
    while (getline(file, line))
    {
        auto colonIndex = line.find(':');
        auto resultAsString = line.substr(0, colonIndex);
        long long result = stoll(resultAsString);
        stringstream ss(line.substr(colonIndex + 1));
        vector<long long> operands;
        long long operand;
        while (ss >> operand)
        {
            operands.push_back(operand);
        }
        operations.push_back({result, operands});
    }
}

long long Concat(long long a, long long b)
{
    string s1 = to_string(a), s2 = to_string(b);
    return stoll(s1 + s2);
}

set<long long> GetResults(vector<long long>& v, long long expectedResult, bool canConcat)
{
    set<long long> results;
    if (v.size() ==  2)
    {
        long long sum = v[0] + v[1];
        if (sum <= expectedResult)
        {
            results.insert(sum);
        }
        
        long long mul = v[0] * v[1];
        if (mul <= expectedResult)
        {
            results.insert(mul);
        }

        if (canConcat)
        {
            long long concat = Concat(v[0], v[1]);
            if (concat <= expectedResult)
            {
                results.insert(concat);
            }
        }
        
        return results;
    }

    long long last = v.back();
    auto otherElements = vector<long long>(v.begin(), v.end() - 1);
    auto previousResults = GetResults(otherElements, expectedResult, canConcat);
    for (auto result : previousResults)
    {
        long long sum = result + last;
        if (sum <= expectedResult)
        {
            results.insert(sum);
        }

        long long mul = result * last;
        if (mul <= expectedResult)
        {
            results.insert(mul);
        }

        if (canConcat)
        {
            long long concat = Concat(result, last);
            if (concat <= expectedResult)
            {
                results.insert(concat);
            }
        }
    }
    return results;
}

int main(int argc, char* argv[])
{
    vector<tuple<long long, vector<long long>>> operations;
    ReadFile(operations);

    long long result1 = 0, result2 = 0;
    int i = 1;
    for (auto op : operations)
    {
        cout << "Calculating operation " << i << " of " << operations.size() << endl;
        i++;
        
        auto expectedResult = get<0>(op);
        auto operands = get<1>(op);
        auto results = GetResults(operands, expectedResult, false);
        if (results.find(expectedResult) != results.end())
        {
            result1 += expectedResult;
        }

        results = GetResults(operands, expectedResult, true);
        if (results.find(expectedResult) != results.end())
        {
            result2 += expectedResult;
        }
    }

    // Part1
    cout << result1 << endl;

    // Part2
    cout << result2 << endl;
    
    return 0;
}
