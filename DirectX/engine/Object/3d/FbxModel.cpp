#include "FbxModel.h"
#include <DirectXTex.h>
#include <string>
#include "GameHelper.h"

using namespace Microsoft::WRL;
using namespace DirectX;

ID3D12Device* FbxModel::device = nullptr;
FbxManager* FbxModel::fbxManager = nullptr;
FbxImporter* FbxModel::fbxImporter = nullptr;
FbxTime FbxModel::frameTime;
const std::string FbxModel::baseDirectory = "Resources/Fbx/";

FbxModel::~FbxModel()
{
	constBuffSkin.Reset();
	for (auto& i : data->buffData) {
		i.vertBuff.Reset();
		i.indexBuff.Reset();
	}
}

void FbxModel::StaticInitialize(ID3D12Device* device)
{
	HRESULT result = S_FALSE;

	// 再初期化チェック
	assert(!FbxModel::device);

	// nullptrチェック
	assert(device);

	FbxModel::device = device;

	fbxManager = FbxManager::Create();
	fbxImporter = FbxImporter::Create(fbxManager, "imp");

	//フレーム設定
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

void FbxModel::LoadMaterial(FbxNode* fbxNode)
{
	HRESULT result = S_FALSE;

	//マテリアルの個数確認
	const int materialCount = fbxNode->GetMaterialCount();

	if (materialCount > 0)
	{
		//先頭のマテリアルを取得
		FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);

		//テクスチャの有無確認
		bool textureLoaded = false;

		if (material)
		{
			//マテリアル名
			data->buffData[elementsNum].material.name = material->GetName();

			//ベースカラー
			const FbxProperty propBaseColor = FbxSurfaceMaterialUtils::GetProperty("baseColor", material);
			if (propBaseColor.IsValid())
			{
				//プロパティ値読み取り
				FbxDouble3 baseColor = propBaseColor.Get<FbxDouble3>();
				//書き込み
				data->buffData[elementsNum].material.baseColor.x = (float)baseColor.Buffer()[0];
				data->buffData[elementsNum].material.baseColor.y = (float)baseColor.Buffer()[1];
				data->buffData[elementsNum].material.baseColor.z = (float)baseColor.Buffer()[2];
			}

			//金属度
			const FbxProperty propMetalness = FbxSurfaceMaterialUtils::GetProperty("metalness", material);
			if (propMetalness.IsValid())
			{
				//書き込み
				data->buffData[elementsNum].material.metalness = propMetalness.Get<float>();
			}

			//鏡面反射度
			const FbxProperty propSpecular = FbxSurfaceMaterialUtils::GetProperty("specular", material);
			if (propSpecular.IsValid())
			{
				//書き込み
				data->buffData[elementsNum].material.specular = propSpecular.Get<float>();
			}

			//粗さ
			const FbxProperty propRoughness = FbxSurfaceMaterialUtils::GetProperty("roughness", material);
			if (propRoughness.IsValid())
			{
				//書き込み
				data->buffData[elementsNum].material.roughness = propRoughness.Get<float>();
			}

			//マテリアルデータ
			if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

				//環境光係数
				FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
				data->buffData[elementsNum].material.ambient.x = (float)ambient.Get()[0];
				data->buffData[elementsNum].material.ambient.y = (float)ambient.Get()[1];
				data->buffData[elementsNum].material.ambient.z = (float)ambient.Get()[2];

				//環境光係数
				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				data->buffData[elementsNum].material.diffuse.x = (float)diffuse.Get()[0];
				data->buffData[elementsNum].material.diffuse.y = (float)diffuse.Get()[1];
				data->buffData[elementsNum].material.diffuse.z = (float)diffuse.Get()[2];
			}

			//ディフューズテクスチャ
			const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (diffuseProperty.IsValid())
			{
				const FbxFileTexture* textureFile = diffuseProperty.GetSrcObject<FbxFileTexture>();

				if (textureFile)
				{
					const char* filePath = textureFile->GetFileName();

					//ファイルパスからファイル名取得
					std::string path_str(filePath);
					std::string fileName = ExtractFileName(path_str);

					//テクスチャ読み込み
					const std::string texName = baseDirectory + name + '/' + fileName;
					if (!texture[texName]) {
						TextureManager::LoadTexture(texName, texName);
						texture[texName]=std::make_unique<TextureManager>(texName);
					}
					data->buffData[elementsNum].texName = texName;
					textureLoaded = true;
				}
			}
		}

		//textureが無い場合白にする
		if (!textureLoaded)
		{
			data->buffData[elementsNum].texName = "white";
		}
	}
}

