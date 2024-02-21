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

void direct() {
    switch (instructionRegister) {
        case 0: // HLT
            cout << "HALT" << endl;
            break_loop = true;
            break;
        case 1: // ADD
            accumulator += ram[addressRegister];
            break;
        case 2: // SUB
            accumulator -= ram[addressRegister];
            break;
        case 3: // STA
            ram[addressRegister] = accumulator;
            break;
        case 5: // LDA
            accumulator = ram[addressRegister];
            break;
        case 6: // BRA
        case 7: // BRZ
            if (instructionRegister == 6 || (instructionRegister == 7 && accumulator == 0)) {
                programCounter = addressRegister;
            }
            break;
        case 8: // BRP
            if (accumulator >= 0) {
                programCounter = addressRegister;
            }
            break;
        case 9: // INP/OUT
            if (addressRegister == 1) {
                cout << "INPUT REQUIRED:";
                cin >> accumulator;
            } else if (addressRegister == 2) {
                cout << accumulator << endl;
            }
            break;
        default:
            throw invalid_argument("INVALID INTEGER AT ADDRESS: " + to_string(programCounter - 1));
    }
}


void readInstructions(istream& input) {
    string inputLine;
    int inputNumber;
    int inputCount = 0;
    
    while (inputCount < maxMemory && getline(input, inputLine)) {
        if (inputLine.empty()){ 
            break;
        }
        try {
            inputNumber = stoi(inputLine);
            ram[inputCount++] = inputNumber;
        } catch (const invalid_argument& ia) {
            cerr << "Invalid input. Please enter only integer numbers." << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {  // Check if a file was given using the command line
        ifstream file(argv[1]);
        if (file) {
            readInstructions(file);
        } else {
            cerr << "Failed to open file: " << argv[1] << endl;
            return 1; 
        }
    } else {
        cout << "Enter instructions:" << endl;
        readInstructions(cin);
    }

    while (!break_loop) {
        instructionRegister = ram[programCounter] / 100;
        addressRegister = ram[programCounter] % 100;
        programCounter += 1;
        direct();
    }

    return 0;
}
