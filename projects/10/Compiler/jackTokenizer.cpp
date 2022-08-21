#include "jackTokenizer.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

// 削除するのは行コメントのみ
std::string omitComment(std::string line) {
	std::string tmp;
	// 任意の空白 + // + 任意の文字列
	tmp = std::regex_replace(line, std::regex(R"(\s*//.*$)"), "");
	// 任意の空白 + /** + 任意の文字列 + */
	tmp = std::regex_replace(tmp, std::regex(R"(\s*/\*\*.*\*/)"), "");
	// 任意の空白 + /* + 任意の文字列 + */
	tmp = std::regex_replace(tmp, std::regex(R"(\s*/\*.*\*/)"), "");
	return tmp;
}

// 余分な空白の削除
std::string omitSpace(std::string line) {
	std::string tmp;
	// 先頭の空白の削除
	// 	"hoge  oi    ppp   "
	tmp = std::regex_replace(line, std::regex(R"(^\s*)"), "");
	// 行末の空白の削除
	// 	"hoge  oi	 ppp"
	tmp = std::regex_replace(line, std::regex(R"(\s*$)"), "");
	// 中間の空白の削除
	// 	"hoge oi ppp"
	tmp = std::regex_replace(line, std::regex(R"(\s+)"), " ");
	return tmp;
}

// 空白ごとにsplitして配列におしこむ
std::vector<std::string> split(const std::string& line) {
	static bool isComment = false;
	const char* delim = " ";
	std::vector<std::string> ret;
	std::string::size_type len = line.length();
	std::string token;
	for(std::string::size_type i = 0, n; i < len; i = n + 1) {
		n = line.find_first_of(delim, i);
		if(n == std::string::npos) {
			n = len;
		}
		token = line.substr(i, n-i);
		if(!token.empty())
			ret.push_back(token);
	}
	return ret;
}

void debugToken(const std::vector<std::string> tokens) {
	for(std::string token: tokens) {
		std::cout << token << std::endl;
	}
}

const std::vector<std::string> symbols = {
	"{", "}", "(", ")", "[", "]", ".", ",", ";","+", "-", "*", "/", "&", "|", "<", ">", "=", "~"
};

bool checkCommentStart(std::string token) {
	if(token == "/**" || token == "/*")
		return true;
	return false;
}

bool checkCommentEnd(std::string token) {
	if(token == "*/")
		return true;
	return false;
}

/** 
	int x, y;
	int x , y ;
	上記のように分離する
*/
void separateSymbol(std::string beforeToken, std::vector<std::string>& tokens) {
	// symbolの配列を文字列に変換
	std::string symbol = [&]() -> std::string {
		std::string ret;
		for(std::string symbol: symbols)	ret += symbol;
		return ret;
	}();
	// int x,y;
	// 上を x , y ; に分ける
	for(std::string::size_type ptr = 0; ptr != std::string::npos; ) {
		std::string::size_type foundPtr = beforeToken.find_first_of(symbol, ptr);
		std::string token;
		if(foundPtr == std::string::npos) {
			token = beforeToken.substr(ptr, beforeToken.size() - ptr);
			if(!token.empty()) {
				// std::cout << "push token:\t" << token << std::endl;
				tokens.push_back(token);
			}
			break;
		}
		// symbolじゃない
		if(ptr != foundPtr)
			token = beforeToken.substr(ptr, foundPtr - ptr);
		if(!token.empty()) {
			// std::cout << "push token:\t" << token << std::endl;
			tokens.push_back(token);
		}
		// symbol
		token = beforeToken.substr(foundPtr, 1);
		if(!token.empty()) {
			// std::cout << "push token:\t" << token << std::endl;
			tokens.push_back(token);
		}
		ptr = foundPtr + 1;
	}
}

