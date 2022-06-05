#include "codeWriter.h"

int main(int arg, char* argv[]) {
	Parser parser(argv[arg-1]);
	string fileName = "hoge.asm";
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
