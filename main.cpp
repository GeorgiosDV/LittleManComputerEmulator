#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

const int HLT = 0, ADD = 1, SUB = 2, STA = 3, LDA = 5,
          BRA = 6, BRZ = 7, BRP = 8, IO = 9;

bool break_loop = false;
int programCounter = 0;

int instructionRegister;
int addressRegister;
int accumulator;

int maxMemory = 100;
vector<int> ram(maxMemory, 0);

bool compileOnly = false;
bool unlimitedMemory = false;
bool quietMode = false;

int leadingPlace(int n) {
	int place = 1;
	while (n / place >= 10) {
		place *= 10;
	}
	return place;
}

void checkAddress(int address) {
	if (unlimitedMemory && address >= maxMemory) {
		maxMemory = address + 1;
		ram.resize(maxMemory, 0);
	}
	if (address < 0 || address >= maxMemory) {
		throw runtime_error("ADDRESS OUT OF RANGE: " + to_string(address));
	}
}

void decode() {
	int word = ram[programCounter];
	if (unlimitedMemory && word >= 1000) {
		int place = leadingPlace(word);
		instructionRegister = word / place;
		addressRegister = word % place;
	} else {
		instructionRegister = word / 100;
		addressRegister = word % 100;
	}
}

void execute() {
	switch (instructionRegister) {
		case HLT:
			if (!quietMode) {
				cout << "HALT" << endl;
			}
			break_loop = true;
			break;
		case ADD:
			accumulator += ram[addressRegister];
			break;
		case SUB:
			accumulator -= ram[addressRegister];
			break;
		case STA:
			ram[addressRegister] = accumulator;
			break;
		case LDA:
			accumulator = ram[addressRegister];
			break;
		case BRA:
			programCounter = addressRegister;
			break;
		case BRZ:
			if (accumulator == 0) {
				programCounter = addressRegister;
			}
			break;
		case BRP:
			if (accumulator >= 0) {
				programCounter = addressRegister;
			}
			break;
		case IO:
			if (addressRegister == 1) {
				if (!quietMode) {
					cout << "INPUT REQUIRED:";
				}
				while (!(cin >> accumulator)) {
					if (cin.eof()) {
						if (!quietMode) {
							cout << endl;
						}
						throw runtime_error("NO INPUT AVAILABLE AT ADDRESS: " + to_string(programCounter - 1));
					}
					cin.clear();
					string skipped;
					getline(cin, skipped);
					if (!quietMode) {
						cout << "INVALID INPUT, ENTER AN INTEGER:";
					}
				}
			} else if (addressRegister == 2) {
				cout << accumulator << endl;
			}
			break;
		default:
			throw invalid_argument("INVALID INTEGER AT ADDRESS: " + to_string(programCounter - 1));
	}
}

void run() {
	while (!break_loop) {
		checkAddress(programCounter);
		decode();
		checkAddress(addressRegister);
		programCounter += 1;
		execute();
	}
}

void readInstructions(istream& input) {
	string inputLine;
	int inputNumber;
	int inputCount = 0;

	while ((unlimitedMemory || inputCount < maxMemory) && getline(input, inputLine)) {
		if (inputLine.empty()){
			break;
		}
		try {
			inputNumber = stoi(inputLine);
			checkAddress(inputCount);
			ram[inputCount++] = inputNumber;
		} catch (const invalid_argument& ia) {
			cerr << "Invalid input. Please enter only integer numbers." << endl;
		}
	}
}

map<string, int> labels;

struct ParsedLine {
	string mnemonic;
	string operand;
	int lineNumber;
};

int findOpcode(string mnemonic) {
	if (mnemonic == "HLT" || mnemonic == "DAT") {
		return HLT;
	} else if (mnemonic == "ADD") {
		return ADD;
	} else if (mnemonic == "SUB") {
		return SUB;
	} else if (mnemonic == "STA") {
		return STA;
	} else if (mnemonic == "LDA") {
		return LDA;
	} else if (mnemonic == "BRA") {
		return BRA;
	} else if (mnemonic == "BRZ") {
		return BRZ;
	} else if (mnemonic == "BRP") {
		return BRP;
	} else if (mnemonic == "INP" || mnemonic == "OUT") {
		return IO;
	}
	return -1;
}

int findLabel(string name) {
	auto it = labels.find(name);
	if (it != labels.end()) {
		return it->second;
	}
	return -1;
}

int resolveOperand(string operand, int lineNumber) {
	int labelAddress = findLabel(operand);
	if (labelAddress != -1) {
		return labelAddress;
	}
	try {
		return stoi(operand);
	} catch (const invalid_argument& ia) {
		throw runtime_error("UNKNOWN LABEL '" + operand + "' ON LINE " + to_string(lineNumber));
	}
}

int encodeInstruction(int opcode, int address) {
	if (address < 100) {
		return opcode * 100 + address;
	}
	int place = leadingPlace(address) * 10;
	return opcode * place + address;
}

