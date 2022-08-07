#include "jackTokenizer.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

// 削除するのは行コメントのみ
std::string omitComment(std::string line) {
	// 任意の空白 + // + 任意の文字列
	return std::regex_replace(line, std::regex(R"(\s*//.*$)"), "");
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

const std::string symbol[] = {
	"{", "}", "(", ")", "[", "]", ".", ",", ";","+", "-", "*", "/", "&", "|", "<", ">", "=", "~"
};

JackTokenizer::JackTokenizer(std::ifstream file) {
	// 入力ファイル1行分
	std::string line;
	while(std::getline(file, line)) {
		// キャリッジリターン(\n\r)対策
		if(!line.empty() && line.back() == '\r') {
			line.pop_back();
		}
		line = omitComment(line);
		// 空行無視
		if(line.empty())	continue;
		line = omitSpace(line);
		std::vector<std::string> temp = split(line);
		// 空白がない文字列からトークンを抽出
		for(std::string beforeToken: temp) {
		}
		debugToken(tokens);
	}
}
