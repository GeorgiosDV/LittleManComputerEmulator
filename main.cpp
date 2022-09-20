#include <iostream>
#include <vector>
using namespace std;

bool break_loop = false;
int programCounter = 0;


int instructionRegister;
int addressRegister;
int accumulator;

int maxMemory = 25;
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
    if (accumulator >= 0){
        programCounter = addressRegister;
    }
}

void direct(){
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
        cout << instructionRegister << "\t" << addressRegister << "\t" << accumulator << endl;
        direct();
    }
    return 0;
}

