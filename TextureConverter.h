#pragma once
#include <string>
#include <DirectXTex.h>

class TextureConverter
{
public:

	/// <summary>
	/// テクスチャをWICからDDSに変換する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void ConvertTextureWICToDDS(const std::string filePath);

private:

	/// <summary>
	/// テクスチャファイル読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadWICTextureFromFile(const std::string& filePath);

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="mstring">マルチバイト文字</param>
	/// <returns>ワイド文字</returns>
	static std::wstring ConvertMultByteStringToWideString(const std::string& mstring);

	/// <summary>
	/// フォルダパスとファイル名を分離する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void SeparateFilePath(const std::wstring& filePath);

	/// <summary>
	/// DDSテクスチャとしてファイル書き出し
	/// </summary>
	void SaveDDStextureFile();

private:

	//画像の情報
	DirectX::TexMetadata metadata_{};

	//画像イメージのコンテナ
	DirectX::ScratchImage scratchImage_{};

	//ディレクトリパス
	std::wstring directoryPath_;

	//ファイル名
	std::wstring fileName_;

	//ファイル拡張子
	std::wstring fileExt_;


};

