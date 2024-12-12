#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
using namespace std;

class Position;
vector<Position> GetNeighbors(const vector<string>& garden, int i, int j);
vector<vector<Position>> GetCorners(vector<string>& garden, int i, int j);

class Position
{
public:
    int X;
    int Y;
    char Type;

    Position() = default;
    
    Position(int x, int y, char type) : X(x), Y(y), Type(type) {}
    
    Position(vector<string>& garden, int x, int y)
    {
        if (x < 0 || x >= garden.size() || y < 0 || y >= garden[x].size())
        {
            *this = Invalid;
            return;
        }
        X = x;
        Y = y;
        Type = garden[x][y];
    }
    

    int GetNumberOfCorners(vector<string>& garden)
    {
        int cornerCount = 0;
        auto corners = GetCorners(garden, X, Y);
        for (auto corner : corners)
        {
            // Concave corner.
            // X  ?
            // A. X
            if (corner[0].Type != Type && corner[2].Type != Type)
            {
                cornerCount++;
            }

            // Convex corner.
            // A  X
            // A. A
            if (corner[0].Type == Type && corner[2].Type == Type && corner[1].Type != Type)
            {
                cornerCount++;
            }

        }
        return cornerCount;
    }

    // Invalid position outside the garden
    static Position Invalid;
};
Position Position::Invalid = { -1, -1, ' ' };

class Region
{
public:
    Position StartPosition;
    vector<Position> Positions;
    char Type;

    int GetArea() const
    {
        return Positions.size();
    }

    int GetPerimeter(vector<string>& garden) const
    {
        int perimeter = 0;
        for (auto position : Positions)
        {
            for (auto neighbor : GetNeighbors(garden, position.X, position.Y))
            {
                if (neighbor.Type != position.Type)
                {
                    perimeter++;
                }
            }
        }
        return perimeter;
    }

    vector<Position> GetPerimeterPositions(vector<string>& garden) const
    {
        vector<Position> positions;
        for (auto position : Positions)
        {
            for (auto neighbor : GetNeighbors(garden, position.X, position.Y))
            {
                if (neighbor.Type != position.Type)
                {
                    bool shouldAdd = true;
                    // Avoid duplicates. Could have used a set, but I was lazy to implement the comparer.
                    for (const auto pos : positions)
                    {
                        if (pos.X == position.X && pos.Y == position.Y)
                        {
                            shouldAdd = false;
                            break;
                        }
                    }
                    if (shouldAdd)
                    {
                        positions.push_back(position);
                    }
                }
            }
        }
        return positions;
    }

    int GetNumberOfSides(vector<string>& garden) const
    {
        int sides = 0;
        for (auto position : Positions)
        {
            sides += position.GetNumberOfCorners(garden);
        }
        return sides;
    }
};


void ReadFile(vector<string>& garden)
{
    ifstream file("input.txt");
    string line;
    bool firstLine = true;
    while (getline(file, line))
    {
        if (firstLine)
        {
            // Skip BOM
            line.erase(0, 3);
            firstLine = false;
        }
        garden.push_back(line);
    }
}

vector<vector<Position>> GetCorners(vector<string>& garden, int i, int j)
{
    vector<vector<Position>> corners;

    const int leftX = i, leftY = j - 1;
    const int bottomX = i + 1, bottomY = j;
    const int topX = i - 1, topY = j;
    const int rightX = i, rightY = j + 1;

    // Position constructor will check if position is valid. If it's not, returns Position::Invalid 
    
    // Top Right
    vector<Position> topRight{
        Position(garden, topX, topY),
        Position(garden, topX, rightY),
        Position(garden, rightX, rightY),
    };

    // Bottom Right
    vector<Position> bottomRight{
        Position(garden, bottomX, bottomY),
        Position(garden, bottomX, rightY),
        Position(garden, rightX, rightY),
    };

    // Bottom Left
    vector<Position> bottomLeft{
        Position(garden, bottomX, bottomY),
        Position(garden, bottomX, leftY),
        Position(garden, leftX, leftY),
    };

    // Top Left
    vector<Position> topLeft{
        Position(garden, topX, topY),
        Position(garden, topX, leftY),
        Position(garden, leftX, leftY),
    };

    corners.push_back(topRight);
    corners.push_back(bottomRight);
    corners.push_back(bottomLeft);
    corners.push_back(topLeft);
    return corners;
}

vector<Position> GetNeighbors(const vector<string>& garden, int i, int j)
{
    if (i < 0 || i >= garden.size() || j < 0 || j >= garden[i].size())
    {
        return {};
    }

    vector<Position> neighbors;
    
    const int leftX = i - 1, leftY = j;
    if (leftX >= 0 && leftX < garden.size())
    {
        neighbors.push_back({ leftX, leftY, garden[leftX][leftY] });
    }
    else
    {
        neighbors.push_back(Position::Invalid);
    }
    
    const int rightX = i + 1, rightY = j;
    if (rightX >= 0 && rightX < garden.size())
    {
        neighbors.push_back({ rightX, rightY, garden[rightX][rightY] });
    }
    else
    {
        neighbors.push_back(Position::Invalid);
    }
    
    const int topX = i, topY = j - 1;
    if (topY >= 0 && topY < garden[i].size())
    {
        neighbors.push_back({ topX, topY, garden[topX][topY] });
    }
    else
    {
        neighbors.push_back(Position::Invalid);
    }
    
    const int bottomX = i, bottomY = j + 1;
    if (bottomY >= 0 && bottomY < garden[i].size())
    {
        neighbors.push_back({ bottomX, bottomY, garden[bottomX][bottomY] });
    }
    else
    {
        neighbors.push_back(Position::Invalid);
    }
    return neighbors;
}

Region FindRegion(const vector<string>& garden, vector<vector<bool>>& visited, int i, int j)
{
    Region region;
    region.StartPosition = { i, j, garden[i][j] };
    region.Type = garden[i][j];
    
    queue<Position> q;
    q.push({i, j, garden[i][j]});
    while (q.empty() == false)
    {
        const auto curr = q.front();
        q.pop();
        
        if (visited[curr.X][curr.Y])
        {
            continue;
        }
        region.Positions.push_back(curr);
        visited[curr.X][curr.Y] = true;
        
        auto neighbors = GetNeighbors(garden, curr.X, curr.Y);
        for (auto& neighbor : neighbors)
        {
            if (neighbor.Type == curr.Type)
            {
                q.push(neighbor);
            }
        }
    }
    return region;
}

int main(int argc, char* argv[])
{
    vector<string> garden;
    ReadFile(garden);
    
    vector<vector<bool>> visited(garden.size(), vector<bool>(garden[0].size(), false));
    vector<Region> regions;
    for (int i = 0; i < garden.size(); i++)
    {
        for (int j = 0; j < garden[i].size(); j++)
        {
            if (visited[i][j])
            {
                continue;
            }

            regions.push_back(FindRegion(garden, visited, i, j));
        }
    }

    int result1 = 0, result2 = 0;
    for (auto region : regions)
    {
        result1 += region.GetArea() * region.GetPerimeter(garden);
        result2 += region.GetArea() * region.GetNumberOfSides(garden);
        cout << "Found Region " << region.Type << " in (" << region.StartPosition.X << ", " << region.StartPosition.Y << "). Area: " << region.GetArea() << ", Perimeter: " << region.GetPerimeter(garden) << ", Sides: " << region.GetNumberOfSides(garden) << endl; 
    }

    // Part1
    cout << endl << result1 << endl;

    // Part2
    cout << endl << result2 << endl;
    
    return 0;
}