void FbxModel::CollectMesh(FbxNode* fbxNode)
{
	//メッシュ取得
	FbxMesh* fbxMesh = fbxNode->GetMesh();

	//頂点情報読み取り
	CollectVertices(fbxMesh);

	//面を構成するデータの読み取り
	CollectMeshFaces(fbxMesh);

	//マテリアルの読み取り
	LoadMaterial(fbxNode);

	elementsNum++;
}

void FbxModel::CollectVertices(FbxMesh* fbxMesh)
{
}

void FbxModel::CollectMeshFaces(FbxMesh* fbxMesh)
{
	//スキニング情報の読み取り
	std::vector<VectexProcess> vectexProcess;
	CollectSkin(fbxMesh, vectexProcess);

	//面の数
	const int polygonCount = fbxMesh->GetPolygonCount();

	//UVデータの数
	const int textureUVCount = fbxMesh->GetTextureUVCount();

	//UV名リスト
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	int inIndex = 0;

	//面ごとの情報
	for (int i = 0; i < polygonCount; i++)
	{
		//面の頂点数取得
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		//1頂点ごとに読み込み
		for (int j = 0; j < polygonSize; j++)
		{
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);

			//取得した法線を一時格納する
			FbxVector4 normal;

			Vertex add = {};

			//頂点格納
			add.pos = vectexProcess[index].pos;
			for (int num = 0; num < MAX_BONE_INDICES; num++) {
				add.boneWhight[num] = vectexProcess[index].boneWhight[num];
			}
			for (int num = 0; num < MAX_BONE_INDICES; num++) {
				add.boneIndex[num] = vectexProcess[index].boneIndex[num];
			}

			//法線読み込み
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{
				add.normal.x = (float)normal[0];
				add.normal.y = (float)normal[1];
				add.normal.z = (float)normal[2];
			}

			//テクスチャuv読み込み
			if (textureUVCount > 0)
			{
				FbxVector2 uvs;
				bool LUnmappedUV;

				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, LUnmappedUV))
				{
					add.uv.x = (float)uvs[0];
					add.uv.y = 1 - (float)uvs[1];
				}
			}

			//3頂点までのインデックス追加
			if (j < 3)
			{
				data->buffData[elementsNum].indices.push_back(inIndex);
				data->buffData[elementsNum].vertices.push_back(add);
			}
			////4頂点目がある場合
			//else
			//{
			//	//仮格納
			//	int index2 = data->buffData[elementsNum].indices[data->buffData[elementsNum].indices.size() - 1];
			//	int index3 = index;
			//	int index0 = data->buffData[elementsNum].indices[data->buffData[elementsNum].indices.size() - 3];

			//	//本格納
			//	data->buffData[elementsNum].indices.push_back(index2);
			//	data->buffData[elementsNum].indices.push_back(index3);
			//	data->buffData[elementsNum].indices.push_back(index0);
			//}

			inIndex++;
		}
	}
}

