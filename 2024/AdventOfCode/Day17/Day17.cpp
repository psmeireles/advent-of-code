#include <bitset>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// Registers
int a, b, c;
int instruction = 0;

void ReadFile(int& a, int& b, int& c, vector<int>& program)
{
    ifstream file("input.txt");
    string line1, line2, line3, _, operationsLine;
    getline(file, line1);
    getline(file, line2);
    getline(file, line3);
    sscanf_s(line1.c_str(), "Register A: %d", &a);
    sscanf_s(line2.c_str(), "Register B: %d", &b);
    sscanf_s(line3.c_str(), "Register C: %d", &c);
    getline(file, _);
    getline(file, operationsLine);
    {
        // Skip "Program: "
        string operationsS = operationsLine.substr(9);
        vector<int> ops, operands;
        size_t last = 0;
        size_t next = 0;
        while ((next = operationsS.find(',', last)) != string::npos)
        {
            auto number = operationsS.substr(last, next - last);
            program.push_back(atoi(number.c_str()));
            last = next + 1;
        }
        // Haven't found a comma, add last operand.
        program.push_back(atoi(operationsS.substr(last).c_str()));
    }
}

int combo(int x)
{
    switch (x)
    {
    case 0:
    case 1:
    case 2:
    case 3:
        return x;
    case 4:
        return a;
    case 5:
        return b;
    case 6:
        return c;
    default:
        return -1;
    }
}

// opcode 0
void adv(int x)
{
    int numerator = a;
    int denominator = pow(2, combo(x));
    a = numerator / denominator;
}

// opcode 1
void bxl(int x)
{
    b ^= x;
}

// opcode 2
void bst(int x)
{
    b = combo(x) % 8;
}

// opcode 3
bool jnz(int x)
{
    if (a == 0)
    {
        return false;
    }

    instruction = x;
    return true;
}

// opcode 4
void bxc()
{
    b ^= c;
}

// operand 5
int out(int x)
{
    return combo(x) % 8;
}

// operand 6
void bdv(int x)
{
    int numerator = a;
    int denominator = pow(2, combo(x));
    b = numerator / denominator;
}

// operand 7
void cdv(int x)
{
    int numerator = a;
    int denominator = pow(2, combo(x));
    c = numerator / denominator;
}

void Run(vector<int>& program, vector<int>& outs, bool earlyOut = false)
{
    while (instruction < program.size() - 1)
    {
        int op = program[instruction];
        int operand = program[instruction + 1];

        bool hasJumped = false;
        switch (op)
        {
        case 0:
            adv(operand);
            break;
        case 1:
            bxl(operand);
            break;
        case 2:
            bst(operand);
            break;
        case 3:
            hasJumped = jnz(operand);
            break;
        case 4:
            bxc();
            break;
        case 5:
            {
                int newValue = out(operand);
                if (earlyOut)
                {
                    if (newValue != program[outs.size()])
                    {
                        // Found a difference, stop executing.
                        return;
                    }
                }
                outs.push_back(newValue);
            }
            break;
        case 6:
            bdv(operand);
            break;
        case 7:
            cdv(operand);
        }

        if (!hasJumped)
        {
            instruction += 2;
        }
    }
}

void Reset()
{
    a = 0;
    b = 0;
    c = 0;
    instruction = 0;
}

bool Equals(vector<int>& a, vector<int>& b)
{
    if (a.size() != b.size())
    {
        return false;
    }
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }
    return true;
}

int Solve(int numA, int result)
{
    int solution = 0;
    // working bit by bit
    for (int i = 0; i < 3; i++)
    {
        int mask = 1 << i;
        int bitA = numA & mask;
        int bitResult = result & mask;
        if (bitA == bitResult)
        {
            // This bit from solution should be zero. Do nothing.
        }
        else
        {
            if (bitResult == 0)
            {
                // We must have the same bit as A
                solution |= bitA;
            }
            else
            {
                // Result is 1, so we must be different from A
                solution |= ~bitA & mask;
            }
        }
    }

    cout << bitset<10>(numA).to_string().substr(7) << " ^ " << bitset<10>(solution).to_string().substr(7) << " = " << bitset<10>(result).to_string().substr(7) << endl;
    
    return solution;
}

int main(int argc, char* argv[])
{
    vector<int> program;
    ReadFile(a, b, c, program);

    cout << a << " " << b << " " << c << endl;

    vector<int> outs;
    Run(program, outs);

    // Part 1 - Answer: 2,0,1,3,4,0,2,1,7
    cout << "Part 1: ";
    for (int i = 0; i < outs.size() - 1; i++)
    {
        cout << outs[i] << ",";
    }
    cout << outs[outs.size() - 1] << endl;

    // Part 2
    reverse(program.begin(), program.end());
    long long result = 0;
    int previous = 0;
    for (int i = 0; i < program.size(); i++)
    {
        int step = Solve(previous, program[i]);
        // cout << step << ", ";
        previous = step;
        result |= step;
        result <<= 3;
    }
    cout << endl;

    cout << "Part 2: " << result << endl;

    Reset();
    reverse(program.begin(), program.end());
    a = result;
    vector<int> outs2;
    Run(program, outs2);
    for (int i = 0; i < outs2.size() - 1; i++)
    {
        cout << outs2[i] << ",";
    }
    cout << outs2[outs2.size() - 1] << endl;

    return 0;
}
