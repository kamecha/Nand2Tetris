#include <fstream>
#include <ostream>
#include <string>

#include "codeWriter.h"
using namespace std;

CodeWriter::CodeWriter(string fileName) { file.open(fileName, ios::out); }

void CodeWriter::setFileName(string fileName) {
  CodeWriter::close();
  file.open(fileName, ios::out);
}

void CodeWriter::writeInit() {
  //SP=256(0x0100)
  file << "@256" << endl;
  file << "D=A" << endl;
  file << "@SP" << endl;
  file << "M=D" << endl;
  //LCL=-1
  file << "@1" << endl;
  file << "D=-A" << endl;
  file << "@LCL" << endl;
  file << "M=D" << endl;
  //ARG=-2
  file << "@2" << endl;
  file << "D=-A" << endl;
  file << "@ARG" << endl;
  file << "M=D" << endl;
  //THIS=-3
  file << "@3" << endl;
  file << "D=-A" << endl;
  file << "@THIS" << endl;
  file << "M=D" << endl;
  //THAT=-4
  file << "@4" << endl;
  file << "D=-A" << endl;
  file << "@THAT" << endl;
  file << "M=D" << endl;
  // call Sys.init
  writeCall("Sys.init", 0);
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
  file << "// start lt" << endl;
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
  file << "// end lt" << endl;
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
  if (command == "add") {
    add(file);
  }
  if (command == "sub") {
    sub(file);
  }
  if (command == "neg") {
    neg(file);
  }
  if (command == "eq") {
    eq(file);
  }
  if (command == "gt") {
    gt(file);
  }
  if (command == "lt") {
    lt(file);
  }
  if (command == "and") {
    And(file);
  }
  if (command == "or") {
    Or(file);
  }
  if (command == "not") {
    Not(file);
  }
}

// M[SP] = D
void pushToGlobalStack(ofstream &file) {
  file << "@SP" << endl;
  file << "A=M" << endl;
  file << "M=D" << endl;
}

void constantPush(ofstream &file, int index) {
  // M[SP] = index
  file << "@" << index << endl;
  file << "D=A" << endl;
  pushToGlobalStack(file);
  // SP++
  addStackPtr(file);
}

void localPush(ofstream &file, int index) {
  // local + index
  file << "@LCL" << endl;
  file << "D=M" << endl;
  file << "@" << index << endl;
  file << "A=D+A" << endl;
  // D = M[local+index]
  file << "D=M" << endl;
  pushToGlobalStack(file);
  // SP++
  addStackPtr(file);
}

void argumentPush(ofstream &file, int index) {
  // argument + index
  file << "@ARG" << endl;
  file << "D=M" << endl;
  file << "@" << index << endl;
  file << "A=D+A" << endl;
  // D = M[argument+index]
  file << "D=M" << endl;
  pushToGlobalStack(file);
  // SP++
  addStackPtr(file);
}

void thisPush(ofstream &file, int index) {
  // this + index
  file << "@THIS" << endl;
  file << "D=M" << endl;
  file << "@" << index << endl;
  file << "A=D+A" << endl;
  // D = M[this+index]
  file << "D=M" << endl;
  pushToGlobalStack(file);
  // SP++
  addStackPtr(file);
}

void thatPush(ofstream &file, int index) {
  // that + index
  file << "@THAT" << endl;
  file << "D=M" << endl;
  file << "@" << index << endl;
  file << "A=D+A" << endl;
  // D = M[that+index]
  file << "D=M" << endl;
  pushToGlobalStack(file);
  // SP++
  addStackPtr(file);
}

// temp, index → @R(5+index)
void tempPush(ofstream &file, int index) {
  // temp + index
  file << "@R5" << endl;
  file << "D=A" << endl;
  file << "@" << index << endl;
  file << "A=D+A" << endl;
  // D = M[R(5+index)]
  file << "D=M" << endl;
  pushToGlobalStack(file);
  // SP++
  addStackPtr(file);
}

// M[SP] = M[3 + index]
void pointerPush(ofstream &file, int index) {
  // 3 + index
  file << "@3" << endl;
  file << "D=A" << endl;
  file << "@" << index << endl;
  file << "A=D+A" << endl;
  // D = M[3 + index]
  file << "D=M" << endl;
  pushToGlobalStack(file);
  // SP++
  addStackPtr(file);
}

// M[SP] = M[@Xxx.index]
void staticPush(ofstream &file, int index, string fileNameWithoutExtension) {
  // @Xxx.index
  file << "@" << fileNameWithoutExtension << "." << index << endl;
  file << "D=M" << endl;
  pushToGlobalStack(file);
  addStackPtr(file);
}

