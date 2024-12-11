#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
using namespace std;

void ReadFile(vector<long long>& stones)
{
    ifstream file("input.txt");
    string line;
    while (getline(file, line))
    {
        stringstream ss;
        ss << line;
        int number;
        while (ss >> number)
        {
            stones.push_back(number);
        }
    }
}

int CountDigits(long long number)
{
    int digits = 1;
    while (number >= 10)
    {
        number /= 10;
        digits++;
    }
    return digits;
}

long long Transform(long long number, long long& outSecondElement)
{
    if (number == 0)
    {
        return 1;
    }

    const int digits = CountDigits(number);
    if (digits % 2 == 0)
    {
        const long long half = static_cast<int>(pow(10, digits / 2));
        outSecondElement = number % half;
        return number / half;
    }

    return number * 2024;
}

int main(int argc, char* argv[])
{
    vector<long long> stones;
    ReadFile(stones);

    map<long long, long long> numberCount;
    
    for (auto number : stones)
    {
        numberCount[number]++;
    }
    
    long long result1 = 0, result2 = 0;
    for (int iteration = 1; iteration <= 75; iteration++)
    {
        map<long long, long long> newMap;
        for (const auto kvp : numberCount)
        {
            long long transform, second = -1;
            transform = Transform(kvp.first, second);
            newMap[transform] += kvp.second;
            if (second != -1)
            {
                newMap[second] += kvp.second;
            }
        }
        numberCount = newMap;

        // Part 1
        if (iteration == 25)
        {
            for (const auto kvp : numberCount)
            {
                result1 += kvp.second;
            }
        }
        
        // Part 2
        if (iteration == 75)
        {
            for (const auto kvp : numberCount)
            {
                result2 += kvp.second;
            }
        }
    }
    
    cout << result1 << endl;
    cout << result2 << endl;
    return 0;
}
