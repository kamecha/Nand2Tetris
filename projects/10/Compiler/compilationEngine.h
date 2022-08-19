#pragma once

#include "jackTokenizer.h"
#include <fstream>

class CompilationEngine {
	std::ofstream outFile;
	JackTokenizer jackTokenizer;
	public:
	// 与えられた入力と出力に対して新しいコンパイルエンジンを生成する。
	// 次に呼ぶルーチンはcompleClass()でなければならない
	CompilationEngine(std::ifstream inFile, std::ofstream outFile);
	// クラスをコンパイルする
	void compileClass();
	// スタティック宣言またはフィールド宣言をコンパイルする
	void compileClassVarDec();
	// メソッド、ファンクション、コンストラクタをコンパイルする
	void compileSubroutine();
	// パラメータのリスト(空の可能性もある)をコンパイルする。括弧()は含まない
	void compileParameterList();
	// サブルーチンのボディをコンパイルする
	void compileSubroutineBody();
	// var宣言をコンパイルする
	void compileVarDec();
	// keywordをコンパイルする
	void compileKeyword();
	// symbolをコンパイルする
	void compileSymbol();
	// 0~32767までの10進数の数字をコンパイルする
	void compileIntegerConstant();
	// ダブルクォートと改行文字を含まないユニコードの文字列をコンパイルする
	void compileStringConstnat();
	// identifierをコンパイルする
	void compileIdentifier();
	// 一連の文をコンパイルする。波カッコ{}は含まない
	void compileStatements();
	// do文をコンパイルする
	void compileDo();
	// let文をコンパイルする
	void compileLet();
	// while文をコンパイルする
	void compileWhile();
	// return文をコンパイルする
	void compileReturn();
	// if文をコンパイルする
	// else文を伴う可能性がある
	void compileIf();
	// 式をコンパイルする
	void compileExpression();
	// termをコンパイルする
	// 識別子の場合トークンの先読みをする必要がある
	void compileTerm();
	// 関数呼び出しをコンパイルする
	void compileSubroutineCall();
	// コンマで分離された式のリスト(空の可能性もある)をコンパイルする
	void compileExpressionList();
};
