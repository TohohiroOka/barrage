#pragma once
#include <fbxsdk.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "Texture/TextureManager.h"
#include <vector>
#include <unordered_map>
#include "Math/DirectXMathHelper.h"

class FbxModel
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public://固定値

	//ボーンの最大値
	static const int MAX_BONES = 128;

private://構造体宣言

	static const int MAX_BONE_INDICES = 4;

	//頂点データ3D
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWhight[MAX_BONE_INDICES];
	};

	struct VectexProcess {
		XMFLOAT3 pos;
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWhight[MAX_BONE_INDICES];
	};

	//マテリアル用
	struct Material
	{
		std::string name;//マテリアル名
		XMFLOAT3 ambient = {};//アンビエント影響度
		XMFLOAT3 diffuse = {};//ディフューズ影響度
		float alpha = 0;//アルファ
		XMFLOAT3 baseColor = { 1,1,1 };//ベースカラ―
		float metalness = 0.0f;//金属度(0 or 1)
		float specular = 0.5f;//鏡面反射度
		float roughness = 0.0f;//粗さ
	};

	//ボーン
	struct Bone
	{
		//名前
		std::string name;

		//初期姿勢行列
		XMMATRIX invInitialPose;

		//クラスター
		FbxCluster* fbxCluster;

		//コンストラクタ
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

	//ノード
	struct Node
	{
		//名前
		std::string name;
		//ローカルスケール
		DirectX::XMVECTOR scaling = { 1.0f,1.0f,1.0f,0.0f };
		//ローカル回転
		XMVECTOR rotation = { 0.0f,0.0f,0.0f,0.0f };
		//ローカル移動
		XMVECTOR translation = { 0.0f,0.0f,0.0f,1.0f };
		//ローカル変形行列
		XMMATRIX transform = {};
		//グローバル変形行列
		XMMATRIX globalTransform = {};
		//親ノード
		Node* parent = nullptr;
	};

	struct BuffData {
		Material material;
		std::vector<Vertex> vertices;
		std::vector<unsigned short>indices;
		std::vector<Bone> bones;
		//テクスチャパス
		std::string texName;
		//頂点バッファ
		ComPtr<ID3D12Resource> vertBuff = nullptr;
		//頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView;
		//インデックスバッファ
		ComPtr<ID3D12Resource> indexBuff = nullptr;
		//インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW ibView;
	};

	//スキン用定数バッファデータ
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	//アニメーションデータ
	struct FbxUpdate
	{
		bool isAnimation = false;//アニメーション可能か
		FbxTime startTime = {};//フレームのスタート
		FbxTime stopTime = {};//フレームの最後
		FbxTime nowTime = {};//現在の進行フレーム
		XMFLOAT3 startPos = {};//モデルの初期座標
	};

	//Fbxデータ
	struct Data
	{
		std::vector<Node> nodes;
		std::vector<BuffData> buffData;
		Node* meshNode;
		FbxScene* fbxScene = nullptr;
		std::vector<FbxUpdate> fbxUpdate;
	};

private://メンバ関数

	/// <summary>
	/// ノード読み込み
	/// </summary>
	/// <param name="parent">親ノード</param>
	void LoadNode(FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュを探す
	/// </summary>
	void CollectMesh(FbxNode* fbxNode);

	/// <summary>
	/// 頂点読み込み
	/// </summary>
	/// <param name="fbxMesh">メッシュ</param>
	void CollectVertices(FbxMesh* fbxMesh);

	/// <summary>
	/// 面ごとの読み込み
	/// </summary>
	/// <param name="fbxMesh">メッシュ</param>
	void CollectMeshFaces(FbxMesh* fbxMesh);

	/// <summary>
	/// スキニング情報の読み取り
	/// </summary>
	/// <param name="fbxMesh">メッシュ</param>
	void CollectSkin(FbxMesh* fbxMesh, std::vector<VectexProcess>& _vectexProcess);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(FbxNode* fbxNode);

	/// <summary>
	/// アニメーション読み込み
	/// </summary>
	void LoadAnimation(FbxScene* fbxScene, const int _animationNum);

	/// <summary>
	/// Fbxファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void LoadFbx(const std::string modelName);

	/// <summary>
	/// 生成
	/// </summary>
	void Initialize(const int _createNum);

	/// <summary>
	/// //行列の変換
	/// </summary>
	/// <param name="dst">格納するXMMATRIX型変数</param>
	/// <param name="src">変換するFbxMatrix型変数</param>
	void ConvertMatrixFormFbx(DirectX::XMMATRIX* dst, const FbxMatrix& src);

	/// <summary>
	/// //ファイル名抽出
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <returns></returns>
	std::string ExtractFileName(const std::string& path);

public://静的メンバ関数

	FbxModel() {};
	~FbxModel();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	static std::unique_ptr<FbxModel> Create(const std::string fileName, const std::string& _baseBoneName = "");

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

public:

	/// <summary>
	/// 更新
	/// </summary>
	bool Update(const int _animationNum = 0, const bool _isRoop = true);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="_motionBlend">blend用モデル</param>
	/// <param name="_rate1">現在のモデルの比率</param>
	/// <param name="_rate2">ブレンドするモデルの比率</param>
	bool Update(FbxModel* _motionBlend, const float _rate1, const float _rate2, const int _animationNum = 0, const bool _isRoop = true);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 
	/// </summary>
	void AnimationReset(const int _animationNum) {
		data->fbxUpdate[_animationNum].nowTime = data->fbxUpdate[_animationNum].startTime;
		beforePos = data->fbxUpdate[_animationNum].startPos;
	}

private://静的メンバ変数

	//デバイス
	static ID3D12Device* device;
	//Fbxの基盤
	static FbxManager* fbxManager;
	//FBXインポータ
	static FbxImporter* fbxImporter;
	//1フレームの時間
	static FbxTime frameTime;
	//ファイルパス
	static const std::string baseDirectory;
	//Fbxのデータ
	std::unique_ptr<Data> data = nullptr;

public://メンバ変数

	//モデル名
	std::string name;
	//テクスチャパス
	std::unordered_map<std::string,std::unique_ptr<TextureManager>> texture;
	//定数Texture
	ComPtr<ID3D12Resource> constBuffSkin = nullptr;
	//アニメーション可能か
	bool isAnimation = false;
	//スキニングを行うか
	bool isSkinning = true;
	//要素数
	int elementsNum;
	//motionblend用
	std::vector<ConstBufferDataSkin> skinData;
	//ボーン描画用の行列
	std::unordered_map<std::string, XMMATRIX> boneMatWorld;
	//基準ボーン
	std::string baseBoneName="";
	//１フレーム前の座標
	XMFLOAT3 beforePos;
	//FBXの動き
	XMFLOAT3 move;

public:

	XMMATRIX GetSkinData(const int _number, const int _bonesNumber) { return skinData[_number].bones[_bonesNumber]; }
	XMMATRIX GetBornMatWorld(const std::string _boneName) { return boneMatWorld[_boneName]; }
	XMFLOAT3 GetWorldMove(const int _animationNum) { return move; }

	///// <summary>
	///// アンビエント影響度の取得
	///// </summary>
	///// <returns>アンビエント</returns>
	//XMFLOAT3& GetAmbient() { return data->material.ambient; }

	///// <summary>
	///// ディフューズ影響度の取得
	///// </summary>
	///// <returns>ディフューズ</returns>
	//XMFLOAT3& GetDiffuse() { return data->material.diffuse; }

	///// <summary>
	///// アルファの取得
	///// </summary>
	///// <returns>アルファ</returns>
	//float& GetAlpha() { return data->material.alpha; }

	///// <summary>
	///// ベースカラ―の取得
	///// </summary>
	///// <returns>ベースカラ―</returns>
	//XMFLOAT3& GetBaseColor() { return data->material.baseColor; }

	///// <summary>
	///// 金属度の取得
	///// </summary>
	///// <returns>金属度(0 or 1)</returns>
	//float& GetMetalness() { return data->material.metalness; }

	///// <summary>
	///// 鏡面反射度の取得
	///// </summary>
	///// <returns>鏡面反射度</returns>
	//float& GetSpecular() { return data->material.specular; }

	///// <summary>
	///// 粗さの取得
	///// </summary>
	///// <returns>粗さ</returns>
	//float& GetRoughness() { return data->material.roughness; }
};