// M[@R14]にSPの値を代入する
void popGlobalStackToDestination(ofstream &file) {
  // SP--
  file << "@SP" << endl;
  file << "AM=M-1" << endl;
  // D=M[SP]
  file << "D=M" << endl;
  // M[R13]=D
  file << "@R13" << endl;
  file << "M=D" << endl;
  // M[local+index]=M[SP]
  file << "@R13" << endl;
  file << "D=M" << endl;
  file << "@R14" << endl;
  file << "A=M" << endl;
  file << "M=D" << endl;
}

void constantPop(ofstream &file, int index) {
  // SP--
  file << "@SP" << endl;
  file << "AM=M-1" << endl;
  // D=M[SP]
  file << "D=M" << endl;
  // M[index]=D
  file << "@" << index << endl;
  file << "M=D" << endl;
}

void localPop(ofstream &file, int index) {
  // D=local+index
  file << "@LCL" << endl;
  file << "D=M" << endl;
  file << "@" << index << endl;
  file << "D=D+A" << endl;
  // M[R14]=D
  file << "@R14" << endl;
  file << "M=D" << endl;
  popGlobalStackToDestination(file);
}

void argumentPop(ofstream &file, int index) {
  // D=argument+index
  file << "@ARG" << endl;
  file << "D=M" << endl;
  file << "@" << index << endl;
  file << "D=D+A" << endl;
  // M[R14]=D
  file << "@R14" << endl;
  file << "M=D" << endl;
  popGlobalStackToDestination(file);
}

void thisPop(ofstream &file, int index) {
  // D=this+index
  file << "@THIS" << endl;
  file << "D=M" << endl;
  file << "@" << index << endl;
  file << "D=D+A" << endl;
  // M[R14]=D
  file << "@R14" << endl;
  file << "M=D" << endl;
  popGlobalStackToDestination(file);
}

void thatPop(ofstream &file, int index) {
  // D=that+index
  file << "@THAT" << endl;
  file << "D=M" << endl;
  file << "@" << index << endl;
  file << "D=D+A" << endl;
  // M[R14]=D
  file << "@R14" << endl;
  file << "M=D" << endl;
  popGlobalStackToDestination(file);
}

void tempPop(ofstream &file, int index) {
  // D=R(5+index)
  file << "@R5" << endl;
  file << "D=A" << endl;
  file << "@" << index << endl;
  file << "D=D+A" << endl;
  // M[R14]=D
  file << "@R14" << endl;
  file << "M=D" << endl;
  popGlobalStackToDestination(file);
}

void pointerPop(ofstream &file, int index) {
  // D=R(3+index)
  file << "@3" << endl;
  file << "D=A" << endl;
  file << "@" << index << endl;
  file << "D=D+A" << endl;
  // M[R14]=D
  file << "@R14" << endl;
  file << "M=D" << endl;
  popGlobalStackToDestination(file);
}

void staticPop(ofstream &file, int index, string fileNameWithoutExtension) {
  // D=@Xxx.index
  file << "@" << fileNameWithoutExtension << "." << index << endl;
  file << "D=A" << endl;
  // M[R14]=D
  file << "@R14" << endl;
  file << "M=D" << endl;
  popGlobalStackToDestination(file);
}

void CodeWriter::writePushPop(CommandType command, string segment, int index) {
  switch (command) {
  // push segment index
  // segment[index]をスタックの上にプッシュする
  case C_PUSH:
    if (segment == "constant")
      constantPush(file, index);
    if (segment == "local")
      localPush(file, index);
    if (segment == "argument")
      argumentPush(file, index);
    if (segment == "this")
      thisPush(file, index);
    if (segment == "that")
      thatPush(file, index);
    if (segment == "temp")
      tempPush(file, index);
    if (segment == "pointer")
      pointerPush(file, index);
    if (segment == "static")
      staticPush(file, index, fileNameWithoutExtension);
    break;
  // pop segment index
  // スタックの一番上のデータをポップし、それをsegment[index]に格納する
  case C_POP:
    if (segment == "constant")
      constantPop(file, index);
    if (segment == "local")
      localPop(file, index);
    if (segment == "argument")
      argumentPop(file, index);
    if (segment == "this")
      thisPop(file, index);
    if (segment == "that")
      thatPop(file, index);
    if (segment == "temp")
      tempPop(file, index);
    if (segment == "pointer")
      pointerPop(file, index);
    if (segment == "static")
      staticPop(file, index, fileNameWithoutExtension);
    break;
  default:
    break;
  }
}

// 関数名$ラベル名のグローバルラベルを設定する
void CodeWriter::writeLabel(string label) {
  string LastLabel = functionName + "$" + label;
  file << "(" << LastLabel << ")" << endl;
}

void CodeWriter::writeGoto(string label) {
  string LastLabel = functionName + "$" + label;
  file << "@" << LastLabel << endl;
  file << "0;JMP" << endl;
}

