#include <fstream>

#include "codeWriter.h"
using namespace std;

CodeWriter::CodeWriter(string fileName) {
	file.open(fileName, ios::out);
}

void CodeWriter::setFileName(string fileName) {
	CodeWriter::close();
	file.open(fileName, ios::out);
}

void CodeWriter::writeArithmetic(string command) {
	if(command == "add") {
	}
}

void CodeWriter::close() {
	file.close();
}
