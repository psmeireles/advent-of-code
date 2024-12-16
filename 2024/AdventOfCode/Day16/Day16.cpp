
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
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

map<Position, vector<Position>> BuildGraph(vector<string>& m, Position& start, Position& end)
{
    map<Position, vector<Position>> graph;
    for (int i = 1; i < m.size() - 1; i++)
    {
        for (int j = 1; j < m[i].size() - 1; j++){
            if (m[i][j] == '#')
            {
                continue;
            }

            if (m[i][j] == 'S')
            {
                start = Position(i, j);
            }

            if (m[i][j] == 'E')
            {
                end = Position(i, j);
            }

            auto currentPosition = Position(i, j);

            vector<Position> neighbors{
                Position(i, j-1),
                Position(i, j+1),
                Position(i-1, j),
                Position(i+1, j),
            };

            for (auto neighbor : neighbors)
            {
                if (m[neighbor.x][neighbor.y] != '#')
                {
                    graph[currentPosition].push_back(neighbor);
                }
            }
        }
    }
    return graph;
}

class Compare
{
public:
    // Using > so it's a min heap
    bool operator()(tuple<Position, Position, int> a, tuple<Position, Position, int> b)
    {
        return get<2>(a) > get<2>(b);
    }
    bool operator()(tuple<Position, Position, int, set<Position>> a, tuple<Position, Position, int, set<Position>> b)
    {
        return get<2>(a) > get<2>(b);
    }
};

int Dijkstra(map<Position, vector<Position>>& graph, Position start, Position direction, Position end)
{
    // Distances map also considers direction we entered
    map<Position, map<Position, int>> distances;
    for (const auto& kvp : graph)
    {
        distances[kvp.first] = {
            {Position(0, 1), INT_MAX},
            {Position(0, -1), INT_MAX},
            {Position(1, 0), INT_MAX},
            {Position(-1, 0), INT_MAX},
        };
    }
    
    priority_queue<tuple<Position, Position, int>, vector<tuple<Position, Position, int>>, Compare> heap;
    heap.push(make_tuple(start, direction, 0));
    distances[start][direction] = 0;
    while (!heap.empty())
    {
        auto top = heap.top();
        auto currentPosition = get<0>(top), currentDirection = get<1>(top);
        auto currentDistance = get<2>(top);
        heap.pop();

        if (currentDistance > distances[currentPosition][currentDirection])
        {
            continue;
        }

        for (auto neighbor : graph[currentPosition])
        {
            int newDistance;
            if (neighbor == currentPosition + currentDirection)
            {
                // Direct in front
                newDistance = currentDistance + 1;
            }
            else if (neighbor == currentPosition - currentDirection)
            {
                // Direct behind. 2 turns
                newDistance = currentDistance + 2001;
            }
            else
            {
                // Left or Right. One turn
                newDistance = currentDistance + 1001;
            }

            auto newDirection = neighbor - currentPosition;
            if (newDistance < distances[neighbor][newDirection])
            {
                distances[neighbor][newDirection] = newDistance;
                heap.push(make_tuple(neighbor, newDirection, newDistance));
            }
        }
    }

    int minDistance = INT_MAX;
    for (auto kvp : distances[end])
    {
        minDistance = min(minDistance, kvp.second);
    }
    return minDistance;
}

