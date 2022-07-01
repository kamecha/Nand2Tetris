#include <algorithm>
#include <vector>
#include <string>

#include "codeWriter.h"

// ../**/hoge.vm -> hoge.asm
string makeFileName(string path) {
	string tmp = path;
	// mv.egoh/**...
	reverse(tmp.begin(), tmp.end());
	auto itr = tmp.find('/');
	// mv.egoh
	tmp = tmp.substr(0, itr);
	// hoge.mv
	reverse(tmp.begin(), tmp.end());
	itr =  tmp.find('.');
	// hoge.asm
	tmp = tmp.substr(0, itr) + ".asm";
	return tmp;
}

int main(int arg, char* argv[]) {
	Parser parser(argv[arg-1]);
	vector<string> args(argv, argv + arg);
	string filePath = args[arg-1];
	CodeWriter code(makeFileName(filePath));
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
