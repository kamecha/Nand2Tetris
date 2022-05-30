#include <string>

#include "symbolTable.h"
using namespace std;

SymbolTable::SymbolTable() {
	symbolTable["SP"] = 0;
	symbolTable["LCL"] = 1;
	symbolTable["ARG"] = 2;
	symbolTable["THIS"] = 3;
	symbolTable["THAT"] = 4;
	symbolTable["R0"] = 0;
	symbolTable["R1"] = 1;
	symbolTable["R2"] = 2;
	symbolTable["R3"] = 3;
	symbolTable["R4"] = 4;
	symbolTable["R5"] = 5;
	symbolTable["R6"] = 6;
	symbolTable["R7"] = 7;
	symbolTable["R8"] = 8;
	symbolTable["R9"] = 9;
	symbolTable["R10"] = 10;
	symbolTable["R11"] = 11;
	symbolTable["R12"] = 12;
	symbolTable["R13"] = 13;
	symbolTable["R14"] = 14;
	symbolTable["R15"] = 15;
	symbolTable["SCREEN"] = 16384;	//0x4000
	symbolTable["KBD"] = 24576;		//0x6000
}

void SymbolTable::addEntry(string symbol, int address) {
	symbolTable[symbol] = address;
}

bool SymbolTable::contains(string symbol) {
	auto findSymbol = symbolTable.find(symbol);
	if(findSymbol == symbolTable.end()) {
		return false;
	} else {
		return true;
	}
}

int SymbolTable::getAddress(string symbol) {
	return symbolTable[symbol];
}
