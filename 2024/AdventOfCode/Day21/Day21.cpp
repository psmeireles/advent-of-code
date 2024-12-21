
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <stack>
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

vector<string> numPad {
    "789",
    "456",
    "123",
    "#0A"
};

/** Position for each character in the num pad */
map<char, Position> numPadPositionMap = {
    {'0', Position(3, 1)},
    {'1', Position(2, 0)},
    {'2', Position(2, 1)},
    {'3', Position(2, 2)},
    {'4', Position(1, 0)},
    {'5', Position(1, 1)},
    {'6', Position(1, 2)},
    {'7', Position(0, 0)},
    {'8', Position(0, 1)},
    {'9', Position(0, 2)},
    {'A', Position(3, 2)},
};

vector<string> dirPad {
    "#^A",
    "<v>"
};

/** Position for each character in the direction pad */
map<char, Position> dirPadPositionMap = {
    {'A', Position(0, 2)},
    {'^', Position(0, 1)},
    {'<', Position(1, 0)},
    {'v', Position(1, 1)},
    {'>', Position(1, 2)},

};

void ReadFile(vector<string>& sequences)
{
    ifstream file("input.txt");
    string line;
    while (getline(file, line))
    {
        sequences.push_back(line);
    }
}

/** Returns all neighboring position in grid. '#' is an invalid position. */
vector<Position> GetNeighbors(Position pos, Position limits, vector<string>& grid)
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
        if (proposedPosition.x <= limits.x && proposedPosition.y <= limits.y
            && proposedPosition.x >= 0 && proposedPosition.y >= 0)
        {
            // Valid position inside grid
            if (grid[proposedPosition.x][proposedPosition.y] != '#')
            {
                // Not invalid
                neighbors.push_back(proposedPosition);
            }
        }
    }

    return neighbors;
}

/** DFS returning all shortest paths to end. */
vector<vector<Position>> DFS(vector<string>& pad, Position start, Position end, Position limits)
{    
    int distanceToEnd = INT_MAX;
    vector<vector<Position>> shortestPaths;
    stack<pair<Position, vector<Position>>> s;
    s.emplace(start, vector<Position>());
    while (!s.empty())
    {
        auto top = s.top();
        auto currentPosition = top.first;
        auto currentVisited = top.second;
        s.pop();

        if (find(currentVisited.begin(), currentVisited.end(), currentPosition) != currentVisited.end())
        {
            // Already visited
            continue;
        }

        currentVisited.push_back(currentPosition);
        if (currentPosition == end)
        {
            if (currentVisited.size() < distanceToEnd)
            {
                shortestPaths = {currentVisited};
                distanceToEnd = currentVisited.size();
            }
            else if (currentVisited.size() == distanceToEnd)
            {
                shortestPaths.push_back(currentVisited);
            }
        }
        
        auto neighbors = GetNeighbors(currentPosition, limits, pad);
        for (auto neighbor : neighbors)
        {
            if (find(currentVisited.begin(), currentVisited.end(), neighbor) == currentVisited.end())
            {
                s.emplace(neighbor, currentVisited);
            }
        }
    }
    
    return shortestPaths;
}

char StepToButton(Position start, Position end)
{
    map<Position, char> neighborDirections{
        make_pair(Position(0, -1), '<'),
        make_pair(Position(0, 1), '>'),
        make_pair(Position(-1, 0), '^'),
        make_pair(Position(1, 0), 'v')
    };
    for (auto direction : neighborDirections)
    {
        if (end == start + direction.first)
        {
            return direction.second;
        }
    }
}

/** Converts a path of positions to a string with the button presses needed to achieve the positions */
string PathToPresses(vector<Position>& path)
{
    string presses;
    for (auto i = 1; i < path.size(); i++)
    {
        auto last = path[i - 1];
        auto destination = path[i];
        presses += StepToButton(last, destination);
    }
    
    // Add final A to confirm input
    presses += 'A';
    return presses;
}

