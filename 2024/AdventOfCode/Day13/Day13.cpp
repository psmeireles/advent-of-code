#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Problem
{
public:
    long long A[2];
    long long B[2];
    long long Target[2];
};

void ReadFile(vector<Problem>& problems)
{
    ifstream file("input.txt");
    string line1, line2, line3;
    bool firstLine = true;
    while (getline(file, line1) && getline(file, line2) && getline(file, line3))
    {
        if (firstLine)
        {
            // Remove BOM
            line1.erase(0, 3);
            firstLine = false;
        }
        Problem p;
        sscanf_s(line1.c_str(), "Button A: X+%lld, Y+%lld", &p.A[0], &p.A[1]);
        sscanf_s(line2.c_str(), "Button B: X+%lld, Y+%lld", &p.B[0], &p.B[1]);
        sscanf_s(line3.c_str(), "Prize: X=%lld, Y=%lld", &p.Target[0], &p.Target[1]);
        problems.push_back(p);

        // Consume empty line
        string _;
        getline(file, _);
    }
}

vector<long long> FindPresses(Problem p)
{
    // It's a system
    // Na*Ax + Nb*Bx = Tx
    // Na*Ay + Nb*By = Ty
    // Na = (Tx - Nb*Bx) / Ax
    // Nb = (Ty*Ax - Tx*Ay) / (By*Ax - Bx*Ay)

    const auto a = p.A;
    const auto b = p.B;
    const auto target = p.Target;

    const long double nB = (target[1] * a[0] - target[0] * a[1]) / static_cast<long double>(b[1] * a[0] - b[0] * a[1]);
    long double floorB = floorl(nB);
    long double ceilB = ceill(nB);
    const bool nBIsInteger = floorB == ceilB;
    if (nBIsInteger == false || nB < 0)
    {
        return {};
    }
    
    long long nBLong = nB;
    long double nA = (target[0] - nBLong * b[0]) / static_cast<long double>(a[0]);
    long double floorA = floorl(nA);
    long double ceilA = ceill(nA);
    const bool nAIsInteger = floorA == ceilA;
    if (nAIsInteger == false || nA < 0)
    {
        return {};
    }
    
    long long nALong = nA;
    return { nALong, nBLong };
}

int main(int argc, char* argv[])
{
    vector<Problem> problems;
    ReadFile(problems);

    long long result1 = 0, result2 = 0;
    for (auto p : problems)
    {
        // Part 1
        {
            auto numbers = FindPresses(p);
            if (!numbers.empty())
            {
                result1 += 3 * numbers[0] + numbers[1];
            }
        }

        // Part 2
        {
            p.Target[0] += 10000000000000;
            p.Target[1] += 10000000000000;
            auto numbers = FindPresses(p);
            if (!numbers.empty())
            {
                result2 += 3 * numbers[0] + numbers[1];
            }
        }
    }
    
    // Part 1
    cout << "Result 1: " << result1 << endl;

    // Part 2
    cout << "Result 2: " << result2 << endl;
    
    return 0;
}