void FbxModel::CollectSkin(FbxMesh* fbxMesh, std::vector<VectexProcess>& _vectexProcess)
{
	//頂点座標データ数
	const int controlPointCount = fbxMesh->GetControlPointsCount();

	//Fbxメッシュの頂点座標取得
	FbxVector4* vertex = fbxMesh->GetControlPoints();

	//配列のサイズ変更
	std::vector<XMFLOAT3> vertList;
	_vectexProcess.resize(controlPointCount);

	//頂点をコピー
	for (int i = 0; i < controlPointCount; i++)
	{
		_vectexProcess[i].pos.x = (float)vertex[i][0];
		_vectexProcess[i].pos.y = (float)vertex[i][1];
		_vectexProcess[i].pos.z = (float)vertex[i][2];
	}

	//スキニング情報
	FbxSkin* fbxSkin =
		static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));

	//スキニング情報が無ければ終了
	if (fbxSkin == nullptr)
	{
		isSkinning = false;
		return;
	}

	//ボーン配列の参照
	std::vector<Bone>& bones = data->buffData[elementsNum].bones;

	//ボーンの数
	int clusterCount = fbxSkin->GetClusterCount();

	//配列確保
	bones.reserve(clusterCount);

	//全てのボーンについて
	for (int i = 0; i < clusterCount; i++)
	{
		//Fbxボーン情報
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

		//ボーン自体のノードの名前を取得
		const char* boenName = fbxCluster->GetLink()->GetName();

		//ボーンを追加して参照を得る
		bones.emplace_back(Bone(boenName));
		Bone& bone = bones.back();

		//自作ボーンとFBXを紐づける
		bone.fbxCluster = fbxCluster;

		//FBXから初期姿勢行列を取得
		FbxAMatrix fbxMat;
		fbxCluster->GetTransformLinkMatrix(fbxMat);

		//XMMatrix型に変換
		XMMATRIX initialPos;
		ConvertMatrixFormFbx(&initialPos, fbxMat);

		//初期姿勢行列の逆行列の取得
		bone.invInitialPose = XMMatrixInverse(nullptr, initialPos);
	}

	//スキンウェイト
	struct WeightSet
	{
		UINT index;
		float weight;
	};

	//保存用配列
	std::vector<std::list<WeightSet>> weightLists(_vectexProcess.size());

	//全てのボーンについて
	for (int i = 0; i < clusterCount; i++)
	{
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

		//ボーンに影響を受ける頂点の数
		int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();

		int* controlPointIndices = fbxCluster->GetControlPointIndices();
		double* controlWeights = fbxCluster->GetControlPointWeights();

		//影響を受ける全頂点について
		for (int j = 0; j < controlPointIndicesCount; j++)
		{
			//頂点番号
			int vertIndex = controlPointIndices[j];

			//スキンウェイト
			float weight = (float)controlWeights[j];

			//その頂点の影響を受けるボーンリストに、ボーンをウェイトのペアを追加
			weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
		}
	}

	//頂点配列書き換え用の参照
	auto& vertices = _vectexProcess;

	//各頂点について処理
	for (int i = 0; i < vertices.size(); i++)
	{
		//頂点のウェイトから最も大きい4つを選択
		auto& weightList = weightLists[i];

		//大小企画用のラムダ式を指定して降順にソート
		weightList.sort(
			[](auto const& lhs, auto const& rhs)
			{
				//左の要素が大きければtrue,でなければfalse
				return lhs.weight > rhs.weight;
			});

		int weightArrayIndex = 0;

		//降順ソート済みのウェイトリストから
		for (auto& WeightSet : weightList)
		{
			//頂点データに書き込み
			vertices[i].boneIndex[weightArrayIndex] = WeightSet.index;
			vertices[i].boneWhight[weightArrayIndex] = WeightSet.weight;

			//4つに達したら終了
			if (++weightArrayIndex >= MAX_BONE_INDICES)
			{
				float weight = 0.0f;

				//2番目以降のウェイト計算
				for (int j = 1; j < MAX_BONE_INDICES; j++)
				{
					weight += vertices[i].boneWhight[j];
				}

				//合計で1.0f(100%)になるよう調整
				vertices[i].boneWhight[0] = 1.0f - weight;
				break;
			}
		}
	}
}