void CodeWriter::writeFunction(string functionName, int numLocals) {
	this->functionName = functionName;
	// functionのやつ書いておく
	file << "(" << functionName << ")" << endl;
	// numLocalsの分だけローカル変数を0にセットしておく
	for(int i = 0; i < numLocals; i++) {
		constantPush(file, 0);
	}
}

// push return-address
// push LCL
// push ARG
// push THIS
// push THAT
// ARG = SP-n-5
// LCL = SP
// goto f
// (return-address)
void CodeWriter::writeCall(string functionName, int numArgs) {
	static int num = 0;
	//後で設定しておく
	string returnAddress = functionName + "." + to_string(num++);
	// push return-address
	file << "@" << returnAddress << endl;
	file << "D=A" << endl;
	pushToGlobalStack(file);
	addStackPtr(file);
	// push LCL
	file << "@LCL" << endl;
	file << "D=M" << endl;
	pushToGlobalStack(file);
	addStackPtr(file);
	// push ARG
	file << "@ARG" << endl;
	file << "D=M" << endl;
	pushToGlobalStack(file);
	addStackPtr(file);
	// push THIS
	file << "@THIS" << endl;
	file << "D=M" << endl;
	pushToGlobalStack(file);
	addStackPtr(file);
	// push THAT
	file << "@THAT" << endl;
	file << "D=M" << endl;
	pushToGlobalStack(file);
	addStackPtr(file);
	// ARG = SP-n-5
	file << "@SP" << endl;
	file << "D=M" << endl;
	for(int i = 0; i < numArgs+5; i++) {
		file << "D=D-1" << endl;
	}
	file << "@ARG" << endl;
	file << "M=D" << endl;
	// LCL = SP
	file << "@SP" << endl;
	file << "D=M" << endl;
	file << "@LCL" << endl;
	file << "M=D" << endl;
	// goto f
	file << "@" << functionName << endl;
	file << "0;JMP" << endl;
	// (return-address)
	file << "(" << returnAddress << ")" << endl;
}

// FRAME = LCL	FRAME:一時変数
// RET = * (FRAME-5)
// *ARG = pop()
// SP = ARG+1
// THAT = * (FRAME-1)
// THIS = * (FRAME-2)
// ARG = * (FRAME-3)
// LCL = * (FRAME-4)
// goto RET
void CodeWriter::writeReturn() {
	// FRAME = LCL
	file << "@LCL" << endl;
	file << "D=M" << endl;
	file << "@FRAME" << endl;
	file << "M=D" << endl;
	// RET = * (FRAME - 5)
	file << "@FRAME" << endl;
	file << "D=M" << endl;
	file << "@5" << endl;
	file << "A=D-A" << endl;
	file << "D=M" << endl;
	file << "@RET" << endl;
	file << "M=D" << endl;
	// *ARG = pop()
	file << "@ARG" << endl;
	file << "D=M" << endl;
	file << "@R14" << endl;
	file << "M=D" << endl;
	popGlobalStackToDestination(file);
	// SP = ARG + 1
	file << "@ARG" << endl;
	file << "D=M+1" << endl;
	file << "@SP" << endl;
	file << "M=D" << endl;
	// THAT = * (FRAME - 1)
	file << "@FRAME" << endl;
	file << "AM=M-1" << endl;
	file << "D=M" << endl;
	file << "@THAT" << endl;
	file << "M=D" << endl;
	// THIS = * (FRAME - 2)
	file << "@FRAME" << endl;
	file << "AM=M-1" << endl;
	file << "D=M" << endl;
	file << "@THIS" << endl;
	file << "M=D" << endl;
	// ARG = * (FRAME - 3)
	file << "@FRAME" << endl;
	file << "AM=M-1" << endl;
	file << "D=M" << endl;
	file << "@ARG" << endl;
	file << "M=D" << endl;
	// LCL = * (FRAME - 4)
	file << "@FRAME" << endl;
	file << "AM=M-1" << endl;
	file << "D=M" << endl;
	file << "@LCL" << endl;
	file << "M=D" << endl;
	// goto RET
	file << "@RET" << endl;
	file << "A=M" << endl;
	file << "0;JMP" << endl;
}

// スタックの最上位の値をポップし0でないならばラベルへ移動する
// 移動先は同じ関数内に限られる
void CodeWriter::writeIf(string label) {
  string LastLabel = functionName + "$" + label;
  // SP--
  file << "@SP" << endl;
  file << "AM=M-1" << endl;
  // D=M[SP]
  file << "D=M" << endl;
  file << "@" << LastLabel << endl;
  // JNE = if out != 0 jump
  file << "D;JNE" << endl;
}

void CodeWriter::close() { file.close(); }