set<Position> Dijkstra2(map<Position, vector<Position>>& graph, Position start, Position direction, Position end)
{
    // Distances map also considers direction we entered.
    // Storing for each position/direction the distance and all positions traversed to get there.
    map<Position, map<Position, tuple<int, set<Position>>>> distances;
    for (const auto& kvp : graph)
    {
        distances[kvp.first] = {
            {Position(0, 1), make_tuple(INT_MAX, set<Position>())},
            {Position(0, -1), make_tuple(INT_MAX, set<Position>())},
            {Position(1, 0), make_tuple(INT_MAX, set<Position>())},
            {Position(-1, 0), make_tuple(INT_MAX, set<Position>())},
        };
    }
    
    priority_queue<tuple<Position, Position, int, set<Position>>, vector<tuple<Position, Position, int, set<Position>>>, Compare> heap;
    heap.push(make_tuple(start, direction, 0, set<Position>()));
    distances[start][direction] = make_tuple(0, set<Position>());
    while (!heap.empty())
    {        
        auto top = heap.top();
        auto currentPosition = get<0>(top), currentDirection = get<1>(top);
        auto currentDistance = get<2>(top);
        auto currentVisited = get<3>(top);
        heap.pop();
    
        if (currentDistance > get<0>(distances[currentPosition][currentDirection]))
        {
            continue;
        }
        
        if (currentDistance == get<0>(distances[currentPosition][currentDirection]))
        {
            // Same size, but maybe different paths. Merge both sets.
            auto existingSet = get<1>(distances[currentPosition][currentDirection]);
            set<Position> newSet;
            for (auto pos : existingSet)
            {
                newSet.insert(pos);
            }
            for (auto pos : currentVisited)
            {
                newSet.insert(pos);
            }
            distances[currentPosition][currentDirection] = make_tuple(currentDistance, newSet);
        }
        
        for (auto neighbor : graph[currentPosition])
        {
            int newDistance;
            if (neighbor == currentPosition + currentDirection)
            {
                // Direct in front
                newDistance = currentDistance + 1;
            }
            else if (neighbor == currentPosition - currentDirection)
            {
                // Direct behind. 2 turns
                newDistance = currentDistance + 2001;
            }
            else
            {
                // Left or Right. One turn
                newDistance = currentDistance + 1001;
            }
        
            auto newDirection = neighbor - currentPosition;
            if (newDistance < get<0>(distances[neighbor][newDirection]))
            {
                auto newSet = currentVisited;
                newSet.insert(currentPosition);
                distances[neighbor][newDirection] = make_tuple(newDistance, newSet);
                heap.push(make_tuple(neighbor, newDirection, newDistance, newSet));
            }
            else if (newDistance == get<0>(distances[neighbor][newDirection]))
            {
                // Same size, but maybe different paths. Merge both sets.
                auto existingSet = get<1>(distances[neighbor][newDirection]);
                set<Position> newSet;
                for (auto pos : existingSet)
                {
                    newSet.insert(pos);
                }
                for (auto pos : currentVisited)
                {
                    newSet.insert(pos);
                }
                newSet.insert(currentPosition);
                distances[neighbor][newDirection] = make_tuple(newDistance, newSet);
                heap.push(make_tuple(neighbor, newDirection, newDistance, newSet));
            }
        }
    }

    int minDistance = INT_MAX;
    for (auto kvp : distances[end])
    {
        minDistance = min(minDistance, get<0>(kvp.second));
    } 
    
    set<Position> visited;
    for (auto kvp : distances[end])
    {
        if (get<0>(kvp.second) == minDistance)
        {
            visited.insert(get<1>(kvp.second).begin(), get<1>(kvp.second).end());
        }
    }
    
    // We have to add end because it wasn't considered part of the path.
    visited.insert(end);
    
    return visited;
}

int main(int argc, char* argv[])
{
    vector<string> m;
    ReadFile(m);

    Position start, end;
    map<Position, vector<Position>> graph = BuildGraph(m, start, end);

    // Part 1 - Answer: 133584
    cout << "Part 1: " << Dijkstra(graph, start, Position(0, 1), end) << endl;

    // Part 2 - Answer: 622
    auto positions = Dijkstra2(graph, start, Position(0, 1), end);
    cout << "Part 2: " << positions.size() << endl;


    // Printing paths found
    // for (int i = 0; i < m.size(); i++)
    // {
    //     for (int j = 0; j < m[i].size(); j++)
    //     {
    //         if (positions.find(Position(i, j)) != positions.end())
    //         {
    //             cout << "O";
    //         }
    //         else
    //         {
    //             cout << m[i][j];
    //         }
    //     }
    //     cout << endl;
    // }
    
    return 0;
}