void FbxModel::LoadNode(FbxNode* fbxNode, Node* parent)
{
	data->nodes.emplace_back();
	Node& virtualNode = data->nodes.back();

	virtualNode.name = fbxNode->GetName();

	//ノードのローカル情報
	FbxDouble3 rota = fbxNode->LclRotation;
	FbxDouble3 scaling = fbxNode->LclScaling;
	FbxDouble3 translation = fbxNode->LclTranslation;

	//形式変換して代入
	virtualNode.rotation = { (float)rota[0], (float)rota[1], (float)rota[2], 0.0f };
	virtualNode.scaling = { (float)scaling[0], (float)scaling[1], (float)scaling[2], 0.0f };
	virtualNode.translation = { (float)translation[0], (float)translation[1], (float)translation[2], 0.0f };

	//行列計算
	XMMATRIX matScaling, matRoration, matTranslation;
	matScaling = XMMatrixScalingFromVector(virtualNode.scaling);
	matRoration = XMMatrixRotationRollPitchYawFromVector(virtualNode.rotation);
	matTranslation = XMMatrixTranslationFromVector(virtualNode.translation);

	//変形行列計算
	virtualNode.transform = XMMatrixIdentity();
	virtualNode.transform *= matScaling;
	virtualNode.transform *= matRoration;
	virtualNode.transform *= matTranslation;

	//グローバル行列計算
	virtualNode.globalTransform = virtualNode.transform;
	if (parent)
	{
		//親を登録
		virtualNode.parent = parent;

		//親の変形を乗算
		virtualNode.globalTransform *= parent->globalTransform;
	}

	//メtッシュ情報の解析
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

	if (fbxNodeAttribute)
	{
		if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			//仮変数に入れたものをコンテナの一番後ろにセット
			data->meshNode = &virtualNode;
			data->buffData.emplace_back();
			CollectMesh(fbxNode);
		}
	}

	//子ノードがあれば再帰する
	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		LoadNode(fbxNode->GetChild(i), &virtualNode);
	}
}

void FbxModel::LoadAnimation(FbxScene* fbxScene, const int _animationNum)
{
	//0番目のアニメーション取得
	FbxAnimStack* animstick = fbxScene->GetSrcObject<FbxAnimStack>(_animationNum);

	//アニメーションが無ければスキップ
	if (!animstick) { return; }

	//アニメーションの名前取得
	const char* anumstackkname = animstick->GetName();
	//アニメーションの時間情報
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(anumstackkname);

	//開始時間
	data->fbxUpdate[_animationNum].startTime = takeinfo->mLocalTimeSpan.GetStart();

	//終了時間
	data->fbxUpdate[_animationNum].stopTime = takeinfo->mLocalTimeSpan.GetStop();

	//開始時間に合わせる
	data->fbxUpdate[_animationNum].nowTime = data->fbxUpdate[_animationNum].startTime;

	//再生可能
	data->fbxUpdate[_animationNum].isAnimation = true;

	std::vector<Bone>& bones = data->buffData[0].bones;
	int i = 0;
	if (baseBoneName != "") {
		for (; i < bones.size(); i++) {
			if (bones[i].name != baseBoneName) { continue; }
			break;
		}
	}
	else {
		baseBoneName = bones[0].name;
	}

	//今の姿勢行列
	XMMATRIX matCurrentPose;
	//現在の姿勢を取得
	FbxAMatrix fbxCurrentPose =
		data->buffData[0].bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(data->fbxUpdate[_animationNum].nowTime);
	//XMMATRIXに変換
	ConvertMatrixFormFbx(&matCurrentPose, fbxCurrentPose);
	boneMatWorld[baseBoneName] = matCurrentPose;

	//初期座標セット
	data->fbxUpdate[_animationNum].startPos =
		XMFLOAT3{ boneMatWorld[baseBoneName].r[3].m128_f32[0],
		boneMatWorld[baseBoneName].r[3].m128_f32[1],
		boneMatWorld[baseBoneName].r[3].m128_f32[2]
	};
}

void FbxModel::LoadFbx(const std::string modelName)
{
	elementsNum = 0;

	//名前の登録
	name = modelName;
	// モデルと同じ名前のフォルダから読み込む
	const std::string directoryPath = baseDirectory + modelName + "/";
	// 拡張子.fbxを付加
	const std::string fileName = modelName + ".fbx";
	// 連結してフルパスを得る
	const std::string fullpath = directoryPath + fileName;

	// ファイル名を指定してFBXファイルを読み込む
	if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings())) {
		assert(0);
	}

	// シーン生成
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");
	// ファイルからロードしたFBXの情報をシーンにインポート
	fbxImporter->Import(fbxScene);

	// FBXノードの数を取得
	int nodeCount = fbxScene->GetNodeCount();
	// あらかじめ必要数分のメモリを確保することで、アドレスがずれるのを予防
	data->nodes.reserve(nodeCount);

	//ノード読み込み
	LoadNode(fbxScene->GetRootNode());

	//アニメーションの設定
	int animStackCount = fbxImporter->GetAnimStackCount();
	data->fbxUpdate.resize(animStackCount);
	for (int i = 0; i < animStackCount; i++) {
		LoadAnimation(fbxScene, i);
	}

	data->fbxScene = fbxScene;

	texture["white"] = std::make_unique<TextureManager>("white");

	//モーションブレンド用の配列
	skinData.resize(data->buffData.size());

	for (int buffNum = 0; buffNum < data->buffData.size(); buffNum++)
	{
		//ボーン配列取得
		std::vector<Bone>& bones = data->buffData[buffNum].bones;
		for (int i = 0; i < bones.size(); i++)
		{
			if (bones[i].name == baseBoneName) { continue; }
			boneMatWorld[bones[i].name] = XMMatrixIdentity();
		}
	}
}

