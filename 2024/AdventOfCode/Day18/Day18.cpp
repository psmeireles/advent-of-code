
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

void ReadFile(vector<Position>& corruption)
{
    ifstream inputFile("input.txt");
    string line;
    while (getline(inputFile, line))
    {
        Position p;
        sscanf_s(line.c_str(), "%d,%d", &p.y, &p.x);
        corruption.push_back(p);
    }
}

vector<Position> GetNeighbors(Position pos, Position end, set<Position>& corruptedSet)
{
    vector<Position> neighbors;
    vector<Position> neighborDirections{
        Position(0, -1),
        Position(0, 1),
        Position(-1, 0),
        Position(1, 0)
    };

    for (auto direction : neighborDirections)
    {
        auto proposedPosition = pos + direction;
        if (corruptedSet.find(proposedPosition) == corruptedSet.end())
        {
            // Not corrupted
            if (proposedPosition.x <= end.x && proposedPosition.y <= end.y
                && proposedPosition.x >= 0 && proposedPosition.y >= 0)
            {
                // Valid position inside grid
                neighbors.push_back(proposedPosition);
            }
        }
    }

    return neighbors;
}

class Compare
{
public:
    // Using > so it's a min heap
    bool operator()(tuple<Position, int> a, tuple<Position, int> b)
    {
        return get<1>(a) > get<1>(b);
    }
};

int Dijkstra(vector<Position>& corruption, Position end, int time)
{
    Position start(0, 0);
    map<Position, int> distances;
    for (int i = 0; i <= end.x; i++)
    {
        for (int j = 0; j <= end.y; j++)
        {
            distances.insert(make_pair(Position(i, j), INT_MAX));
        }
    }
    set<Position> corruptedSet(corruption.begin(), corruption.begin() + time);

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

        auto neighbors = GetNeighbors(currentPosition, end, corruptedSet);
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

    return distances[end];
}

int main(int argc, char* argv[])
{
    vector<Position> corruption;
    ReadFile(corruption);

    // Part 1 - Answer: 234
    auto result1 = Dijkstra(corruption, Position(70, 70), 1024);
    cout << "Result1: " << result1 << endl;

    // Part 2 - Answer: 58,19
    for (auto i = 0; i < corruption.size(); i++)
    {
        auto distance = Dijkstra(corruption, Position(70, 70), i + 1);
        if (distance == INT_MAX)
        {
            // Printing inverted, as it wants in form of (x,y), but we store as [row][column]
            cout << "Result2: " << Position(corruption[i].x, corruption[i].y) << endl;
            return 0;
        }
    }
    
    return 0;
}
