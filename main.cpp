#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <fstream>

using namespace std;

bool break_loop = false;
int programCounter = 0;

int instructionRegister;
int addressRegister;
int accumulator;

int maxMemory = 99;
vector<int> ram(maxMemory, 0);

// use the global instruction register to execute each instruction
void direct() {
    switch (instructionRegister) {
//      HLT
        case 0:
            cout << "HALT" << endl;
            break_loop = true;
            break;

//      ADD
        case 1:
            accumulator += ram[addressRegister];
            break;

//      SUB
        case 2:
            accumulator -= ram[addressRegister];
            break;

//      STA
        case 3:
            ram[addressRegister] = accumulator;
            break;

//      LDA
        case 5:
            accumulator = ram[addressRegister];
            break;

//      BRA
        case 6:
            programCounter = addressRegister;
            break;

//      BRZ
        case 7:
            if (accumulator == 0) {
                programCounter = addressRegister;
            }
            break;

//      BRP
        case 8:
            if (accumulator >= 0) {
                programCounter = addressRegister;
            }
            break;

        case 9:
//          INP
            if (addressRegister == 1) {
                cout << "INPUT REQUIRED:";
                cin >> accumulator;
            }
//          OUT
            else if (addressRegister == 2) {
                cout << accumulator << endl;
            }
            break;

        default:
            throw invalid_argument("INVALID INTEGER AT ADDRESS: " + to_string(programCounter - 1));

    }
}

// Parse the file given as a vector of strings
vector<string> parse(const string &filename) {
    ifstream inputFile(filename);
    std::vector<std::string> lines;
    if (!inputFile.good()) {
        throw invalid_argument("File Does not exist");
    }
    for (std::string line; std::getline(inputFile, line);) {
        cout << line << endl;
        lines.push_back(line);
    }
    return lines;
}

// Convert any line that is only one opcode into an instruction
int ones(string instruction){
    if (instruction == "HLT") {
        return 0;
    } else if (instruction == "INP") {
        return 901;

    } else if (instruction == "OUT") {
        return 902;
    } else {
        throw invalid_argument("INVALID INSTRUCTION");
    }
}
int twos (vector<string>) {
    if (parsedSeperatedStrings[i][0] == "STA") {
        interpretedProgram.push_back(0);
    } else if (parsedSeperatedStrings[i][0] == "LDA") {
        interpretedProgram.push_back(901);

    } else if (parsedSeperatedStrings[i][0] == "ADD") {
        interpretedProgram.push_back(-1);
}
vector<int> interpret(const vector<string> &program) {
    vector<vector<string>> parsedSeperatedStrings;
    for (auto seperatedLine: program) {
        int pos = 0;
        string token;
        vector<string> seperatedInstructions;
        while (seperatedLine.find("\t") != string::npos) {
            seperatedInstructions.push_back(seperatedLine.substr(0, seperatedLine.find("\t")));
            seperatedLine.erase(0, pos);
        }
        parsedSeperatedStrings.push_back(seperatedInstructions);

    }
    vector<int> interpretedProgram;
    vector<string> variables;
    vector<string> loops;
    for (int i = 0; i < parsedSeperatedStrings.size(); ++i) {
        switch (parsedSeperatedStrings[i].size()) {
            case 1:
                interpretedProgram.push_back(ones(parsedSeperatedStrings[i][0]));
                break;

            case 2:

                    break;
                    case 3:



                    break;

                }
        }


    }
    return interpretedProgram;
}

int main(int argc, char *argv[]) {
    parse(argv[1]);
    (void) argc;
    while (!break_loop) {

        instructionRegister = ram[programCounter] / 100;
        addressRegister = ram[programCounter] % 100;
        programCounter += 1;
        direct();
    }
    return 0;
}