JackTokenizer::JackTokenizer(std::ifstream& file) {
	// 入力ファイル1行分
	std::string line;
	// コメントかどうか
	bool isComment = false;
	// 文字列かどうか
	bool isString = false;
	while(std::getline(file, line)) {
		// std::cout << "これを今からトークン化していくよ" << std::endl;
		// std::cout << line << std::endl;

		// キャリッジリターン(\n\r)対策
		if(!line.empty() && line.back() == '\r') {
			line.pop_back();
		}
		if(!line.empty() && line.back() == '\n') {
			line.pop_back();
		}
		if(line.empty())	continue;
		std::smatch match;
		std::string beforeToken;
		auto start = line.cbegin();
		// std::cout << "条件判定をする" << std::endl;
		// std::cout << std::regex_search(start, line.cend(), match, std::regex(R"((\S+)|(\*.*\*))")) << std::endl;
		while(std::regex_search(start, line.cend(), match, std::regex(R"(".*"|[^\s"]+)"))) {
			beforeToken = match.str();
			// std::cout << "token:\t"	<< beforeToken << std::endl;
			if(beforeToken == "//")	break;
			if(checkCommentStart(beforeToken)) {
				isComment = true;
				start = match[0].second;
				continue;
			}
			if(checkCommentEnd(beforeToken)) {
				isComment = false;
				start = match[0].second;
				continue;
			}
			if(!isComment) {
				std::string::size_type firstStringPtr = beforeToken.find_first_of('"');
				if(firstStringPtr == std::string::npos) {
					separateSymbol(beforeToken, tokens);
				} else {
					tokens.push_back(beforeToken);
				}
			}
			start = match[0].second;
		}
	}
	tokenIndex = -1;
	// std::cout << "############################### トークナイザの確認 ###############################" << std::endl;
	for(std::string token: tokens) {
		// std::cout << token << std::endl;
	}
}

bool JackTokenizer::hasMoreTokens() {
	return tokenIndex + 1 < tokens.size();
}

void JackTokenizer::advance() {
	tokenIndex++;
}

std::string JackTokenizer::next() {
	return tokens[tokenIndex + 1];
}

const std::vector<std::string> keywords = {
	"class", "constructor", "function", "method", "field", "static", "var", "int", "char", "boolean", "void", "true", "false", "null", "this", "let", "do", "if", "else", "while", "return"
};

TokenType JackTokenizer::tokenType() {
	std::string token = tokens[tokenIndex];
	auto check = [](std::string token, const std::vector<std::string>& words) -> bool {
		return std::find(words.begin(), words.end(), token) != words.end();
	};
	if(check(token, keywords))	return KEYWORD;
	if(check(token, symbols))	return SYMBOL;
	if(std::all_of(token.cbegin(), token.cend(), isdigit))	return INT_CONST;
	if(token.front() == '"' && token.back() == '"')	return STRING_CONST;
	return IDENTIFIER;
}

KeyWord JackTokenizer::keyWord() {
	std::string token = tokens[tokenIndex];
	if(token == "class")	return KEY_CLASS;
	if(token == "constructor")	return KEY_CONSTRUCTOR;
	if(token == "function")	return KEY_FUNCTION;
	if(token == "method")	return KEY_METHOD;
	if(token == "field")	return KEY_FIELD;
	if(token == "static")	return KEY_STATIC;
	if(token == "var")		return KEY_VAR;
	if(token == "int")		return KEY_INT;
	if(token == "char")		return KEY_CHAR;
	if(token == "boolean")	return KEY_BOOLEAN;
	if(token == "void")		return KEY_VOID;
	if(token == "true")		return KEY_TRUE;
	if(token == "false")	return KEY_FALSE;
	if(token == "null")		return KEY_NULL;
	if(token == "this")		return KEY_THIS;
	if(token == "let")		return KEY_LET;
	if(token == "do")		return KEY_DO;
	if(token == "if")		return KEY_IF;
	if(token == "else")		return KEY_ELSE;
	if(token == "while")	return KEY_WHILE;
	if(token == "return")	return KEY_RETURN;
}

char JackTokenizer::symbol() {
	std::string symbol = tokens[tokenIndex];
	return symbol.front();
}

std::string JackTokenizer::identifier() {
	std::string identifier = tokens[tokenIndex];
	return identifier;
}

int JackTokenizer::intVal() {
	std::string intTmp = tokens[tokenIndex];
	std::cout << "これをintにしようとしてエラーが出たよ" << ">" << intTmp << "<" << std::endl;
	return std::stoi(intTmp);
}

std::string JackTokenizer::stringVal() {
	std::string stringTmp = tokens[tokenIndex];
	stringTmp.erase(stringTmp.begin());
	stringTmp.pop_back();
	return stringTmp;
}
