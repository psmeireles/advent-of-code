#include <bitset>
#include <string>
#include <queue>
#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>

using namespace std;

// Registers
unsigned long long a, b, c;
int instruction = 0;

void ReadFile(unsigned long long& a, unsigned long long& b, unsigned long long& c, vector<int>& program)
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

unsigned long long combo(int x)
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
    unsigned long long numerator = a;
    unsigned long long denominator = pow(2, combo(x));
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
    unsigned long long numerator = a;
    unsigned long long denominator = pow(2, combo(x));
    b = numerator / denominator;
}

// operand 7
void cdv(int x)
{
    unsigned long long numerator = a;
    unsigned long long denominator = pow(2, combo(x));
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

int main(int argc, char* argv[])
{
    vector<int> program;
    ReadFile(a, b, c, program);
    
    vector<int> outs;
    Run(program, outs);

    // Part 1 - Answer: 2,0,1,3,4,0,2,1,7
    cout << "Part 1: ";
    for (int i = 0; i < outs.size() - 1; i++)
    {
        cout << outs[i] << ",";
    }
    cout << outs[outs.size() - 1] << endl;



    // Part 2 - Answer: 236580836040301
    // Program: 2,4,1,3,7,5,0,3,4,3,1,5,5,5,3,0
    //
    // (2,4), (1,3), (7,5), (0,3), (4,3), (1,5), (5,5), (3,0)
    // bst(4), bxl(3), cdv(5), adv(3), bxc(3), bxl(5), out(5), jnz(0)
    //
    // bst(4) -> b = a % 8
    // bxl(3) -> b ^= 011
    // cdv(5) -> c = a / (2 ** b)
    // adv(3) -> a = a / 8
    // bxc(3) -> b ^= c
    // bxl(5) -> b ^= 101
    // out(5) -> return b % 8
    // jnz(0) -> restart

    // Each output of the program only depends on the last 6 bits of a. The very last output necessarily only depends on the
    // last 3 bits of a, as the other 3 will be 0. That means that there are 8 possibilities to try if we fix the other 3 bits.
    // My approach here is start with 0, and try 0 + 0..7 and check if it returns the expected output. If it does, shift 3 bits
    // and try Shifted + 0..7 recursively until we find the solution. More than one of the combinations can work at one step,
    // but may fail in a later step. Adding all calculations to a queue until we eventually find our result.

    queue<tuple<unsigned long long, int>> q;
    q.emplace(0, 1);
    while (!q.empty())
    {
        auto temp = q.front();
        q.pop();
        unsigned long long number = get<0>(temp);

        // Build the expected result at this step
        int size = get<1>(temp);
        vector<int> expected(program.end() - size, program.end());

        // Try from number to number + 7 (all combinations of the last 3 bits)
        for (int i = 0; i < 8; i++)
        {
            Reset();
            a = number | i;
            vector<int> output;
            Run(program, output);

            if (Equals(output, program))
            {
                // Found the result
                cout << "Part 2: " << (number | i) << endl;
                return 0;
            }

            // Found an intermediate result. Shift 3 and try again with the next element of the output.
            if (Equals(output, expected))
            {
                auto newNumber = (number | i) << 3;
                q.emplace(newNumber, size + 1);
            }
        }
    }
    
    return 0;
}