void FbxModel::Initialize(const int _createNum)
{
	HRESULT result = S_FALSE;

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * data->buffData[_createNum].vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * data->buffData[_createNum].indices.size());

	//頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&data->buffData[_createNum].vertBuff));
	assert(SUCCEEDED(result));

	//インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB), // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&data->buffData[_createNum].indexBuff));
	assert(SUCCEEDED(result));

	//頂点バッファへのデータ転送
	Vertex* vertMap = nullptr;
	result = data->buffData[_createNum].vertBuff->Map(0, nullptr, (void**)&vertMap);
	std::copy(data->buffData[_createNum].vertices.begin(), data->buffData[_createNum].vertices.end(), vertMap);
	data->buffData[_createNum].vertBuff->Unmap(0, nullptr);

	//インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = data->buffData[_createNum].indexBuff->Map(0, nullptr, (void**)&indexMap);
	std::copy(data->buffData[_createNum].indices.begin(), data->buffData[_createNum].indices.end(), indexMap);
	data->buffData[_createNum].indexBuff->Unmap(0, nullptr);

	//頂点バッファビューの生成
	data->buffData[_createNum].vbView.BufferLocation = data->buffData[_createNum].vertBuff->GetGPUVirtualAddress();
	data->buffData[_createNum].vbView.SizeInBytes = sizeVB;
	data->buffData[_createNum].vbView.StrideInBytes = sizeof(data->buffData[_createNum].vertices[0]);

	//インデックスバッファビューの作成
	data->buffData[_createNum].ibView.BufferLocation = data->buffData[_createNum].indexBuff->GetGPUVirtualAddress();
	data->buffData[_createNum].ibView.Format = DXGI_FORMAT_R16_UINT;
	data->buffData[_createNum].ibView.SizeInBytes = sizeIB;

	//定数バッファSkinの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin));
	assert(SUCCEEDED(result));
}

std::unique_ptr<FbxModel> FbxModel::Create(const std::string fileName, const std::string& _baseBoneName)
{
	// 3Dオブジェクトのインスタンスを生成
	FbxModel* instance = new FbxModel();

	instance->data = std::make_unique<Data>();

	instance->baseBoneName = _baseBoneName;

	//Fbxファイルの読み込み
	instance->LoadFbx(fileName);

	//Fbxの初期設定
	const int nodeNum = int(instance->data->buffData.size());
	for (int i = 0; i < nodeNum; i++) {
		instance->Initialize(i);
	}

	return std::unique_ptr<FbxModel>(instance);
}

