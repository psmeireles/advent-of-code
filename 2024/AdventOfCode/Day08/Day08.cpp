
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

class Vector
{
public:
    int x;
    int y;
    Vector() = default;
    Vector(int _x, int _y) : x(_x), y(_y) {}

    // Necessary to use set
    bool operator<(const Vector& rhs) const 
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

Vector operator+(Vector a, Vector b)
{
    return Vector(a.x + b.x, a.y + b.y);
}

Vector operator-(Vector a, Vector b)
{
    return Vector(a.x - b.x, a.y - b.y);
}

ostream& operator<<(ostream& os, const Vector& v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

bool IsInside(Vector a, Vector limit)
{
    return a.x <= limit.x && a.y <= limit.y && a.x >= 0 && a.y >= 0;
}

void ReadFile(map<char, vector<Vector>>& antennas, Vector& limitPosition)
{
    ifstream file("input.txt");
    string line;
    int i = 0;
    while (getline(file, line))
    {
        for (int j = 0; j < line.size(); j++)
        {
            char c = line[j];
            if (isalpha(c) || isdigit(c))
            {
                antennas[c].push_back(Vector(i, j));
            }
            limitPosition.y = j;
        }
        limitPosition.x = i;
        i++;
    }
}

int main(int argc, char* argv[])
{
    map<char, vector<Vector>> antennas;
    Vector limitPosition;
    ReadFile(antennas, limitPosition);
    for (auto it = antennas.begin(); it != antennas.end(); it++)
    {
        cout << it->first << ": ";
        for (auto v : it->second)
        {
            cout << v << ", ";
        }
        cout << endl;
    }

    
    set<Vector> antinodes1, antinodes2;
    int result1 = 0, result2 = 0;
    for (auto kvp : antennas)
    {
        auto positions = kvp.second;
        for (int i = 0; i < positions.size(); i++)
        {
            for (int j = i + 1; j < positions.size(); j++)
            {
                auto p1 = positions[i];
                auto p2 = positions[j];
                auto distance = p2 - p1;

                // Part 1
                {
                    auto possibility1 = p1 - distance;
                    if (antinodes1.find(possibility1) == antinodes1.end() && IsInside(possibility1, limitPosition))
                    {
                        result1++;
                        antinodes1.insert(possibility1);
                    }

                    auto possibility2 = p2 + distance;
                    if (antinodes1.find(possibility2) == antinodes1.end() && IsInside(possibility2, limitPosition))
                    {
                        result1++;
                        antinodes1.insert(possibility2);
                    }
                }

                // Part 2
                {
                    // Going in -distance direction
                    // If there are at least 3 antennas, every antenna will be in line with other 2, so also consider antenna position.
                    Vector possibility1 = positions.size() > 2 ? p1 : p1 - distance;
                    while (IsInside(possibility1, limitPosition))
                    {
                        if (antinodes2.find(possibility1) == antinodes2.end())
                        {
                            result2++;
                            antinodes2.insert(possibility1);
                        }
                        possibility1 = possibility1 - distance;
                    }

                    // Going in distance direction
                    // If there are at least 3 antennas, every antenna will be in line with other 2, so also consider antenna position.
                    Vector possibility2 = positions.size() > 2 ? p2 : p2 + distance;
                    while (IsInside(possibility2, limitPosition))
                    {
                        if (antinodes2.find(possibility2) == antinodes2.end())
                        {
                            result2++;
                            antinodes2.insert(possibility2);
                        }
                        possibility2 = possibility2 + distance;
                    }
                }
            }
        }
    }

    // Part1
    cout << result1 << endl;

    // Part2
    cout << result2 << endl;
    return 0;
}
