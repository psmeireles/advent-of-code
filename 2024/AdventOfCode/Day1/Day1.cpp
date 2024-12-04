#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void ReadFile(vector<int>& arr1, vector<int>& arr2)
{
    string line;
    std::ifstream file("input.txt");

    while (getline(file, line))
    {
        istringstream ss(line);
        int x, y;
        ss >> x >> y;
        arr1.push_back(x);
        arr2.push_back(y);
    }
    file.close();
}

int main(int argc, char* argv[])
{
    vector<int> arr1, arr2;
    ReadFile(arr1, arr2);

    sort(arr1.begin(), arr1.end());
    sort(arr2.begin(), arr2.end());
    
    int answer1 = 0;
    for (int i = 0; i < arr1.size(); i++)
    {
        answer1 += abs(arr1[i] - arr2[i]);
    }
    
    // Part 1
    cout << answer1 << '\n';

    map<int, int> count1In2;
    for (int x : arr1)
    {
        count1In2[x] = 0;
    }

    for (int x : arr2)
    {
        count1In2[x]++;
    }

    int answer2 = 0;
    for (int x : arr1)
    {
        answer2 += x * count1In2[x];
    }

    // Part 2
    cout << answer2 << '\n';
    return 0;
}
