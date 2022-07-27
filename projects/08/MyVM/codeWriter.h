#include <fstream>
#include <string>

#include "parser.h"
using namespace std;

class CodeWriter {
	ofstream file;
	string fileNameWithoutExtension;
	string functionName;
	public:
	//出力ファイル/ストリームを開き書き込む準備を行う
	CodeWriter(string fileName);
	//VMの初期化(ブートストラップ)
	//出力ファイルの先頭に配置する
	void writeInit();
	//CodeWriterモジュールに新しいVMファイルの変換が開始した事を知らせる
	void setFileName(string fileName);
	//与えられた算術コマンドをアセンブリコードに変換し、それを書き込む
	void writeArithmetic(string command);
	//C_PUSHまたはC_POPコマンドをアセンブリコードに変換し、それを書き込む
	void writePushPop(CommandType command, string segment, int index);
	//labelコマンドを行うアセンブリコードを書く
	void writeLabel(string label);
	//gotoコマンドを行うアセンブリコードを書く
	void writeGoto(string label);
	//if-gotoコマンドを行うアセンブリコードを書く
	void writeIf(string label);
	//出力ファイルを閉じる
	void close();
};
