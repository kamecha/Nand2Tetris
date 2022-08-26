#pragma once

#include <string>
#include <map>

// 属性
enum Kind {
	STATIC,
	FIELD,
	ARG,
	VAR,
	NONE
};

// スコープ
enum Scope {
	CLASS,
	SUBROUTINE
};

struct SymbolInformation {
	// 型
	std::string type;
	// 属性
	Kind kind;
	// 番号
	int number;
};

class SymbolTable {
	Scope scope;
	std::map<std::string, SymbolInformation> classMap;
	std::map<std::string, SymbolInformation> subroutineMap;
	int staticCount, fieldCount;
	public:
	// 空のシンボルテーブルを生成する
	SymbolTable();
	// 新しいサブルーチンのスコープを開始する
	// サブルーチンのシンボルテーブルをリセットする
	void startSubroutine();
	// 引数の名前、型、属性で指定された新しい識別子を定義し、それに実行インデックスを割り当てる
	// STATICとFIELD属性の識別子はクラスのスコープを持ち、ARGとVAR属性の識別子はサブルーチンのスコープを持つ
	void define(std::string name, std::string type, Kind kind);
	// 引数で与えられた属性について、それが現在のスコープで定義されている数を返す
	int varCount(Kind kind);
	// 引数で与えられた名前の識別子を現在のスコープで探し、その属性を返す
	// その識別子が現在のスコープで見つからなければ、NONEを返す
	Kind kindOf(std::string name);
	// 引数で与えられた名前の識別子を現在のスコープで探し、その型を返す
	std::string typeOf(std::string name);
	// 引数で与えられた名前の識別子を現在のスコープで探し、そのインデックスを返す
	int indexOf(std::string name);
}