bool FbxModel::Update(const int _animationNum, const bool _isRoop)
{
	HRESULT result;

	bool isAnim = true;
	if (!isAnimation) {
		isAnim = false;
	}
	if (data->fbxUpdate.size() != 0) {
		if (!data->fbxUpdate[_animationNum].isAnimation) {
			isAnim = false;
		}
	}

	//アニメーションしない場合
	if (!isAnim) { return true; }
	bool isAnimEnd = false;

	//アニメーション
	const float a = GameHelper::Instance()->GetGameSpeed();
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
	frameTime.Set(frameTime.Get() / 100 * (int)(a * 100));
	data->fbxUpdate[_animationNum].nowTime += frameTime;

	//最後まで行ったら先頭に戻す
	if (data->fbxUpdate[_animationNum].nowTime > data->fbxUpdate[_animationNum].stopTime)
	{
		if (_isRoop) {
			data->fbxUpdate[_animationNum].nowTime = data->fbxUpdate[_animationNum].startTime;
			beforePos = data->fbxUpdate[0].startPos;
		}
		isAnimEnd = true;
	}

	FbxAnimStack* pStack = data->fbxScene->GetSrcObject<FbxAnimStack>(_animationNum);
	data->fbxScene->SetCurrentAnimationStack(pStack);

	XMMATRIX boneMatWorld000 = {};

	for (int buffNum = 0; buffNum < data->buffData.size(); buffNum++)
	{
		//ボーン配列取得
		std::vector<Bone>& bones = data->buffData[buffNum].bones;

		// 定数バッファSkinへデータ転送
		ConstBufferDataSkin* constMapSkin = nullptr;
		result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
		if (isSkinning)
		{
			for (int i = 0; i < bones.size(); i++)
			{
				//今の姿勢行列
				XMMATRIX matCurrentPose;
				//現在の姿勢を取得
				FbxAMatrix fbxCurrentPose =
					bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(data->fbxUpdate[_animationNum].nowTime);
				//XMMATRIXに変換
				ConvertMatrixFormFbx(&matCurrentPose, fbxCurrentPose);
				//合成してスキニング行列に保存
				boneMatWorld[bones[i].name] = matCurrentPose;

				matCurrentPose.r[3].m128_f32[0] -= boneMatWorld[baseBoneName].r[3].m128_f32[0];
				matCurrentPose.r[3].m128_f32[1] -= boneMatWorld[baseBoneName].r[3].m128_f32[1];
				matCurrentPose.r[3].m128_f32[2] -= boneMatWorld[baseBoneName].r[3].m128_f32[2];
				skinData[buffNum].bones[i] = bones[i].invInitialPose * matCurrentPose;
				constMapSkin->bones[i] = skinData[buffNum].bones[i];

				if (bones[i].name == baseBoneName) {
					boneMatWorld000 = matCurrentPose;
				}
				else {
					boneMatWorld[bones[i].name] = matCurrentPose;
				}
			}
		}
		//スキニングをしない場合初期化値を送る
		else
		{
			skinData[buffNum].bones[0] = XMMatrixIdentity();
			constMapSkin->bones[0] = skinData[buffNum].bones[0];
		}
		constBuffSkin->Unmap(0, nullptr);
	}

	move = XMFLOAT3{ boneMatWorld[baseBoneName].r[3].m128_f32[0],
			boneMatWorld[baseBoneName].r[3].m128_f32[1],
			boneMatWorld[baseBoneName].r[3].m128_f32[2] } - beforePos;
	beforePos = XMFLOAT3{ boneMatWorld[baseBoneName].r[3].m128_f32[0],
			boneMatWorld[baseBoneName].r[3].m128_f32[1],
			boneMatWorld[baseBoneName].r[3].m128_f32[2] };

	boneMatWorld[baseBoneName] = boneMatWorld000;

	return isAnimEnd;
}

