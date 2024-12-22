
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void ReadFile(vector<long long>& secrets)
{
    ifstream file("input.txt");
    string line;
    while (getline(file, line))
    {
        secrets.push_back(stoll(line));
    }
}

long long Mix(long long value, long long secret)
{
    return value ^ secret;
}

long long Prune(long long secret)
{
    return secret % 16777216;
}

long long FindNextSecret(long long secret)
{
    // Multiply by 64 (2**6), mix and prune.
    auto step1 = Prune(Mix(secret << 6, secret));
    
    // Divide by 32 (2**5) round down, mix and prune
    auto step2 = Prune(Mix(step1 >> 5, step1));

    // Multiply by 2048 (2**11), mix and prune
    auto step3 = Prune(Mix(step2 << 11, step2));
    return step3;
}

int main(int argc, char* argv[])
{
    vector<long long> secrets;
    ReadFile(secrets);

    long long result1 = 0;
    map<string, int> sumOverAll;
    int result2 = 0;
    for (auto secret: secrets)
    {
        vector<int> differences, prices;
        auto currSecret = secret;
        auto currPrice = currSecret % 10;
        for (int i = 0; i < 2000; i++)
        {
            currSecret = FindNextSecret(currSecret);
            auto newPrice = currSecret % 10;
            differences.push_back(newPrice - currPrice);
            prices.push_back(newPrice);
            currPrice = newPrice;
        }

        map<string, int> sums;
        for (int i = 3; i < differences.size(); i++)
        {
            stringstream ss;
            ss << differences[i-3] << "," << differences[i-2] << "," << differences[i-1] << "," << differences[i]; 
            string key = ss.str();
            if (sums.find(key) == sums.end())
            {
                sums[key] = prices[i];
                sumOverAll[key] += prices[i];
                result2 = max(result2, sumOverAll[key]);
            }
        }
        
        
        result1 += currSecret;
    }

    // Part 1 - Answer: 14119253575
    cout << "Part 1: " << result1 << endl;

    // Part 2 - Answer: 1600
    cout << "Part 2: " << result2 << endl;
    
    return 0;
}
