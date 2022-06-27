#include <vector>
#include <string>

#include "codeWriter.h"

int main(int arg, char* argv[]) {
	Parser parser(argv[arg-1]);
	vector<string> args(argv, argv + arg);
	string fileName = args[arg-1];
	auto itr = fileName.find('.');
	fileName = fileName.substr(0, itr) + ".asm";
	CodeWriter code(fileName);
	while(parser.hasMoreCommands()) {
		parser.advance();
		switch(parser.commandType()) {
			case C_ARITHEMETIC:
				code.writeArithmetic(parser.arg1());
				break;
			case C_PUSH:
				code.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
				break;
			default:
				break;
		}
	}
	return 0;
}
