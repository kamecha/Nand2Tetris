#include <fstream>
#include <string>

#include "parser.h"
using namespace std;

class CodeWriter {
	ofstream file;
	string fileNameWithoutExtension;
	public:
	//出力ファイル/ストリームを開き書き込む準備を行う
	CodeWriter(string fileName);
	//CodeWriterモジュールに新しいVMファイルの変換が開始した事を知らせる
	void setFileName(string fileName);
	//与えられた算術コマンドをアセンブリコードに変換し、それを書き込む
	void writeArithmetic(string command);
	//C_PUSHまたはC_POPコマンドをアセンブリコードに変換し、それを書き込む
	void writePushPop(CommandType command, string segment, int index);
	//出力ファイルを閉じる
	void close();
};
