#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>
using namespace std;

void ReadFile(vector<string>& m, string& moves)
{
    ifstream file("input.txt");
    string line;
    while(getline(file, line))
    {
        if (line.empty())
        {
            break;
        }
        m.push_back(line);
    }

    getline(file, moves);
}

void FindRobotWallsAndBoxes(vector<string>& m, vector<int>& robot, set<vector<int>>& walls, set<vector<int>>& boxes)
{
    for (int i = 0; i < m.size(); i++)
    {
        for (int j = 0; j < m[i].length(); j++)
        {
            switch(m[i][j])
            {
                case '#':
                    walls.insert({i, j});
                    break;
                case '@':
                    robot = {i, j};
                    break;
                case 'O':
                    boxes.insert({i, j});
                    break;
                default: ;
            }
        }
    }
}

void FindRobotWallsAndBoxes2(vector<string>& m, vector<int>& robot, set<vector<int>>& walls, set<vector<int>>& boxes)
{
    for (int i = 0; i < m.size(); i++)
    {
        for (int j = 0; j < m[i].length(); j++)
        {
            switch(m[i][j])
            {
            case '#':
                walls.insert({i, 2*j});
                walls.insert({i, 2*j +1});
                break;
            case '@':
                robot = {i, 2*j};
                break;
            case 'O':
                boxes.insert({i, 2*j});
                break;
            default: ;
            }
        }
    }
}

void PerformStep(char step, vector<int>& robot, set<vector<int>>& walls, set<vector<int>>& boxes)
{
    map<char, vector<int>> charToDir{
        {'<', {0, -1}},
        {'>', {0, 1}},
        {'^', {-1, 0}},
        {'v', {1, 0}},
    };
    
    auto dir = charToDir[step];
    auto newPos = {robot[0] + dir[0], robot[1] + dir[1]};
    
    if (walls.find(newPos) != walls.end())
    {
        // Trying to walk into a wall. Do nothing.
        return;
    }
    
    if (boxes.find(newPos) == boxes.end())
    {
        // Empty space. Just walk.
        robot = newPos;
        return;
    }
    
    // Facing a box. Check if it can be pushed.
    vector<int> pos = newPos;
    stack<vector<int>> boxesToPush;
    while (boxes.find(pos) != boxes.end())
    {
        boxesToPush.push(pos);
        pos = {pos[0] + dir[0], pos[1] + dir[1]};
    }
    
    if (walls.find(pos) != walls.end())
    {
        // Found a wall after all blocks. Can't move.
        return;
    }
    
    // Found a free space. Push all blocks and move robot.
    while (!boxesToPush.empty())
    {
        auto box = boxesToPush.top();
        boxesToPush.pop();
        boxes.erase(box);
        
        auto newBox = {box[0] + dir[0], box[1] + dir[1]};
        boxes.insert(newBox);
    }
    robot = newPos;
}

