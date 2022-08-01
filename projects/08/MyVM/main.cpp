#include <algorithm>
#include <fstream>
#include <ostream>
#include <vector>
#include <string>
#include <filesystem>

#include "codeWriter.h"

void generateFromFolder(string path, CodeWriter &code) {
	Parser parser(path);
	while(parser.hasMoreCommands()) {
		parser.advance();
		switch(parser.commandType()) {
			case C_ARITHEMETIC:
				code.writeArithmetic(parser.arg1());
				break;
			case C_PUSH:
			case C_POP:
				code.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
				break;
			case C_LABEL:
				code.writeLabel(parser.arg1());
				break;
			case C_GOTO:
				code.writeGoto(parser.arg1());
				break;
			case C_IF:
				code.writeIf(parser.arg1());
				break;
			case C_FUNCTION:
				code.writeFunction(parser.arg1(), parser.arg2());
				break;
			case C_CALL:
				code.writeCall(parser.arg1(), parser.arg2());
				break;
			case C_RETURN:
				code.writeReturn();
				break;
			default:
				break;
		}
	}
}

void generateFromDirectory(string path, CodeWriter &code) {
	for(const filesystem::directory_entry& entry: filesystem::directory_iterator(path)) {
		if(entry.path().extension() == ".vm") {
			cout << ".vmが存在したよ！" << endl;
			cout << entry.path() << endl;
			generateFromFolder(entry.path(), code);
		}
	}
}

int main(int arg, char* argv[]) {
	filesystem::path path = argv[arg-1];
	string fileName = path.stem().string() + ".asm";
	cout << "fileName:" << fileName << endl;
	CodeWriter code(fileName);
	if(filesystem::is_directory(filesystem::status(path))) {
		cout << "direcotry" << endl;
		code.writeInit();
		generateFromDirectory(path, code);
	} else {
		code.writeInit();
		cout << "file(path)から開いたよ" << endl;
		generateFromFolder(path, code);
	}
	return 0;
}
