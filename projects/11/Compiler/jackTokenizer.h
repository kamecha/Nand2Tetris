#pragma once
#include <istream>
#include <string>
#include <vector>

enum TokenType {
	KEYWORD,
	SYMBOL,
	IDENTIFIER,
	INT_CONST,
	STRING_CONST,
};

enum KeyWord {
	KEY_CLASS,
	KEY_METHOD,
	KEY_FUNCTION,
	KEY_CONSTRUCTOR,
	KEY_INT,
	KEY_BOOLEAN,
	KEY_CHAR,
	KEY_VOID,
	KEY_VAR,
	KEY_STATIC,
	KEY_FIELD,
	KEY_LET,
	KEY_DO,
	KEY_IF,
	KEY_ELSE,
	KEY_WHILE,
	KEY_RETURN,
	KEY_TRUE,
	KEY_FALSE,
	KEY_NULL,
	KEY_THIS
};

class JackTokenizer {
	// 現在のトークンへのindex
	int tokenIndex;
	// トークン配列
	std::vector<std::string> tokens;
	public:
	// 入力ファイル/ストリームを開き、トークン化を行う準備をする
	JackTokenizer(std::ifstream& file);
	// 入力にまだトークンは存在するか？
	bool hasMoreTokens();
	// 入力から次のトークンを取得し、それを現在のトークンとする
	// このルーチンはhasMoreTokensがtrueの場合のみ呼び出すことができる
	// 最初は現トークンは設定されていない
	void advance();
	// 次のトークンを取得し、返す
	// 現在のトークンは変化しない
	std::string next();
	// 現トークンの種類を返す
	TokenType tokenType();
	// 現トークンのキーワードを返す
	// tokenTypeがKEYWORDの場合のみ呼び出すことができる
	KeyWord keyWord();
	// 現トークンの文字を返す
	// tokenTypeがSYMBOLの場合のみ呼び出すことができる
	char symbol();
	// 現トークンの識別子を返す
	// tokenTypeがIDENTIFIERの場合のみ呼び出すことができる
	std::string identifier();
	// 現トークンの整数の値を返す
	// tokenTypeがINT_CONSTの場合のみ呼び出すことができる
	int intVal();
	// 現トークンの文字列を返す
	// tokenTypeがSTRING_CONSTの場合のみ呼び出すことができる
	std::string stringVal();
};
