#include <string>
using namespace std;

class Code {
	public:
	//dest ニーモニックのバイナリコード(3bit)を返す
	string dest(string mnemonic);
	//comp ニーモニックのバイナリコード(7bit)を返す
	string comp(string mnemonic);
	//jump ニーモニックのバイナリコード(3bit)を返す
	string jump(string mnemonic);
};
