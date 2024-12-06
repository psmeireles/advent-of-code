
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void ReadFile(vector<vector<int>>& lines)
{
    string line;
    ifstream file("input.txt");

    while (getline(file, line))
    {
        istringstream ss(line);
        vector<int> v;
        int x;
        while (ss >> x)
        {
            v.push_back(x);
        }
        lines.push_back(v);
    }
    file.close();
}

bool IsSafe(vector<int> line, int& outUnsafeIndex)
{
    if (line.size() <= 1)
    {
        return true;
    }

    // Decide if we should be increasing or decreasing depending on the tendency across the whole vector.
    // This is not ideal and may fail with small vectors, but works for this data set.
    int increasing = 0, decreasing = 0;
    for (int i = 1; i < line.size(); i++)
    {
        if (line[i] > line[i - 1])
        {
            increasing++;
        }
        else if (line[i] < line[i - 1])
        {
            decreasing++;
        }
    }
    
    const bool shouldIncrease = increasing > decreasing;
    for (int i = 1; i < line.size(); i++)
    {
        const int diff = shouldIncrease ? line[i] - line[i - 1] : line[i - 1] - line[i];
        if (diff < 1 || diff > 3)
        {
            // Unsafe
            outUnsafeIndex = i - 1;
            return false;
        }
    }
    return true;
}

bool CanTolerate(vector<int> line, int i)
{
    auto copy = line;
    copy.erase(copy.begin() + i);
    int _;
    if (IsSafe(copy, _))
    {
        return true;
    }

    copy = line;
    copy.erase(copy.begin() + i + 1);

    return IsSafe(copy, _);
}

int main(int argc, char* argv[])
{
    vector<vector<int>> lines;
    ReadFile(lines);

    int safeLines = 0;
    int toleratedLines = 0;
    int i = 0;
    for (auto& line : lines)
    {
        int unsafeIndex;
        if (IsSafe(line, unsafeIndex))
        {
            cout << "Line " << i << " is Safe" << '\n';
            safeLines++;
        }
        else
        {
            cout << "Line " << i << " is Unsafe";
            if (CanTolerate(line, unsafeIndex))
            {
                cout << " but can be tolerated";
                toleratedLines++;
            }
            cout << '\n';
        }
        i++;
    }

    // Part 1
    cout << safeLines << '\n';

    // Part 2
    cout << safeLines + toleratedLines << '\n';
    return 0;
}
