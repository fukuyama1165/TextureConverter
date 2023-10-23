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

	//ファイルパスを変換
	std::wstring wfilePath = ConvertMultByteStringToWideString(filePath);

	//WICテクスチャのロード
	result = LoadFromWICFile(
		wfilePath.c_str(),
		DirectX::WIC_FLAGS_NONE,
		&metadata_,
		scratchImage_
	);

	assert(SUCCEEDED(result));

	//フォルダパスとファイル名を分割
	SeparateFilePath(wfilePath);

}

std::wstring TextureConverter::ConvertMultByteStringToWideString(const std::string& mstring)
{

	//文字列のサイズをワイド文字て取得
	int32_t wideStringSize = MultiByteToWideChar(CP_ACP, 0, mstring.c_str(), -1, nullptr, 0);

	//ワイド文字を作って大きさを変更
	std::wstring wstring;
	wstring.resize(wideStringSize);

	//ワイド文字に変換
	MultiByteToWideChar(CP_ACP, 0, mstring.c_str(), -1, &wstring[0], wideStringSize);

	return wstring;

}

void TextureConverter::SeparateFilePath(const std::wstring& filePath)
{

	size_t pos1;
	std::wstring exceptExt;

	//区切り文字'.'が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('.');

	//検索がヒットしたら
	if (pos1 != std::wstring::npos)
	{

		//区切り文字の後ろをファイル拡張子として保存
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);

		//区切り文字の前までを抜き出す
		exceptExt = filePath.substr(0, pos1);

	}
	else
	{

		fileExt_ = L"";
		exceptExt = filePath;

	}

	//区切り文字'\\'が出てくる最後の部分を検索
	pos1 = exceptExt.rfind('\\');

	if (pos1 != std::wstring::npos)
	{

		//区切り文字の後ろをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);

		//区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;

	}


	//区切り文字'/'が出てくる最後の部分を検索
	pos1 = exceptExt.rfind('/');

	if (pos1 != std::wstring::npos)
	{

		//区切り文字の後ろをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);

		//区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;

	}


	//区切り文字が無いのでファイル名のみとして扱うよ
	directoryPath_ = L"";
	fileName_ = exceptExt;

}

void TextureConverter::SaveDDStextureFile()
{

	HRESULT result;

	DirectX::ScratchImage mipChain{};
	//ミップマップの生成
	result = GenerateMipMaps(
		scratchImage_.GetImages(),
		scratchImage_.GetImageCount(),
		scratchImage_.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	if (SUCCEEDED(result))
	{
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}

	//圧縮するよ
	DirectX::ScratchImage converted;
	result = DirectX::Compress(
		scratchImage_.GetImages(),
		scratchImage_.GetImageCount(),
		metadata_,
		DXGI_FORMAT_BC7_UNORM_SRGB,
		DirectX::TEX_COMPRESS_BC7_QUICK | DirectX::TEX_COMPRESS_SRGB_OUT | DirectX::TEX_COMPRESS_PARALLEL,
		1.0f,
		converted
	);

	if (SUCCEEDED(result))
	{
		//やったー書き換え
		scratchImage_ = std::move(converted);
		metadata_ = scratchImage_.GetMetadata();
	}


	//読み込んだテクスチャをSRGBとして扱う
	metadata_.format = DirectX::MakeSRGB(metadata_.format);

	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	//DDSファイル書き出し
	result = DirectX::SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_, DirectX::DDS_FLAGS_NONE, filePath.c_str());
	
	assert(SUCCEEDED(result));

}