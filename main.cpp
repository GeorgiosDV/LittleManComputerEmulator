#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

bool break_loop = false;
int programCounter = 0;

int instructionRegister;
int addressRegister;
int accumulator;

int maxMemory = 99;
vector<int> ram(maxMemory, 0);

void direct(){
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
        if (accumulator == 0){
            programCounter = addressRegister;
        }
        break;

//      BRP
        case 8:
            if (accumulator >= 0){
                programCounter = addressRegister;
            }
        break;

        case 9:
//          INP
            if (addressRegister == 1){
                cout << "INPUT REQUIRED:";
                cin >> accumulator;
            }
//          OUT
            else if (addressRegister == 2){
                cout << accumulator << endl;
            }
        break;

        default:
            throw std::invalid_argument("INVALID CODE");



    }
}

int main() {
   ram[0] = 901;
   ram[1] = 322;
   ram[2] = 323;
   ram[3] = 809;
   ram[4] = 322;
   ram[5] = 525;
   ram[6] = 222;
   ram[7] = 322;
   ram[8] = 323;
   ram[9] = 521;
   ram[10] = 122;
   ram[11] = 321;
   ram[12] = 523;
   ram[13] = 224;
   ram[14] = 323;
   ram[15] = 809;
   ram[16] = 521;
   ram[17] = 222;
   ram[18] = 321;
   ram[19] = 902;
   ram[24] = 1;


    while (!break_loop){
        instructionRegister = ram.at(programCounter) / 100;
        addressRegister = ram.at(programCounter) % 100 ;
        programCounter += 1;
        direct();
    }
    return 0;
}

