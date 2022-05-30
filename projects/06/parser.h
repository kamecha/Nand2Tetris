#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

enum CommandType {
	//@Xxx Xxx=symbol or number
	A_COMMAND,
	//dest=comp;jump
	C_COMMAND,
	//Xxx symbol
	L_COMMAND
};

class Parser {
	ifstream file;
	// 入力文字列
	vector<string> lines;
	//読み込んでいる文字列のindex
	int ptr;
	//現在のコマンド
	string command;
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
		//現在のコマンドの種類を返す
		CommandType commandType();
		//@Xxx or (Xxx)のXxxを返す
		//A_COMMAND or L_COMMAND の時呼ばれる
		string symbol();
		//C instructionのdestニーモニックを返す
		//C_COMMAND の時のみ呼ばれる
		string dest();
		//C instruction のcompニーモニックを返す
		//C_COMMAND の時のみ呼ばれる
		string comp();
		//C instruction のjumpニーモニックを返す
		//C_COMMAND の時のみ呼ばれる
		string jump();
};