bool FbxModel::Update(FbxModel* _motionBlend, const float _rate1, const float _rate2, const int _animationNum, const bool _isRoop)
{
	HRESULT result;

	_motionBlend->Update();

	bool isAnim = true;
	if (!isAnimation) {
		isAnim = false;
	}
	if (data->fbxUpdate.size() != 0) {
		if (!data->fbxUpdate[_animationNum].isAnimation) {
			isAnim = false;
		}
	}

	//アニメーションしない場合
	if (!isAnim) { return true; }
	bool isAnimEnd = false;

	float a = GameHelper::Instance()->GetGameSpeed();
	if (a != 1.0) {
		frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
		frameTime.Set(frameTime.Get() * FbxLongLong(a));
	}
	data->fbxUpdate[_animationNum].nowTime += frameTime;

	//最後まで行ったら先頭に戻す
	if (data->fbxUpdate[_animationNum].nowTime > data->fbxUpdate[_animationNum].stopTime)
	{
		if (_isRoop) {
			data->fbxUpdate[_animationNum].nowTime = data->fbxUpdate[_animationNum].startTime;
			beforePos = data->fbxUpdate[0].startPos;
		}
		isAnimEnd = true;
	}

	XMMATRIX boneMatWorld000 = {};

	for (int buffNum = 0; buffNum < data->buffData.size(); buffNum++)
	{
		//ボーン配列取得
		std::vector<Bone>& bones = data->buffData[buffNum].bones;

		// 定数バッファSkinへデータ転送
		ConstBufferDataSkin* constMapSkin = nullptr;
		result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
		if (isSkinning)
		{
			for (int i = 0; i < bones.size(); i++)
			{
				//今の姿勢行列
				XMMATRIX matCurrentPose;
				//現在の姿勢を取得
				FbxAMatrix fbxCurrentPose =
					bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(data->fbxUpdate[_animationNum].nowTime);
				//XMMATRIXに変換
				ConvertMatrixFormFbx(&matCurrentPose, fbxCurrentPose);
				//合成してスキニング行列に保存
				boneMatWorld[bones[i].name] = matCurrentPose;

				matCurrentPose.r[3].m128_f32[0] -= boneMatWorld[baseBoneName].r[3].m128_f32[0];
				matCurrentPose.r[3].m128_f32[1] -= boneMatWorld[baseBoneName].r[3].m128_f32[1];
				matCurrentPose.r[3].m128_f32[2] -= boneMatWorld[baseBoneName].r[3].m128_f32[2];

				XMMATRIX motionBlendSkin = _motionBlend->GetSkinData(buffNum, i);
				XMMATRIX anime1 = bones[i].invInitialPose * matCurrentPose;
				skinData[buffNum].bones[i] = anime1 * _rate1 + motionBlendSkin * _rate2;
				constMapSkin->bones[i] = skinData[buffNum].bones[i];

				boneMatWorld[bones[i].name] = matCurrentPose * _rate1 + _motionBlend->GetBornMatWorld(bones[i].name) * _rate2;

				if (i != 0) {
					boneMatWorld[bones[i].name] = matCurrentPose;
				}
				else {
					boneMatWorld000 = matCurrentPose;
				}
			}
		}
		//スキニングをしない場合初期化値を送る
		else
		{
			skinData[buffNum].bones[0] = XMMatrixIdentity();
			constMapSkin->bones[0] = skinData[buffNum].bones[0];
		}
		constBuffSkin->Unmap(0, nullptr);
	}

	move = XMFLOAT3{ boneMatWorld[baseBoneName].r[3].m128_f32[0],
		boneMatWorld[baseBoneName].r[3].m128_f32[1],
		boneMatWorld[baseBoneName].r[3].m128_f32[2] } - beforePos;
	beforePos = XMFLOAT3{ boneMatWorld[baseBoneName].r[3].m128_f32[0],
			boneMatWorld[baseBoneName].r[3].m128_f32[1],
			boneMatWorld[baseBoneName].r[3].m128_f32[2] };

	boneMatWorld[baseBoneName] = boneMatWorld000;

	return isAnimEnd;
}

void FbxModel::Draw(ID3D12GraphicsCommandList* cmdList)
{
	int num = 0;
	for (auto& i : data->buffData) {
		//頂点バッファの設定
		cmdList->IASetIndexBuffer(&i.ibView);

		//頂点バッファをセット
		cmdList->IASetVertexBuffers(0, 1, &i.vbView);

		//定数バッファをセット
		cmdList->SetGraphicsRootConstantBufferView(3, constBuffSkin->GetGPUVirtualAddress());

		//シェーダーリソースビューをセット
		if (i.texName == "") {
			cmdList->SetGraphicsRootDescriptorTable(4, texture["white"]->GetDescriptor()->gpu);
		}
		else {
			cmdList->SetGraphicsRootDescriptorTable(4, texture[i.texName]->GetDescriptor()->gpu);
		}
		//描画コマンド
		cmdList->DrawIndexedInstanced((UINT)i.indices.size(), 1, 0, 0, 0);

		num++;
	}
}

void FbxModel::Finalize()
{
	fbxImporter->Destroy();
	fbxManager->Destroy();
}

std::string FbxModel::ExtractFileName(const std::string& path)
{
	size_t pos1;

	//区切り文字'\\'が出てくる一番最後の部分を検索

	pos1 = path.rfind('\\');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	//区切り文字'/'が出てくる一番最後の部分を検索
	pos1 = path.rfind('/');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}

void FbxModel::ConvertMatrixFormFbx(DirectX::XMMATRIX* dst, const FbxMatrix& src)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			dst->r[i].m128_f32[j] = (float)src.Get(i, j);
		}
	}
}