#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>
using namespace std;

class Robot
{
    public:
    int Position[2];
    int Velocity[2];
};

ostream& operator<<(ostream& os, const Robot& robot)
{
    os << "Position: " << robot.Position[0] << ", " << robot.Position[1] << ". Velocity: " << robot.Velocity[0] << ", " << robot.Velocity[1] << endl;
    return os;
}

void ReadFile(vector<Robot>& robots)
{
    ifstream file("input.txt");
    string line;
    while (getline(file, line))
    {
        Robot r;
        sscanf_s(line.c_str(), "p=%d,%d v=%d,%d", &r.Position[1], &r.Position[0],  &r.Velocity[1], &r.Velocity[0]);
        robots.push_back(r);
    }
}

vector<int> GetPositionAfterSteps(Robot robot, int steps, vector<int> gridLimits)
{
    vector<int> position;
    int limitX = gridLimits[0], limitY = gridLimits[1];
    
    int x = robot.Position[0] + steps * robot.Velocity[0];
    // To account for possible negative positions, instead of doing x % limitX, doing ((x % limitX) + limitX) % limitX
    position.push_back(((x % limitX) + limitX) % limitX);
    
    int y = robot.Position[1] + steps * robot.Velocity[1];
    // To account for possible negative positions, instead of doing y % limitY, doing ((y % limitY) + limitY) % limitY
    position.push_back(((y % limitY) + limitY) % limitY);
    return position;
}

void PrintGrid(vector<vector<int>> robots, vector<int> gridLimits)
{
    vector<vector<int>> grid(gridLimits[0], vector<int>(gridLimits[1], 0));
    for (auto robot : robots)
    {
        grid[robot[0]][robot[1]]++;
    }
    for (int i = 0; i < gridLimits[0]; i++)
    {
        for (int j = 0; j < gridLimits[1]; j++)
        {
            cout << (grid[i][j] == 0 ? "." : to_string(grid[i][j]));
        }
        cout << endl;
    }
}

int MultiplyQuadrants(vector<vector<int>> robots, vector<int> gridLimits)
{
    vector<vector<int>> grid(gridLimits[0], vector<int>(gridLimits[1], 0));
    for (auto robot : robots)
    {
        grid[robot[0]][robot[1]]++;
    }
    
    // Top left
    int topLeft = 0;
    for (int i = 0; i < gridLimits[0]/2; i++)
    {
        for (int j = 0; j < gridLimits[1]/2; j++)
        {
            topLeft += grid[i][j];
        }
    }

    int topRight = 0;
    for (int i = 0; i < gridLimits[0]/2; i++)
    {
        for (int j = gridLimits[1]/2 + 1; j < gridLimits[1]; j++)
        {
            topRight += grid[i][j];
        }
    }

    int bottomLeft = 0;
    for (int i = gridLimits[0]/2 + 1; i < gridLimits[0]; i++)
    {
        for (int j = 0; j < gridLimits[1]/2; j++)
        {
            bottomLeft += grid[i][j];
        }
    }

    int bottomRight = 0;
    for (int i = gridLimits[0]/2 + 1; i < gridLimits[0]; i++)
    {
        for (int j = gridLimits[1]/2 + 1; j < gridLimits[1]; j++)
        {
            bottomRight += grid[i][j];
        }
    }

    cout << "topLeft: " << topLeft << endl;
    cout << "topRight: " << topRight << endl;
    cout << "bottomLeft: " << bottomLeft << endl;
    cout << "bottomRight: " << bottomRight << endl;
    
    return topLeft * topRight * bottomLeft * bottomRight;
}

int GetLargestConnectedComponentSize(set<vector<int>> robots, vector<int> gridLimits)
{
    set<vector<int>> visited;

    int largest = 0;

    queue<vector<int>> q;
    
    for (auto position : robots)
    {
        int currentComponentSize = 0;
        q.push(position);
        while (!q.empty())
        {
            auto curr = q.front();
            q.pop();
            if (visited.find(curr) != visited.end())
            {
                continue;
            }

            visited.insert(curr);
            currentComponentSize++;

            // Find neighbors
            auto up = {curr[0] - 1, curr[1]};
            if (robots.find(up) != robots.end())
            {
                q.push(up);
            }

            auto down = {curr[0] + 1, curr[1]};
            if (robots.find(down) != robots.end())
            {
                q.push(down);
            }

            auto left = {curr[0], curr[1] - 1};
            if (robots.find(left) != robots.end())
            {
                q.push(left);
            }
            
            auto right = {curr[0], curr[1] + 1};
            if (robots.find(right) != robots.end())
            {
                q.push(right);
            }
        }
        largest = max(largest, currentComponentSize);
    }

    return largest;
}

int main(int argc, char* argv[])
{
    vector<Robot> robots;
    vector<int> gridLimits{103, 101};
    ReadFile(robots);

    // Part 1 - Answer: 229868730
    {
        vector<vector<int>> positions;
        for (auto robot : robots)
        {
            vector<int> position = GetPositionAfterSteps(robot, 100, gridLimits);
            positions.push_back(position);
        }
        // PrintGrid(positions, gridLimits);
        cout << "Result1: " << MultiplyQuadrants(positions, gridLimits) << endl << endl;
    }
    
    // Part 2 - Answer: 7861
    {
        int largestFound = 0;
        for (int i = 1; i < gridLimits[0] * gridLimits[1]; i++)
        {
            set<vector<int>> positions;
            for (auto robot : robots)
            {
                vector<int> position = GetPositionAfterSteps(robot, i, gridLimits);
                positions.insert(position);
            }

            // vector<vector<int>> positionsV(positions.begin(), positions.end());
            // PrintGrid(positionsV, gridLimits);
            // cout << endl;

            // Find the largest connected component. If it's bigger than 50, it's probably the christmas tree.
            auto largestComponentSize = GetLargestConnectedComponentSize(positions, gridLimits);
            largestFound = max(largestFound, largestComponentSize);
            if (largestComponentSize > 50)
            {
                cout << "Result2: "  << i << endl;
                break;
            }

            if (i % 100 == 0)
            {
                cout << "Finished iteration " << i << ". Largest found: " << largestFound << endl;
            }
        }
    }
    
    return 0;
}
