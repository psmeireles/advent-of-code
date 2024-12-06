#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void ReadFile(vector<string>& grid)
{
    ifstream file("input.txt");
    string line;
    while (getline(file, line))
    {
        grid.push_back(line);
    }
}

int SearchXMAS(vector<string>& grid, int i, int j)
{
    const string shouldMatch = "XMAS";
    const vector<vector<int>> steps{
        {1, 0},     // Vertical - Forward
        {-1, 0},    // Vertical - Backward
        {0, 1},     // Horizontal - Backward
        {0, -1},    // Horizontal - Backward
        {1, 1},     // Diagonal Right Down
        {1, -1},    // Diagonal Left Down
        {-1, 1},    // Diagonal Right Up
        {-1, -1}    // Diagonal Left Up
    };

    int matchesFound = 0;

    for (auto step : steps)
    {
        int stepI = step[0], stepJ = step[1];
        int index = 0, searchI = i, searchJ = j;
        while (index < shouldMatch.length()
            && searchI < grid.size() && searchI >= 0
            && searchJ < grid[searchI].size() && searchJ >= 0)
        {
            if (grid[searchI][searchJ] != shouldMatch[index])
            {
                break;
            }
            index++;
            searchI += stepI;
            searchJ += stepJ;
        }
        if (index == shouldMatch.length())
        {
            matchesFound++;
        }
    }

    return matchesFound;
}

int SearchXShapedMAS(vector<string>& grid, int i, int j)
{
    if (i + 2 >= grid.size() || j + 2 >= grid[i].size())
    {
        return 0;
    }
    
    vector<string> stringsToMatch{
        "M*M"
        "*A*"
        "S*S",
        "S*M"
        "*A*"
        "S*M",
        "M*S"
        "*A*"
        "M*S",
        "S*S"
        "*A*"
        "M*M"
    };

    const vector<vector<int>> steps{
        {0, 0},
        {0, 2},
        {1, 1},
        {2, 0},
        {2, 2},
    };

    for (auto stringToMatch : stringsToMatch)
    {
        int stringIndex = 0;
        for (auto step : steps)
        {
            int x = i + step[0], y = j + step[1];
            auto inGrid = grid[x][y], inString = stringToMatch[stringIndex];
            if (inGrid != inString)
            {
                // Not matching. Try next pattern.
                break;
            }
            stringIndex += 2;
        }
        if (stringIndex >= stringToMatch.length())
        {
            // Found. Return.
            return 1;
        }
    }
    return 0;
}

int main(int argc, char* argv[])
{
    vector<string> grid;
    ReadFile(grid);

    const vector<vector<int>> steps{
            {1, 0},
            {-1, 0},
            {0, 1},
            {0, -1},
            {1, 1},
            {1, -1},
            {-1, 1},
            {-1, -1}
    };
    
    int result1 = 0, result2 = 0;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == 'X')
            {
                result1 += SearchXMAS(grid, i, j);
            }
            
            if (grid[i][j] == 'M' || grid[i][j] == 'S')
            {
                if (SearchXShapedMAS(grid, i, j))
                {
                    cout << "Found XShapedMAS in " << "i = " << i << ", j = " << j << " : " << grid[i][j] << endl;
                    result2++;
                }
            }
        }
    }
    
    // Part 1
    cout << result1 << endl;
    
    // Part 2
    cout << result2 << endl;
    return 0;
}
