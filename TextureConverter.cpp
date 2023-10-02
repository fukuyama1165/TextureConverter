#include "TextureConverter.h"
#include <stringapiset.h>

void TextureConverter::ConvertTextureWICToDDS(const std::string filePath)
{

	LoadWICTextureFromFile(filePath);

	SaveDDStextureFile();

}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{

	HRESULT result;

	//�t�@�C���p�X��ϊ�
	std::wstring wfilePath = ConvertMultByteStringToWideString(filePath);

	//WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		wfilePath.c_str(),
		DirectX::WIC_FLAGS_NONE,
		&metadata_,
		scratchImage_
	);

	assert(SUCCEEDED(result));

	//�t�H���_�p�X�ƃt�@�C�����𕪊�
	SeparateFilePath(wfilePath);

}

std::wstring TextureConverter::ConvertMultByteStringToWideString(const std::string& mstring)
{

	//������̃T�C�Y�����C�h�����Ď擾
	int32_t wideStringSize = MultiByteToWideChar(CP_ACP, 0, mstring.c_str(), -1, nullptr, 0);

	//���C�h����������đ傫����ύX
	std::wstring wstring;
	wstring.resize(wideStringSize);

	//���C�h�����ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, mstring.c_str(), -1, &wstring[0], wideStringSize);

	return wstring;

}

void TextureConverter::SeparateFilePath(const std::wstring& filePath)
{

	size_t pos1;
	std::wstring exceptExt;

	//��؂蕶��'.'���o�Ă����ԍŌ�̕���������
	pos1 = filePath.rfind('.');

	//�������q�b�g������
	if (pos1 != std::wstring::npos)
	{

		//��؂蕶���̌����t�@�C���g���q�Ƃ��ĕۑ�
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);

		//��؂蕶���̑O�܂ł𔲂��o��
		exceptExt = filePath.substr(0, pos1);

	}
	else
	{

		fileExt_ = L"";
		exceptExt = filePath;

	}

	//��؂蕶��'\\'���o�Ă���Ō�̕���������
	pos1 = exceptExt.rfind('\\');

	if (pos1 != std::wstring::npos)
	{

		//��؂蕶���̌����f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath_ = exceptExt.substr(0, pos1 + 1);

		//��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;

	}


	//��؂蕶��'/'���o�Ă���Ō�̕���������
	pos1 = exceptExt.rfind('/');

	if (pos1 != std::wstring::npos)
	{

		//��؂蕶���̌����f�B���N�g���p�X�Ƃ��ĕۑ�
		directoryPath_ = exceptExt.substr(0, pos1 + 1);

		//��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;

	}


	//��؂蕶���������̂Ńt�@�C�����݂̂Ƃ��Ĉ�����
	directoryPath_ = L"";
	fileName_ = exceptExt;

}

void TextureConverter::SaveDDStextureFile()
{

	//�ǂݍ��񂾃e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata_.format = DirectX::MakeSRGB(metadata_.format);

	HRESULT result;

	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	//DDS�t�@�C�������o��
	result = DirectX::SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_, DirectX::DDS_FLAGS_NONE, filePath.c_str());
	
	assert(SUCCEEDED(result));

}