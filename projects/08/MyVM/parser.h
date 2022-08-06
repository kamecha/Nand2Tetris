#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

enum CommandType {
	C_NULL,	// <- どの種類にも割当たらない
	C_ARITHEMETIC,
	C_PUSH,
	C_POP,
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL
};

class Parser {
	ifstream file;
	// 入力文字列
	vector<string> lines;
	//読み込んでいる文字列のindex
	int ptr;
	//現在のコマンド
	string command;
	//現在のコマンドの分割
	vector<string> commandSplit;
	public:
		//パースを行う準備
		Parser(string fileName);
		//一番最初のコマンドを見る様にする
		void reset();
		//入力にまだコマンドが存在するか
		bool hasMoreCommands();
		//入力から次のコマンドを読み、それを現在のコマンドにする
		//hasMoreCommandsがtrueのばあいのみ呼ばれる
		void advance();
		//現在のVMコマンドの種類を返す
		//算術コマンドはすべてC_ARITHEMETICが返される
		CommandType commandType();
		//現コマンドの最初の引数が返される
		//C_ARITHEMETICの場合、コマンド自体が返される
		//C_RETURNの場合、本ルーチンは呼ばないようにする
		string arg1();
		//現コマンドの二番目の引数が返される
		//現コマンドがC_PUSH,C_POP,C_FUNCTION,C_CALLの場合のみ本ルーチンを呼ぶようにする
		int arg2();
};

