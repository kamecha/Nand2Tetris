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

// hack言語を出力するようにする
// add
// sub
// neg
// eq
// gt
// lt
// and
// or
// not

/*
SP:		    *
stack:	x y 

SP:		*
stack:	x y 
R13: y
D: x
*/
void checkLeftRightOperands(ofstream &file) {
	file << "@SP" << endl;
	file << "AM=M-1" << endl;
	// D=M[SP]	D=y
	file << "D=M" << endl;
	// R[13]=D	R[13]=y
	file << "@R13" << endl;
	file << "M=D" << endl;
	/*
	SP:		*
	stack:	x y 
	*/
	file << "@SP" << endl;
	file << "AM=M-1" << endl;
	// D=M[SP]	D=x
	file << "D=M" << endl;
}

// SP++
void addStackPtr(ofstream &file) {
	file << "@SP" << endl;
	file << "M=M+1" << endl;
}

// x+y
void add(ofstream &file) {
	checkLeftRightOperands(file);
	// D=R[13]+D	D=y+x
	file << "@13" << endl;
	file << "D=D+M" << endl;
	/*
	SP:		    *
	stack:	x+y y
	*/
	// R[SP]=y+x
	file << "@SP" << endl;
	file << "A=M" << endl;
	file << "M=D" << endl;
	addStackPtr(file);
}

// x-y
void sub(ofstream &file) {
	checkLeftRightOperands(file);
	// D=D-R[13]	D=x-y
	file << "@R13" << endl;
	file << "D=D-M" << endl;
	/*
	SP:		    *
	stack:	x-y y
	*/
	// R[SP]=x-y
	file << "@SP" << endl;
	file << "A=M" << endl;
	file << "M=D" << endl;
	addStackPtr(file);
}

// -y
void neg(ofstream &file) {
	// SP--
	file << "@SP" << endl;
	file << "AM=M-1" << endl;
	// M[SP] = -M[SP]
	file << "M=-M" << endl;
	addStackPtr(file);
}

// x=y -> true
void eq(ofstream &file) {
	static int count = 0;
	string eq = "EQ:";
	string trueFlag = eq + "TRUE" + to_string(count);
	string endFlag = eq + "END" + to_string(count);
	count++;
	file << "// start eq" << endl;
	checkLeftRightOperands(file);
	// D=x-y
	file << "@R13" << endl;
	file << "D=D-M" << endl;
	// x=y -> true
	file << "@" << trueFlag << endl;
	file << "D;JEQ" << endl;
	// false
	file << "@SP" << endl;
	file << "A=M" << endl;
	file << "M=0" << endl;
	file << "@" << endFlag << endl;
	file << "0;JMP" << endl;
	// true
	file << "(" << trueFlag << ")" << endl;
	file << "@SP" << endl;
	file << "A=M" << endl;
	file << "M=-1" << endl;
	// end
	file << "(" << endFlag << ")" << endl;
	addStackPtr(file);
	file << "// end eq" << endl;
}

// x>y -> true
void gt(ofstream &file) {
	static int count = 0;
	string gt = "GT:";
	string trueFlag = gt + "TRUE" + to_string(count);
	string endFlag = gt + "END" + to_string(count);
	count++;
	file << "// start eq" << endl;
	checkLeftRightOperands(file);
	// D=x-y
	file << "@R13" << endl;
	file << "D=D-M" << endl;
	// x>y -> true
	file << "@" << trueFlag << endl;
	file << "D;JGT" << endl;
	// false
	file << "@SP" << endl;
	file << "A=M" << endl;
	file << "M=0" << endl;
	file << "@" << endFlag << endl;
	file << "0;JMP" << endl;
	// true
	file << "(" << trueFlag << ")" << endl;
	file << "@SP" << endl;
	file << "A=M" << endl;
	file << "M=-1" << endl;
	// end
	file << "(" << endFlag << ")" << endl;
	addStackPtr(file);
	file << "// end eq" << endl;
}

// x<y -> true
void lt(ofstream &file) {
	static int count = 0;
	string lt = "LT:";
	string trueFlag = lt + "TRUE" + to_string(count);
	string endFlag = lt + "END" + to_string(count);
	count++;
	file << "// start eq" << endl;
	checkLeftRightOperands(file);
	// D=x-y
	file << "@R13" << endl;
	file << "D=D-M" << endl;
	// x<y -> true
	file << "@" << trueFlag << endl;
	file << "D;JLT" << endl;
	// false
	file << "@SP" << endl;
	file << "A=M" << endl;
	file << "M=0" << endl;
	file << "@" << endFlag << endl;
	file << "0;JMP" << endl;
	// true
	file << "(" << trueFlag << ")" << endl;
	file << "@SP" << endl;
	file << "A=M" << endl;
	file << "M=-1" << endl;
	// end
	file << "(" << endFlag << ")" << endl;
	addStackPtr(file);
	file << "// end eq" << endl;
}

// x&y
void And(ofstream &file) {
	checkLeftRightOperands(file);
	// D=D&R[13]	D=x&y
	file << "@R13" << endl;
	file << "D=D&M" << endl;
	// R[SP]=x&y
	file << "@SP" << endl;
	file << "A=M" << endl;
	file << "M=D" << endl;
	// SP++
	file << "@SP" << endl;
	file << "M=M+1" << endl;
}

// x|y
void Or(ofstream &file) {
	checkLeftRightOperands(file);
	// D=D|R[13]	D=x|y
	file << "@R13" << endl;
	file << "D=D|M" << endl;
	// R[SP]=x|y
	file << "@SP" << endl;
	file << "A=M" << endl;
	file << "M=D" << endl;
	addStackPtr(file);
}

// !y
void Not(ofstream &file) {
	// SP--
	file << "@SP" << endl;
	file << "AM=M-1" << endl;
	// M[SP] = !M[SP]
	file << "M=!M" << endl;
	// SP++
	file << "@SP" << endl;
	file << "M=M+1" << endl;
}

void CodeWriter::writeArithmetic(string command) {
	if(command == "add") { add(file); }
	if(command == "sub") { sub(file); }
	if(command == "neg") { neg(file); }
	if(command == "eq") { eq(file); }
	if(command == "gt") { gt(file); }
	if(command == "lt") { lt(file); }
	if(command == "and") { And(file); }
	if(command == "or") { Or(file); }
	if(command == "not") { Not(file); }
}

void constantPush(ofstream &file, int index) {
	// M[SP] = index
	file << "@" << index << endl;
	file << "D=A" << endl;
	file << "@SP" << endl;
	file << "A=M" << endl;
	file << "M=D" << endl;
	// SP++
	file << "@SP" << endl;
	file << "M=M+1" << endl;
}

void CodeWriter::writePushPop(CommandType command, string segment, int index) {
	switch(command) {
		// push segment index
		// segment[index]をスタックの上にプッシュする
		case C_PUSH:
			if(segment == "constant")	constantPush(file, index);
			break;
		// pop segment index
		// スタックの一番上のデータをポップし、それをsegment[index]に格納する
		case C_POP:
			break;
		default:
			break;
	}
}

void CodeWriter::close() {
	file.close();
}
