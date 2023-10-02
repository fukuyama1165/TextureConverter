#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include "TextureConverter.h"
#include <cassert>
#include <Winerror.h>
#include <combaseapi.h>

//�R�}���h���C������
enum Argument
{
	kApplicationPath,//�A�v���P�[�V�����̃p�X
	kFilePath,//�n���ꂽ�t�@�C���̃p�X

	NumArgument
};

int32_t main(int32_t argc,char* argv[])
{
	
	assert(argc >= NumArgument);

	//COM���C�u�����̏�����
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));

	//�e�N�X�`���R���o�[�^
	TextureConverter converter;

	//�e�N�X�`���ϊ�
	converter.ConvertTextureWICToDDS(argv[kFilePath]);
	

	system("pause");
	return 0;
}