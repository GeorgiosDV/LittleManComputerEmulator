#include <iostream>
#include <vector>
using namespace std;

bool break_loop = false;
int programCounter = 0;


int instructionRegister;
int addressRegister;
int accumulator;

int maxMemory = 99;
vector<int> ram(maxMemory, 0);

void HLT(){
    cout << "HALT" << endl;
    break_loop = true;
}
void INP(){
    cout << "INPUT REQUIRED:";
    cin >> accumulator;
}
void OUT(){
    cout << accumulator << endl;
}
void ADD(){
    accumulator += ram[addressRegister];
}

void SUB(){
    accumulator -= ram[addressRegister] ;

}

void STA(){
    ram[addressRegister] = accumulator;
}

void LDA(){
    accumulator = ram[addressRegister];
}

void BRA(){
    programCounter = addressRegister;
}

void BRZ(){
    if (accumulator == 0){
        programCounter = addressRegister;
    }

}
void BRP(){
    if (addressRegister < 0){
        programCounter = addressRegister;
    }
}

int direct(){
    switch (instructionRegister) {
//      HLT
        case 0:
        HLT();
        break;
//      ADD
        case 1:
        ADD();
        break;
//      SUB
        case 2:
        SUB();
        break;
//      STA
        case 3:
        STA();
        break;
//      LDA
        case 5:
        LDA();
        break;
//      BRA
        case 6:
        BRA();
        break;
//      BRZ
        case 7:
        BRZ();
        break;
//      BRP
        case 8:
        BRP();
        break;
        case 9:
            if (addressRegister == 1){
                INP();
            }
            else if (addressRegister == 2){
                OUT();
            }
            break;

    }
}

int main() {
    ram[0] = 504;
    ram[1] = 205;
    ram[2] = 306;
    ram[3] = 902;
    ram[4] = 005;
    ram[5] = 020;

    while (!break_loop){
        instructionRegister = ram.at(programCounter) / 100;
        addressRegister = ram.at(programCounter) - (instructionRegister * 100);
        programCounter += 1;
//        cout << instructionRegister << "\t" << addressRegister<< endl;
        direct();
    }
}