/** Returns the amount of buttons we need to push in order to take us from @from to @to in layer @layer and then activate. */
map<string, long long> memo;
long long Solve(char from, char to, int layer, map<char, map<char, vector<string>>>& pathsCache) {
    if (layer == 0)
    {
        // We can push any button in 1 step
        return 1;
    }

    // Cache key is "{from}-{to}-{layer}"
    const auto key = to_string(from) + "-" + to_string(to) + "-" + to_string(layer);
    if (memo.find(key) != memo.end())
    {
        // We already calculated that
        return memo[key];
    }

    long long shortest = LLONG_MAX;
    auto paths = pathsCache[from][to];
    // For each path in this layer, the cost will be the sum of the costs for each step in the layer below.
    // Calculate all costs and pick the smaller
    for (const auto& path : paths)
    {
        long long len = 0;
        char start = 'A';
        for (char next : path) {
            len += Solve(start, next, layer - 1, pathsCache);
            start = next;
        }
        shortest = min(len, shortest);
    }
    memo[key] = shortest;
    return shortest;
}

int main(int argc, char* argv[])
{
    vector<string> sequences;
    ReadFile(sequences);

    Position numPadLimits(numPad.size() - 1, numPad[0].length() - 1);
    Position dirPadLimits(dirPad.size() - 1, dirPad[0].length() - 1);

    /** pathsCache[from][to] stores the string of presses to go from @from to @to */
    map<char, map<char, vector<string>>> pathsCache;
    
    // Find all paths between all positions in num pad.
    for (auto kvpFrom : numPadPositionMap)
    {
        auto fromChar = kvpFrom.first;
        auto from = kvpFrom.second;
        for (auto kvpTo : numPadPositionMap)
        {
            auto toChar = kvpTo.first;
            auto to = kvpTo.second;
            auto paths = DFS(numPad, from, to, numPadLimits);
            
            vector<string> solutions;
            solutions.reserve(paths.size());
            for (auto p : paths)
            {
                solutions.push_back(PathToPresses(p));
            }
            pathsCache[fromChar][toChar] = solutions;
        }
    }

    // Find all paths between all positions in directions pad.
    for (auto kvpFrom : dirPadPositionMap)
    {
        auto fromChar = kvpFrom.first;
        auto from = kvpFrom.second;
        for (auto kvpTo : dirPadPositionMap)
        {
            auto toChar = kvpTo.first;
            auto to = kvpTo.second;
            auto paths = DFS(dirPad, from, to, dirPadLimits);
            
            vector<string> solutions;
            solutions.reserve(paths.size());
            for (auto p : paths)
            {
                solutions.push_back(PathToPresses(p));
            }
            pathsCache[fromChar][toChar] = solutions;
        }
    }

    for (auto sequence : sequences)
    {
        auto start = 'A';
        vector<string> answers{""};
        for (int i = 0; i < sequence.size(); i++)
        {
            auto end = sequence[i];
            auto paths = pathsCache[start][end][0];
            vector<string> newAnswers;
            for (auto answer : answers)
            {
                for (auto p : paths)
                {
                    newAnswers.push_back(answer + p);
                }
            }
            answers = newAnswers;
        }
    }

    long long result1 = 0;
    long long result2 = 0;
    for (const auto& sequence : sequences)
    {
        long long temp1 = 0, temp2 = 0;
        auto start = 'A';
        for (auto next : sequence)
        {
            temp1 += Solve(start, next, 3, pathsCache);
            temp2 += Solve(start, next, 26, pathsCache);
            
            start = next;
        }

        int numericalPart = stoi(sequence.substr(0, 3));
        // Calculating complexities
        result1 += temp1 * numericalPart;
        result2 += temp2 * numericalPart;
    }
    
    // Part 1 - Answer: 203734
    cout << "Part 1: " << result1 << endl;

    // Part 2 - Answer: 246810588779586
    cout << "Part 2: " << result2 << endl;

    return 0;
}
