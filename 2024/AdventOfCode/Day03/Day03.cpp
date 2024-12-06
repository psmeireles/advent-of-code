
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void ReadFile(string& output)
{
    ifstream file("input.txt");
    getline(file, output);
}

bool ExtractMul(const string& input, const int i, int& outValue)
{
    int startIndex = i + 4;
    string start = input.substr(i, 4);
    if (start != "mul(")
    {
        // Not "mul("
        return false;
    }

    auto commaPos = input.substr(startIndex, 4).find(',');
    if (commaPos > 3 || commaPos == string::npos)
    {
        //No ',' after a possible 3 digit number
        return false;
    }

    auto closeBracketPos = input.substr(startIndex + commaPos + 1, 4).find(')');
    if (closeBracketPos > 3 || closeBracketPos == string::npos)
    {
        //No ')' after a possible 3 digit number
        return false;
    }

    int x, y;
    try
    {
        x = stoi(input.substr(startIndex, commaPos));
        y = stoi(input.substr(startIndex + commaPos + 1, closeBracketPos));
    }
    catch (...)
    {
        // Can't cast string to number. Something like "mul(abc,def)"
        return false;
    }

    cout << "Found multiplication: " << x << ", " << y << '\n';
    outValue = x * y;
    return true;
}

bool ExtractDoDont(const string& input, const int i, bool& outValue)
{
    // Test don't()
    if (input.substr(i, 7) == "don't()")
    {
        cout << "Found don't()" << '\n';
        outValue = false;
        return true;
    }

    // Test do()
    if (input.substr(i, 4) == "do()")
    {
        cout << "Found don't()" << '\n';
        outValue = true;
        return true;
    }

    return false;
}

int main(int argc, char* argv[])
{
    string input;
    ReadFile(input);

    int result1 = 0, result2 = 0;
    bool canMultiply = true;
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == 'm')
        {
            int value = 0;
            ExtractMul(input, i, value);
            result1 += value;
            if (canMultiply)
            {
                result2 += value;
            }
        }
        else if (input[i] == 'd')
        {
            ExtractDoDont(input, i, canMultiply);
        }
    }

    // Part 1
    cout << result1 << '\n';

    // Part 2
    cout << result2 << '\n';
    return 0;
}
