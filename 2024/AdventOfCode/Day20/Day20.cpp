#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <string>
using namespace std;

class Position
{
public:
    int x;
    int y;
    Position() = default;
    Position(int _x, int _y) : x(_x), y(_y) {}

    // Necessary to use set
    bool operator<(const Position& rhs) const 
    {
        vector<int> a(2);
        a[0] = this->x;
        a[1] = this->y;
        vector<int> b(2);
        b[0] = rhs.x;
        b[1] = rhs.y;
        
        return a < b;
    }
};

Position operator+(Position a, Position b)
{
    return Position(a.x + b.x, a.y + b.y);
}

Position operator-(Position a, Position b)
{
    return Position(a.x - b.x, a.y - b.y);
}

bool operator==(Position a, Position b)
{
    return a.x == b.x && a.y == b.y;
}

ostream& operator<<(ostream& os, const Position& v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

void ReadFile(vector<string>& m)
{
    ifstream file("input.txt");
    string line;
    while (getline(file, line))
    {
        m.push_back(line);
    }
}

void FindStartEndAndWalls(vector<string>& m, Position& start, Position& end, set<Position>& walls)
{
    for (int i = 0; i < m.size(); i++)
    {
        for (int j = 0; j < m[i].size(); j++){
            if (m[i][j] == '#')
            {
                walls.insert(Position(i, j));
            }

            if (m[i][j] == 'S')
            {
                start = Position(i, j);
            }

            if (m[i][j] == 'E')
            {
                end = Position(i, j);
            }
        }
    }
}

/** Returns all valid positions with a Manhattan distance of up to @distance */ 
vector<Position> GetNeighbors(Position pos, Position limits, set<Position>& walls, int distance = 1)
{
    vector<Position> neighbors;
    for (int i = -distance; i <= distance; i++)
    {
        set<Position> proposedSteps;
        for (int j = -(distance - abs(i)); j <= distance - abs(i); j++)
        {
            proposedSteps.insert(Position(i, j));
        }
        for (auto step : proposedSteps)
        {
            auto proposedPosition = pos + step;
            if (walls.find(proposedPosition) == walls.end())
            {
                // Not wall or cheating, so allow traversal
                if (proposedPosition.x <= limits.x && proposedPosition.y <= limits.y
                    && proposedPosition.x >= 0 && proposedPosition.y >= 0)
                {
                    // Valid position inside grid
                    neighbors.push_back(proposedPosition);
                }
            }
        }
    }

    return neighbors;
}

// DFS returning path to end.
vector<Position> DFS(set<Position>& walls, Position start, Position end, Position limits)
{
    vector<int> distancesToEnd;
    vector<Position> winningPath;
    stack<tuple<Position, vector<Position>>> s;
    s.emplace(start, vector<Position>());
    while (!s.empty())
    {
        auto top = s.top();
        auto currentPosition = get<0>(top);
        auto currentVisited = get<1>(top);
        s.pop();

        if (find(currentVisited.begin(), currentVisited.end(), currentPosition) != currentVisited.end())
        {
            // Already visited
            continue;
        }

        currentVisited.push_back(currentPosition);
        if (currentPosition == end)
        {
            winningPath = currentVisited;
            distancesToEnd.push_back(currentVisited.size());
        }
        
        
        auto neighbors = GetNeighbors(currentPosition, limits, walls);
        for (auto neighbor : neighbors)
        {
            if (find(currentVisited.begin(), currentVisited.end(), neighbor) == currentVisited.end())
            {
                s.emplace(neighbor, currentVisited);
            }
        }
    }
    return winningPath;
}

class Compare
{
    public:
    // Using > because it's a min heap
    bool operator()(const tuple<Position, int>& a, const tuple<Position, int>& b) const
    {
        return get<1>(a) > get<1>(b);
    }
};

// Dijkstra returning map of distances to each position
map<Position, int> Dijkstra(set<Position>& walls, Position start, Position limits)
{
    map<Position, int> distances;
    for (int i = 0; i <= limits.x; i++)
    {
        for (int j = 0; j <= limits.y; j++)
        {
            distances[Position(i, j)] = INT_MAX;
        }
    }

    priority_queue<tuple<Position, int>, vector<tuple<Position, int>>, Compare> heap;
    heap.emplace(start, 0);
    distances[start] = 0;

    while (!heap.empty())
    {
        auto top = heap.top();
        auto currentPosition = get<0>(top);
        auto currentDistance = get<1>(top);
        heap.pop();

        if (currentDistance > distances[currentPosition])
        {
            continue;
        }
        auto neighbors = GetNeighbors(currentPosition, limits, walls);
        for (auto neighbor : neighbors)
        {
            auto newDistance = currentDistance + 1;
            if (newDistance < distances[neighbor])
            {
                distances[neighbor] = newDistance;
                heap.emplace(neighbor, newDistance);
            }
        }
    }

    return distances;
}

/** Receives a cheat start and end and returns the new distance after doing the cheat */
int DistanceCheating(Position cheatStart, Position cheatEnd, map<Position, int>& distancesFromStart, map<Position, int>& distancesFromEnd)
{
    const auto distanceVector = cheatStart - cheatEnd;
    const auto distance = abs(distanceVector.x) + abs(distanceVector.y);
    return distancesFromStart[cheatStart] + distancesFromEnd[cheatEnd] + distance;
}

int main(int argc, char* argv[])
{
    vector<string> m;
    ReadFile(m);
    
    Position start, end;
    set<Position> walls;
    FindStartEndAndWalls(m, start, end, walls);
    Position limits(m.size() - 1, m[0].length() - 1);

    auto distancesFromStart = Dijkstra(walls, start, limits);
    auto distanceWithoutCheating = distancesFromStart[end];
    auto distancesFromEnd = Dijkstra(walls, end, limits);
    auto path = DFS(walls, start, end, limits);
    
    long result1 = 0, result2 = 0;
    auto distances = {2, 20};
    for (auto distance : distances)
    {
        // For each position in path, get all possible destinations in Manhattan distance <= distance
        // then check if the new path improves by at least 100.

        // map<int, int> cheatingBenefits; -> Was used to print the output in the example
        map<vector<Position>, int> cheatToDistance;
        for (int i = 0; i < path.size(); i++)
        {
            auto startPos = path[i];
            auto neighbors = GetNeighbors(startPos, limits, walls, distance);
            for (auto endPos : neighbors)
            {
                const int distanceCheating = DistanceCheating(startPos, endPos, distancesFromStart, distancesFromEnd);
                auto benefit = distanceWithoutCheating - distanceCheating;
                if (benefit >= 100)
                {
                    if (distance == 2)
                    {
                        result1++;
                    }
                    else
                    {
                        result2++;
                    }
                }
                
                // if (benefit > 0)
                // {
                //     cheatingBenefits[benefit]++;
                // }
            }
        }
         // if (distance == 2)
         // {
         //     for (auto kvp : cheatingBenefits)
         //     {
         //             cout << "There are " << kvp.second << " cheats that save " << kvp.first << " picoseconds" << endl;
         //     }
         // }
         // else
         // {
         //     for (auto kvp : cheatingBenefits)
         //     {
         //         if (kvp.first >= 50)
         //         cout << "There are " << kvp.second << " cheats that save " << kvp.first << " picoseconds" << endl;
         //     }
         // }
    }
    
    // Part 1 - Answer: 1389
    cout << "Part 1: " << result1 << endl;
    // Part 2 - Answer: 1005068
    cout << "Part 2: " << result2 << endl;
    
    return 0;
}
