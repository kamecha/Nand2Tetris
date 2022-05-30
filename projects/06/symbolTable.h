#include <map>
#include <string>
using namespace std;

class SymbolTable {
	map<string, int> symbolTable;
	public:
		//空のシンボルテーブルを作成する
		SymbolTable();
		//テーブルに(symbol, adress)のペアを追加
		void addEntry(string symbol, int address);
		//シンボルテーブルは与えられたsymbolを含むか？
		//含む-> ture,	含まない->false
		bool contains(string symbol);
		//symbolに結び付けられたアドレスを返す
		int getAddress(string symbol);
};
