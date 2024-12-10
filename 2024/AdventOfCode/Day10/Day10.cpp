
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>
using namespace std;

void ReadFile(vector<vector<int>> &map)
{
    ifstream file("input.txt");
    string line;
    int i = 0;
    while (getline(file, line))
    {
        map.push_back(vector<int>());
        for (const char c : line)
        {
            map[i].push_back(c - '0');
        }
        i++;
    }
}

ostream &operator<<(ostream &os, const tuple<int, int> &t)
{
    os << "(" << get<0>(t) << ", " << get<1>(t) << ")";
    return os;
}

int GetPosition(vector<vector<int>>& map, tuple<int, int>& pos)
{
    int i = get<0>(pos), j = get<1>(pos);
    return map[i][j];
}

void BuildGraph(vector<vector<int>>& map, std::map<tuple<int, int>, vector<tuple<int, int>>>& graph)
{
    auto getNeighbors = [&map](int i, int j)
    {
        auto isInside = [&map](tuple<int, int> t)
        {
            int i = get<0>(t) , j = get<1>(t);
            return i >= 0 && i < map.size() && j >= 0 && j < map[i].size();
        } ;

        vector<tuple<int, int>> neighbors;
        int value = map[i][j];
        if (value < 0)
        {
            return neighbors;
        }
        
        auto left = make_tuple(i, j - 1);
        if (isInside(left) && GetPosition(map, left) == value + 1)
        {
            neighbors.push_back(left);
        }

        auto right = make_tuple(i, j + 1);
        if (isInside(right) && GetPosition(map, right) == value + 1)
        {
            neighbors.push_back(right);
        }

        auto up = make_tuple(i - 1, j);
        if (isInside(up) && GetPosition(map, up) == value + 1)
        {
            neighbors.push_back(up);
        }

        auto down = make_tuple(i + 1, j);
        if (isInside(down) && GetPosition(map, down) == value + 1)
        {
            neighbors.push_back(down);
        }

        return neighbors;
    };
    
    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            auto t = make_tuple(i, j);
            auto neighbors = getNeighbors(i, j);
            graph[t].insert(graph[t].end(), neighbors.begin(), neighbors.end());
        }
    }
}

tuple<int, int> GetScoreAndRating(vector<vector<int>>& map, std::map<tuple<int, int>, vector<tuple<int, int>>>& graph, tuple<int, int>& pos)
{
    int score = 0, rating = 0;
    std::map<tuple<int, int>, bool> visited;
    queue<tuple<int, int>> q{};
    for (auto p : graph[pos])
    {
        q.push(p);
    }
    
    while (!q.empty())
    {
        auto curr = q.front();
        q.pop();

        if (GetPosition(map, curr) == 9)
        {
            // Reached a 9, increase rating
            rating++;
            if (visited[curr] != true)
            {
                // Reached a new 9. Increase score.
                score++;
            }
        }
        visited[curr] = true;
        
        for (const auto& neighbor : graph[curr])
        {
            q.push(neighbor);
        }
    }
    return make_tuple(score, rating);
}

int main(int argc, char* argv[])
{
    vector<vector<int>> map;
    ReadFile(map);
    
    std::map<tuple<int, int>, vector<tuple<int, int>>> graph;
    BuildGraph(map, graph);

    int result1 = 0, result2 = 0;
    for (const auto& kvp : graph)
    {
        auto origin = kvp.first;
        if (GetPosition(map, origin) == 0)
        {
            // Trailhead. Let's get the score.
            auto t = GetScoreAndRating(map, graph, origin);
            auto score = get<0>(t), rating = get<1>(t);
            result1 += score;
            result2 += rating;
            
            cout << kvp.first << ": Score: " << score << ", Rating: " << rating << endl;
        }
    }

    // Part 1
    cout << result1 << endl;

    // Part 2
    cout << result2 << endl;
    
    return 0;
}