int assembleInstructions(istream& input) {
	vector<ParsedLine> parsed;
	string inputLine;
	int lineNumber = 0;

	while (getline(input, inputLine)) {
		lineNumber++;
		if (inputLine.find("//") != string::npos) {
			inputLine = inputLine.substr(0, inputLine.find("//"));
		}

		istringstream words(inputLine);
		string first, mnemonic, operand;
		if (!(words >> first)) {
			continue;
		}
		if (findOpcode(first) != -1) {
			mnemonic = first;
		} else {
			if (labels.count(first)) {
				throw runtime_error("LABEL '" + first + "' DEFINED TWICE ON LINE " + to_string(lineNumber));
			}
			labels[first] = parsed.size();
			words >> mnemonic;
		}
		words >> operand;

		if (findOpcode(mnemonic) == -1) {
			throw runtime_error("UNKNOWN INSTRUCTION ON LINE " + to_string(lineNumber));
		}
		if (!unlimitedMemory && (int)parsed.size() >= maxMemory) {
			throw runtime_error("PROGRAM TOO LARGE FOR MEMORY AT LINE " + to_string(lineNumber));
		}
		parsed.push_back({mnemonic, operand, lineNumber});
	}

	int programSize = parsed.size();
	for (int address = 0; address < programSize; address++) {
		string mnemonic = parsed[address].mnemonic;
		string operand = parsed[address].operand;
		int value = 0;

		if (mnemonic == "HLT") {
			value = encodeInstruction(HLT, 0);
		} else if (mnemonic == "INP") {
			value = encodeInstruction(IO, 1);
		} else if (mnemonic == "OUT") {
			value = encodeInstruction(IO, 2);
		} else if (mnemonic == "DAT") {
			if (!operand.empty()) {
				value = resolveOperand(operand, parsed[address].lineNumber);
			}
		} else if (operand.empty()) {
			throw runtime_error(mnemonic + " ADDRESS REQUIRED ON LINE " + to_string(parsed[address].lineNumber));
		} else {
			int target = resolveOperand(operand, parsed[address].lineNumber);
			if (target < 0 || (!unlimitedMemory && target >= maxMemory)) {
				throw runtime_error("ADDRESS OUT OF RANGE ON LINE " + to_string(parsed[address].lineNumber));
			}
			value = encodeInstruction(findOpcode(mnemonic), target);
		}
		checkAddress(address);
		ram[address] = value;
	}
	return programSize;
}

void writeInstructions(string path, int count) {
	ofstream file(path);
	if (!file) {
		throw runtime_error("FAILED TO WRITE FILE: " + path);
	}
	for (int address = 0; address < count; address++) {
		string number = to_string(ram[address]);
		while (ram[address] >= 0 && number.size() < 3) {
			number = "0" + number;
		}
		file << number << endl;
	}
}

bool endsWith(const string& text, const string& suffix) {
	return text.size() >= suffix.size() &&
	       text.compare(text.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool loadProgram(const string& path) {
	bool isAssembly = endsWith(path, ".lmcp");
	if (compileOnly && !isAssembly) {
		throw runtime_error("-c needs a .lmcp file");
	}
	ifstream file(path);
	if (!file) {
		throw runtime_error("Failed to open file: " + path);
	}
	if (!isAssembly) {
		readInstructions(file);
		return true;
	}
	int programSize = assembleInstructions(file);
	if (compileOnly) {
		string outputPath = path.substr(0, path.size() - 5) + ".lmcb";
		writeInstructions(outputPath, programSize);
		if (!quietMode) {
			cout << "Wrote " << outputPath << endl;
		}
		return false;
	}
	return true;
}

void printHelp() {
	cout << "Usage: ./LMC [flags] <file.lmcp or file.lmcb>" << endl;
	cout << endl;
	cout << "\t-c, --compile\tcompile a .lmcp into .lmcb" << endl;
	cout << "\t-u, --unlimited\tunlimited addresses" << endl;
	cout << "\t-q, --quiet\tonly print the program's output" << endl;
	cout << "\t-h, --help\tshow this message" << endl;
}

int main(int argc, char* argv[]) {
	string path;

	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if (arg == "-c" || arg == "--compile") {
			compileOnly = true;
		} else if (arg == "-u" || arg == "--unlimited") {
			unlimitedMemory = true;
		} else if (arg == "-q" || arg == "--quiet") {
			quietMode = true;
		} else if (arg == "-h" || arg == "--help") {
			printHelp();
			return 0;
		} else if (arg[0] == '-') {
			cerr << "Unknown flag: " << arg << " (use -h to see all flags)" << endl;
			return 1;
		} else {
			path = arg;
		}
	}

	try {
		bool shouldRun = false;
		if (!path.empty()) {
			shouldRun = loadProgram(path);
		} else if (compileOnly) {
			throw runtime_error("Usage: ./LMC -c <file.lmcp>");
		} else {
			if (!quietMode) {
				cout << "Enter instructions:" << endl;
			}
			readInstructions(cin);
			shouldRun = true;
		}

		if (shouldRun) {
			run();
		}
	} catch (const exception& e) {
		cerr << e.what() << endl;
		return 1;
	}

	return 0;
}
