#include<iostream>
#include<fstream>
#include<string>
#include<bitset>

#include"parser.h"
#include"code.h"
#include"symbolTable.h"
using namespace std;

void checkSymbol(Parser &parser, SymbolTable &symbol) {
	//バイナリコードで対応するアドレス
	int romPtr = 0;
	parser.reset();
	while(parser.hasMoreCommands()) {
		parser.advance();
		switch(parser.commandType()) {
			case A_COMMAND:	//@Xxx
			case C_COMMAND:	//dest=comp;jump
				romPtr++;
				break;
			case L_COMMAND:	//(Xxx)
				//(Xxx)の後に命令がないとダメ
				if(symbol.contains(parser.symbol()) == false) {
					symbol.addEntry(parser.symbol(), romPtr);
				}
			break;
		}
	}
}

bool is_digits(const std::string &str) {
	return std::all_of(str.begin(), str.end(), ::isdigit);
}

void codeGeneration(Parser &parser, Code &code, SymbolTable &symbol, ofstream &hack) {
	parser.reset();
	//メモリの変数開始地点(0x0010)
	const int ramHeadPtr = 16;
	int ramPtr = 0;
	while(parser.hasMoreCommands()) {
		parser.advance();
		string command;
		//@value
		if(parser.commandType() == A_COMMAND) {
			string sym = parser.symbol();
			if(is_digits(sym)) {
				//@100
				bitset<15> value(stoi(sym));
				hack << "0" + value.to_string() << endl;
			} else if(symbol.contains(sym)) {
				//@Label
				bitset<15> value(symbol.getAddress(sym));
				hack << "0" + value.to_string() << endl;
			} else {
				//@variable
				symbol.addEntry(sym, ramHeadPtr + ramPtr);
				ramPtr++;
				bitset<15> value(symbol.getAddress(sym));
				hack << "0" + value.to_string() << endl;
			}
		}
		//dest=comp;jump
		if(parser.commandType() == C_COMMAND) {
			string command;
			command += code.comp(parser.comp());
			command += code.dest(parser.dest());
			command += code.jump(parser.jump());
			hack << "111" + command << endl;
		}
	}
}

int main(int arg, char* argv[]) {
	Parser parser(argv[arg-1]);
	Code code = Code();
	SymbolTable symbol = SymbolTable();
	ofstream hack;
	string fileName = "hoge.hack";
	hack.open(fileName, ios::out);
	checkSymbol(parser, symbol);
	codeGeneration(parser, code, symbol, hack);
	hack.close();
}
