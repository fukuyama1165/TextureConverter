#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include "TextureConverter.h"
#include <cassert>
#include <Winerror.h>
#include <combaseapi.h>

//コマンドライン引数
enum Argument
{
	kApplicationPath,//アプリケーションのパス
	kFilePath,//渡されたファイルのパス

	NumArgument
};

int32_t main(int32_t argc,char* argv[])
{
	
	assert(argc >= NumArgument);

	//COMライブラリの初期化
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));

	//テクスチャコンバータ
	TextureConverter converter;

	//テクスチャ変換
	converter.ConvertTextureWICToDDS(argv[kFilePath]);
	

	system("pause");
	return 0;
}