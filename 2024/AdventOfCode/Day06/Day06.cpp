
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
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

void FindStart(vector<string>& grid, vector<int>& position, vector<int>& direction)
{
    map<char, vector<int>> charToDirection{
        {'^', {-1, 0}},
    {'>', {0, 1}},
    {'<', {0, -1}},
    {'v', {1, 0}}
    };
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (charToDirection.find(grid[i][j]) != charToDirection.end())
            {
                position[0] = i;
                position[1] = j;
                
                auto dir = charToDirection[grid[i][j]];
                direction[0] = dir[0];
                direction[1] = dir[1];
                return;
            }
        }
    }
}

bool IsInsideGrid(vector<string>& grid, int i, int j)
{
    return i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size();
}

vector<int> RotateRight(vector<int> direction)
{
    vector<int> newDirection;
    if (direction[0] == -1 && direction[1] == 0)
    {
        // Up -> Right
        newDirection.push_back(0);
        newDirection.push_back(1);
    }
    else if (direction[0] == 0 && direction[1] == 1)
    {
        // Right -> Down
        newDirection.push_back(1);
        newDirection.push_back(0);
    }
    else if (direction[0] == 1 && direction[1] == 0)
    {
        // Down -> Left
        newDirection.push_back(0);
        newDirection.push_back(-1);
    }
    else if (direction[0] == 0 && direction[1] == -1)
    {
        // Left -> Up
        newDirection.push_back(-1);
        newDirection.push_back(0);
    }
    return newDirection;
}

vector<int> RotateLeft(vector<int>& direction)
{
    return RotateRight(RotateRight(RotateRight(direction)));
}

vector<int> operator+(const vector<int>& lhs, const vector<int>& rhs)
{
    vector<int> result;
    for (int i = 0; i < lhs.size(); i++)
    {
        result.push_back(lhs[i] + rhs[i]);
    }
    return result;
}

vector<int> operator-(const vector<int>& lhs, const vector<int>& rhs)
{
    vector<int> result;
    for (int i = 0; i < lhs.size(); i++)
    {
        result.push_back(lhs[i] - rhs[i]);
    }
    return result;
}

bool HasObstacleInDirection(vector<string>& grid, vector<int>& start, vector<int>& direction, int steps)
{
    int i = start[0], j = start[1];
    while (i < grid.size() && j < grid[i].size()
        && i > 0 && j > 0
        && steps > 0)
    {
        // Borders are invalid as we can't place an obstacle outside of the grid
        if (abs(direction[0]) > 0 && (i == 0 || i == grid.size() - 1))
        {
            return true;
        }
        if (abs(direction[1]) > 0 && (j == 0 || j == grid[i].size() - 1))
        {
            return true;
        }
        if (grid[i][j] == '#')
        {
            // Found obstacle
            return true;
        }
        i += direction[0];
        j += direction[1];
        steps--;
    }
    return steps != 0;
}

class Collision
{
    public:
    vector<int> direction;
    vector<int> position;
    vector<int> GetObstaclePosition()
    {
        return position + direction;
    }
};

int main(int argc, char* argv[])
{
    vector<string> grid;
    ReadFile(grid);
    
    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));
    set<vector<int>> visitedPos;
    vector<int> start(2, 0), direction(2, 0); 
    FindStart(grid, start, direction);

    int result1 = 0;
    while(IsInsideGrid(grid, start[0], start[1]))
    {
        int x = start[0], y = start[1];
        if (!visited[x][y])
        {
            visited[x][y] = true;
            visitedPos.insert(start);
            result1++;
        }

        int newX = x + direction[0];
        int newY = y + direction[1];
        if (!IsInsideGrid(grid, newX, newY))
        {
            break;
        }

        if (grid[newX][newY] == '#')
        {
            direction = RotateRight(direction);
        }
        else
        {
            start[0] = newX;
            start[1] = newY;
        }
    }
    
    // Part 1
    cout << result1 << endl;

    // Part 2. Brute forcing, trying to place an obstacle in each position and checking if it creates a loop.
    vector<int> newStart(2, 0), newDirection(2, 0);
    FindStart(grid, newStart, newDirection);
    visitedPos.erase(newStart);

    int result2 = 0;
    int i = 0;
    for (auto pos : visitedPos)
    {
        if (i % 100 == 0)
        {
            cout << "Trying possibility " << i + 1 << " of " << visitedPos.size() << endl;
        }
        i++;
        // Add obstacle
        auto oldChar = grid[pos[0]][pos[1]];
        grid[pos[0]][pos[1]] = '#';
        start = newStart;
        direction = newDirection;
        
        set<string> visitedPosDir;
        while(IsInsideGrid(grid, start[0], start[1]))
        {
            stringstream ss;
            ss << start[0] << "," << start[1] << "-" << direction[0] << "," << direction[1];
            string posDir = ss.str();
            if (visitedPosDir.find(posDir) == visitedPosDir.end())
            {
                visitedPosDir.insert(posDir);
            }
            else
            {
                // Found loop. Stop.
                // cout << "Create loop with obstacle in " << pos[0] << ", " << pos[1] << endl;
                result2++;
                break;
            }

            int x = start[0], y = start[1];
            int newX = x + direction[0];
            int newY = y + direction[1];
            if (!IsInsideGrid(grid, newX, newY))
            {
                break;
            }

            if (grid[newX][newY] == '#')
            {
                direction = RotateRight(direction);
            }
            else
            {
                start[0] = newX;
                start[1] = newY;
            }
        }

        // Restore character
        grid[pos[0]][pos[1]] = oldChar;
    }

    cout << result2 << endl;
    return 0;
}
