#include <iostream>
#include <fstream>
#include <regex>
using namespace std;

struct Instruction {
    string label;
    string operation;
    string operand1;
    string operand2;
};

Instruction parseInstruction(string line) {
    Instruction instruction;
    istringstream stream(line);

    size_t colonPos = line.find(':');
    if (colonPos != string::npos) {
        instruction.label = line.substr(0, colonPos);
        stream.seekg(colonPos + 1);
    }

    stream >> instruction.operation;

    string operands;
    getline(stream, operands);
    istringstream operandStream(operands);

    getline(operandStream, instruction.operand1, ',');
    getline(operandStream, instruction.operand2, ',');

    auto trim = [](string& str) {
        str.erase(0, str.find_first_not_of(' '));
        str.erase(str.find_last_not_of(' ') + 1);
    };

    trim(instruction.operand1);
    trim(instruction.operand2);

    return instruction;
}

string formatLine(string line) {
    /* remove comments and unecessary spacing */
    line = regex_replace(line, regex(";.*|^\\s+|\\s+$"), "");
    line = regex_replace(line, regex("\\s+"), " ");

    return line;
}

bool isLabeled(string line) {
    return regex_search(line, regex("^\\S+:"));
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file_path>\n";
        return 1;
    }

    string filePath = argv[1];
    ifstream inputFile(filePath);
    if (!inputFile) {
        cerr << "Error: Unable to open file " << filePath << "\n";
        return 1;
    }

    /* first pass */
    string line;
    int line_counter = 1;
    int address_counter = 0;
    while (getline(inputFile, line)) {
        line = formatLine(line);
        if (!empty(line)){
            Instruction inst = parseInstruction(line);
            cout << "Label: " << inst.label
                 << ", Operation: " << inst.operation
                 << ", Op1: " << inst.operand1
                 << ", Op2: " << inst.operand2 << "\n";
        }
    }

    return 0;
}