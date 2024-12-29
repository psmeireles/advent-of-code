
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>
using namespace std;

class Operation
{
public:
    string Operator;
    string Wire1;
    string Wire2;
    string WireResult;

    Operation(string Wire1, string Operator, string Wire2, string WireResult = "") : Operator(Operator), Wire1(Wire1),
        Wire2(Wire2), WireResult(WireResult)
    {
    }
    
    bool Operate(map<string, int>& wires, int& outResult)
    {
        if (wires.count(Wire1) == 0 || wires.count(Wire2) == 0)
        {
            // Wires aren't ready
            return false;
        }
        
        if (Operator == "OR")
        {
            outResult = wires.at(Wire1) | wires.at(Wire2);
        }
        else if (Operator == "AND")
        {
            outResult = wires.at(Wire1) & wires.at(Wire2);
        }
        else if (Operator == "XOR")
        {
            outResult = wires.at(Wire1) ^ wires.at(Wire2);
        }
        else
        {
            // Invalid operation
            return false;
        }
        return true;
    }

    string ToString() const
    {
        return Wire1 + Operator + Wire2;
    }
    
    bool operator<(const Operation& op) const
    {
        return ToString() < op.ToString();
    }

};

ostream& operator<<(ostream& os, Operation& operation)
{
    os << operation.Wire1 << " " << operation.Operator << " " << operation.Wire2 << " -> " << operation.WireResult;
    return os;
}

void ReadFile(map<string, int>& wires, queue<Operation>& operations, map<Operation, string>& operationsMap)
{
    ifstream file("input.txt");
    string line;
    while (getline(file, line))
    {
        if (line.find(':') != string::npos)
        {
            auto key = line.substr(0, 3);
            auto value = stoi(line.substr(line.find(':') + 1));
            wires[key] = value;
        }
        else if (line.find("->") != string::npos)
        {
            string wire1 = line.substr(0, line.find(' '));
            line.erase(0, line.find(' ') + 1);
            string op = line.substr(0, line.find(' '));
            line.erase(0, line.find(' ') + 1);
            string wire2 = line.substr(0, line.find(' '));
            line.erase(0, line.find(' ') + 4);
            string wireResult = line;
            
            Operation operation(wire1, op, wire2, wireResult);
            operations.push(operation);
            operationsMap[operation] = operation.WireResult;
        }
    }
}

int main(int argc, char* argv[])
{
    map<string, int> wires;
    map<Operation, string> operationToWire;
    queue<Operation> operations;
    ReadFile(wires, operations, operationToWire);

    // Part 1
    {
        // Calculate operations
        while (operations.empty() == false)
        {
            auto operation = operations.front();
            int result;
            if (operation.Operate(wires, result))
            {
                // Wires are ready. Compute result.
                wires[operation.WireResult] = result;
                operations.pop();
            }
            else
            {
                // Not ready yet. Place back in the queue.
                operations.pop();
                operations.push(operation);
            }
        }

        // Building result
        vector<int> v;
        for (auto kvp : wires)
        {
            if (kvp.first.find('z') == 0)
            {
                v.emplace(v.begin(), kvp.second);
            }
        }

        long long result1 = 0;
        for (auto x : v)
        {
            result1 |= x;
            result1 <<= 1;
        }
        // Have to shift 1 back because the last iteration shifter 1 more.
        result1 >>= 1;

        // Part 1 - Answer: 49430469426918
        cout << "Part 1: " << result1 << endl;
    }
    
    // Part 2
    {

        // Reproduce the circuit (https://www.researchgate.net/figure/A-binary-adder-built-out-of-logical-gates_fig2_228732027)
        // and check when something goes wrong. Add the change to the list and run again until no more mistakes are found. 
        
        // Wire changes
        vector<Operation> operationsToBeChanged{
            Operation("x11", "XOR", "y11", "qnw"),
            Operation("x11", "AND", "y11", "qff"),
            Operation("dfn", "XOR", "qcr", "z16"),
            Operation("x16", "AND", "y16", "pbv"),
            Operation("cts", "XOR", "bcd", "z23"),
            Operation("wdr", "OR", "jcd", "qqp"),
            Operation("jdd", "XOR", "rbm", "z36"),
            Operation("jdd", "AND", "rbm", "fbq"),
        };
        vector<string> changedWires{
            "qnw",
            "qff",
            "z16",
            "pbv",
            "z23",
            "qqp",
            "z36",
            "fbq",
        };
        for (auto operation : operationsToBeChanged)
        {
            operationToWire[operation] = operation.WireResult;
        }
        
        string inC;
        for (int i = 0; i < 45; i++)
        {
            string iAsString = i < 10 ?  "0" + to_string(i) : to_string(i);
            string x = "x" + iAsString, y = "y" + iAsString, z = "z" + iAsString;
            
            // x[i] XOR y[i] -> v1
            Operation xXORy(x, "XOR", y);
            Operation yXORx(y, "XOR", x);
            string v1 = operationToWire.count(xXORy) > 0 ? operationToWire[xXORy] : operationToWire[yXORx];

            // x[i] AND y[i] -> v3
            Operation xANDy(x, "AND", y);
            Operation yANDx(y, "AND", x);
            string v3 = operationToWire.count(xANDy) > 0 ? operationToWire[xANDy] : operationToWire[yANDx];

            if (i > 0)
            {
                // v1 XOR inC -> z[i]
                Operation v1XORinC(v1, "XOR", inC);
                Operation inCXORv1(inC, "XOR", v1);
                string resultWire = operationToWire.count(v1XORinC) > 0 ? operationToWire[v1XORinC] : operationToWire[inCXORv1];

                if (resultWire != z)
                {
                    cout << v1XORinC << " should be " << z << " but is " << v1 << endl;
                }

                // v1 AND inC -> v2
                Operation v1ANDinC(v1, "AND", inC);
                Operation inCANDv1(inC, "AND", v1);
                string v2 = operationToWire.count(v1ANDinC) > 0 ? operationToWire[v1ANDinC] : operationToWire[inCANDv1];
                if (v2.empty())
                {
                    cout << v1ANDinC << " is an invalid operation" << endl;
                }

                // v2 OR v3 -> carry
                Operation v2ORv3(v2, "OR", v3);
                Operation v3ORv2(v3, "OR", v2);
                string carry = operationToWire.count(v2ORv3) > 0 ? operationToWire[v2ORv3] : operationToWire[v3ORv2];
                if (carry.empty())
                {
                    cout << v2ORv3 << " is an invalid operation" << endl;
                }
                inC = carry;
            }
            else
            {
                if (v1 != "z00")
                {
                    cout << xXORy << " should be z00 but is " << v1 << endl;
                }
                
                // There's no carry in, so new inC is just v3.
                inC = v3;
            }
        }

        // Part 2 - Answer: fbq,pbv,qff,qnw,qqp,z16,z23,z36
        sort(changedWires.begin(), changedWires.end());
        cout << "Part 2: ";
        for (int i = 0; i < changedWires.size(); i++)
        {
            cout << changedWires[i] << (i == changedWires.size() - 1 ? "\n" : ",");
        }
    }
    
    return 0;
}