void PerformStep2(char step, vector<int>& robot, set<vector<int>>& walls, set<vector<int>>& boxes)
{
    map<char, vector<int>> charToDir{
            {'<', {0, -1}},
            {'>', {0, 1}},
            {'^', {-1, 0}},
            {'v', {1, 0}},
        };
    
    auto dir = charToDir[step];
    vector<int> newPos = {robot[0] + dir[0], robot[1] + dir[1]};
    
    if (walls.find(newPos) != walls.end())
    {
        // Trying to walk into a wall. Do nothing.
        return;
    }

    // Boxes start in (x, y), but also occupy (x, y+1), so we have to check if there's a box in either (x, y) or (x, y-1)
    if (boxes.find(newPos) == boxes.end() && boxes.find({newPos[0], newPos[1]-1}) == boxes.end())
    {
        // Empty space. Just walk.
        robot = newPos;
        return;
    }
    
    // Facing a box. Check if it can be pushed.
    // Keep track of all boxes that will be pushed. Using a stack as we need to push the last one found first.
    stack<vector<int>> boxesToPush;
    if (step == '>')
    {
        vector<int> pos = newPos;
        // Horizontal move. Can only push boxes in the same row.
        while (boxes.find(pos) != boxes.end())
        {
            boxesToPush.push(pos);
            // Look 2 positions forward, because a box occupies 2 spaces horizontally
            pos = {pos[0], pos[1] + 2 * dir[1]};
        }

        // No more boxes. We either found an empty space or a wall.
        if (walls.find(pos) != walls.end())
        {
            // Found a wall after all blocks. Can't move.
            return;
        }
    }
    else if (step == '<')
    {
        vector<int> pos = {newPos[0], newPos[1] - 1};
        // Horizontal move. Can only push boxes in the same row.
        while (boxes.find(pos) != boxes.end())
        {
            boxesToPush.push(pos);
            // Look 2 positions forward, because a box occupies 2 spaces horizontally
            pos = {pos[0], pos[1] + 2 * dir[1]};
        }

        // No more boxes. We're now 2 blocks away from the last box. Walk back one to the right and check if it's empty or if it's a wall.
        if (walls.find({pos[0], pos[1] + 1}) != walls.end())
        {
            // Found a wall after all blocks. Can't move.
            return;
        }
    }
    else
    {
        queue<vector<int>> positionsToCheck;
        positionsToCheck.push(newPos);
        // Vertical move. One box may end up pushing other 2 boxes.
        while (!positionsToCheck.empty())
        {
            auto position = positionsToCheck.front();
            positionsToCheck.pop();

            // We tried to push and hit a wall. Don't move and return.
            if (walls.find(position) != walls.end())
            {
                return;
            }

            // Check if there's a box overlapping with this position. If there is, it will have to be moved and both its new positions will have to be checked.
            // Box starting here
            if (boxes.find(position) != boxes.end())
            {
                boxesToPush.push(position);
                vector<int> pushedInto1 = {position[0] + dir[0], position[1]};
                vector<int> pushedInto2 = {position[0] + dir[0], position[1] + 1};
                positionsToCheck.push(pushedInto1);
                positionsToCheck.push(pushedInto2);
            }

            // Box starting 1 position to the left
            vector<int> positionLeft = {position[0], position[1] - 1};
            if (boxes.find(positionLeft) != boxes.end())
            {
                boxesToPush.push(positionLeft);
                vector<int> pushedInto1 = {positionLeft[0] + dir[0], positionLeft[1]};
                vector<int> pushedInto2 = {positionLeft[0] + dir[0], positionLeft[1] + 1};
                positionsToCheck.push(pushedInto1);
                positionsToCheck.push(pushedInto2);
            }
        }
    }
    
    // Found a free space. Push all blocks and move robot.
    while (!boxesToPush.empty())
    {
        auto box = boxesToPush.top();
        boxesToPush.pop();
        boxes.erase(box);
        
        auto newBox = {box[0] + dir[0], box[1] + dir[1]};
        boxes.insert(newBox);
    }
    robot = newPos;
}

void PrintMap2(vector<string>& m, vector<int>& robot, set<vector<int>>& walls, set<vector<int>>& boxes)
{
    int sizeX = m.size(), sizeY = 2 * m[0].length();

    for (int i = 0; i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {
            if (robot[0] == i && robot[1] == j)
            {
                cout << '@';
            }
            else if (walls.find({i, j}) != walls.end())
            {
                cout << '#';
            }
            else if (boxes.find({i, j}) != boxes.end())
            {
                cout << '[';
            }
            else if (boxes.find({i, j - 1}) != boxes.end())
            {
                cout << ']';
            }
            else
            {
                cout << '.';
            }
        }
        cout << endl;
    }
}

int main()
{
    vector<string> m;
    string moves;
    ReadFile(m, moves);

    // Part 1 - Answer: 1371036
    {
        vector<int> robot;
        set<vector<int>> boxes, walls;
        FindRobotWallsAndBoxes(m, robot, walls, boxes);
        
        for (auto direction : moves)
        {
            PerformStep(direction, robot, walls, boxes);
        }

        int result1 = 0;
        for (auto box : boxes)
        {
            result1 += 100 * box[0] + box[1];
        }

        cout << "Result1: " << result1 << endl; 
    }

    // Part 2 - Answer: 1392847
    {
        vector<int> robot;
        set<vector<int>> boxes, walls;
        FindRobotWallsAndBoxes2(m, robot, walls, boxes);

        // PrintMap2(m, robot, walls, boxes);

        // int i = 0;
        for (auto direction : moves)
        {
            // cout << endl;
            // cout << "Step: " << i++ << ". Direction: " << direction << endl; 
            PerformStep2(direction, robot, walls, boxes);
            // PrintMap2(m, robot, walls, boxes);
        }

        int result2 = 0;
        for (auto box : boxes)
        {
            result2 += 100 * box[0] + box[1];
        }

        cout << "Result2: " << result2 << endl; 
    }
    
    return 0;
}