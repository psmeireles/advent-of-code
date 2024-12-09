
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

class Block
{
public:
    long long Value = -1;
    int Size = 1;
    Block(long long value) : Value(value) {}
    Block(long long value, int size) : Value(value), Size(size) {}
};

ostream& operator<<(ostream& os, const Block& block)
{
    os << "(" << block.Value << "," << block.Size << ")";
    return os;
}

ostream& operator<<(ostream& os, vector<Block>& blocks)
{
    for (auto block : blocks)
    {
        auto blockValue = to_string(block.Value);
        for (int i = 0; i < block.Size; i++)
        {
            os << (block.Value == -1 ? "." : blockValue);
        }
    }
    return os;
}

void ReadFilePart1(vector<Block>& blocks, vector<long long>& populatedIndexes, queue<long long>& emptyIndexes)
{
    ifstream file("input.txt");
    string line;
    while (getline(file, line))
    {
        for (int i = 0; i < line.length(); i++)
        {
            const int number = line[i] - '0';
            for (int j = 0; j < number; j++)
            {
                if (i % 2 == 0)
                {
                    blocks.push_back(Block(i / 2));
                    populatedIndexes.push_back(blocks.size() - 1);
                }
                else
                {
                    blocks.push_back(Block(-1));
                    emptyIndexes.push(blocks.size() - 1);
                }
            }
        }
    }
}

void ReadFilePart2(vector<Block>& blocks, vector<long long>& populatedIndexes, vector<long long>& gapIndexes)
{
    ifstream file("input.txt");
    string line;
    while (getline(file, line))
    {
        for (int i = 0; i < line.length(); i++)
        {
            const int number = line[i] - '0';
            if (i % 2 == 0)
            {
                blocks.push_back(Block(i / 2, number));
                populatedIndexes.push_back(blocks.size() - 1);
            }
            else if (number > 0)
            {
                blocks.push_back(Block(-1, number));
                gapIndexes.push_back(blocks.size() - 1);
            }
        }
    }
}

long long Checksum(vector<Block>& blocks)
{
    int i = 0;
    long long result = 0;
    for (auto block : blocks)
    {
        for (int j = 0; j < block.Size; j++)
        {
            if (block.Value == -1)
            {
                i++;
                continue;
            }
            result += block.Value * i;
            i++;
        }
    }
    return result;
}

int SwapBlock(vector<Block>& blocks, long long blockIndex, long long gapIndex)
{
    auto emptyBlock = blocks[gapIndex];
    auto block = blocks[blockIndex];
    if (block.Size > emptyBlock.Size)
    {
        return 0;
    }

    auto diff = emptyBlock.Size - block.Size;
    if (diff == 0)
    {
        // Sizes match. Just swap.
        iter_swap(blocks.begin() + blockIndex, blocks.begin() + gapIndex);
        return 0;
    }

    blocks[gapIndex] = block;
    blocks[blockIndex].Value = -1;
    // Next block is a gap. Just increase its size
    if (blocks[gapIndex + 1].Value == -1)
    {
        blocks[gapIndex + 1].Size += diff;
        return 0;
    }

    // Insert block in place of gap and create new gap with the right size.
    blocks.insert(blocks.begin() + gapIndex + 1, Block(-1, diff));
    return 1;
}

int main(int argc, char* argv[])
{
    vector<Block> blocks;
    vector<long long> populatedIndexes;
    queue<long long> emptyIndexes;
    ReadFilePart1(blocks, populatedIndexes, emptyIndexes);

    // Part 1
    while (emptyIndexes.empty() == false && populatedIndexes.back() > emptyIndexes.front())
    {
        auto emptyIndex = emptyIndexes.front();
        emptyIndexes.pop();

        auto lastPopulatedIndex = populatedIndexes.back();
        populatedIndexes.pop_back();

        // Swap elements
        iter_swap(blocks.begin() + lastPopulatedIndex, blocks.begin() + emptyIndex);
    }

    // Result
    cout << Checksum(blocks) << endl;

    // Reset
    blocks.clear();
    while (!populatedIndexes.empty()) {
        populatedIndexes.pop_back();
    }
    while (!emptyIndexes.empty()) {
        emptyIndexes.pop();
    }
    

    // Part 2
    vector<long long> gapStartIndexes;
    ReadFilePart2(blocks, populatedIndexes, gapStartIndexes);

    vector<int> offsets(populatedIndexes.back() + 1, 0);
    // cout << blocks << endl;
    while (populatedIndexes.empty() == false)
    {
        auto movedBlockIndex = populatedIndexes.back();
        auto movedBlock = blocks[movedBlockIndex];
        populatedIndexes.pop_back();

        for (int i = 0; i < movedBlockIndex; i++)
        {
            auto currBlock = blocks[i];
            if (currBlock.Value != -1)
            {
                // Not a gap, skip
                continue;
            }

            if (currBlock.Size >= movedBlock.Size)
            {
                int offset = SwapBlock(blocks, movedBlockIndex, i);\
                if (offset > 0)
                {
                    // We added a node. Update our indexes in the queue.
                    for (int j = 0; j < populatedIndexes.size(); j++)
                    {
                        if (populatedIndexes[j] >= i)
                        {
                            populatedIndexes[j] += offset;
                        }
                    }
                }
                break;
            }
        }
        // cout << blocks << endl;
    }


    // Result
    cout << Checksum(blocks) << endl;
    
    
    return 0;
}