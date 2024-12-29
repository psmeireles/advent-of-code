
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void ReadFile(vector<vector<int>>& keys, vector<vector<int>>& locks)
{
    ifstream file("input.txt");
    string line1, line2, line3, line4, line5, line6, line7, empty;
    while (
        getline(file, line1) &&
        getline(file, line2) &&
        getline(file, line3) &&
        getline(file, line4) &&
        getline(file, line5) &&
        getline(file, line6) &&
        getline(file, line7)
    )
    {
        vector<string> block{
            line1, line2, line3, line4, line5, line6, line7
        };
        if (line1 == "#####")
        {
            // Lock
            vector<int> lock;
            for (int j = 0; j < 5; j++)
            {
                for (int i = 0; i < 7; i++)
                {
                    if (block[i][j] == '.')
                    {
                        lock.push_back(i - 1);
                        break;
                    }
                }
            }
            locks.push_back(lock);
        }
        else
        {
            // Key
            vector<int> key;
            for (int j = 0; j <= 5; j++)
            {
                for (int i = 6; i >= 0; i--)
                {
                    if (block[i][j] == '.')
                    {
                        key.push_back(5 - i);
                        break;
                    }
                }
            }
            keys.push_back(key);
        }
        
        // Consume white line
        getline(file, empty);
    }
}

int main(int argc, char* argv[])
{
    vector<vector<int>> keys, locks;
    ReadFile(keys, locks);

    int result1 = 0;
    for (auto key : keys)
    {
        for (auto lock : locks)
        {
            bool isValid = true;
            for (int i = 0; i < key.size(); i++)
            {
                if (key[i] + lock[i] >= 6)
                {
                    isValid = false;
                    break;
                }
            }
            if (isValid)
            {
                result1++;
            }
        }
    }

    // Part 1 - Answer: 3451
    cout << "Part 1: " << result1 << endl;
    
    return 0;
}
