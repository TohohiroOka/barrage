#pragma once
#include "Scene/InterfaceScene.h"
#include "engine/camera/DebugCamera.h"
#include "engine/Camera/LightCamera.h"

#include <memory>

class TitleScene : public InterfaceScene
{
private:// エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMINT3 = DirectX::XMINT3;

	//選択肢強調表示用クラス
	class ChoiceEmphasisDrawer
	{
	public:
		ChoiceEmphasisDrawer() = default;
		~ChoiceEmphasisDrawer() = default;

		void Initialize();

		void Update();

		void Draw();

		void SetEmphasisPos(float posX, float posY, float sizeX, float sizeY);

		void PlayChoiseAnimation();

		bool IsChooseAnimEnd();

	private:
		//強調用ぼかし
		std::unique_ptr<Sprite> emphasisSprite;

		//時間
		int ALPHA_ANIM_FRAME = 90;

		bool isChoose = false;
		bool isChooseOld = false;

		const int CHOICE_ANIM_FRAME = 60;
		int choiceAnimFrame = 0;
	};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	///	更新
	/// </summary>
	void Update()override;

	/// <summary>
	///	描画
	/// </summary>
	/// <param name="_cameraNum">カメラ番号</param>
	void Draw(const int _cameraNum) override;

	/// <summary>
	///	描画
	/// </summary>
	/// <param name="_cameraNum">カメラ番号</param>
	void DrawLightView(const int _cameraNum) override;

	/// <summary>
	/// ポストエフェクトをかけない描画
	/// </summary>
	void NonPostEffectDraw(const int _cameraNum) override;

	/// <summary>
	///	解放
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// imguiの表示
	/// </summary>
	void ImguiDraw() override;

	/// <summary>
	/// 
	/// </summary>
	void FrameReset() override;

	void CollisionCheck();

private:
	//選択中
	enum class PLAYER_SELECT
	{
		SELECT_STARTGAME,
		SELECT_CONFIG,
		SELECT_EXIT,
	};
	PLAYER_SELECT selecting = PLAYER_SELECT::SELECT_STARTGAME;

	bool isPressed = false;
	bool isSelected = false;

	ChoiceEmphasisDrawer choiceDrawer;
	//表示座標系
	const float OPTIONS_START_Y = 550.f;
	const float OPTIONS_DISTANCE_Y = 50.f;
	//カメラ
	std::unique_ptr<DebugCamera> debugCamera;
	std::unique_ptr<LightCamera> lightCamera;
	//スプライト
	std::unique_ptr<Sprite> titleLogoSprite;
	std::unique_ptr<Sprite> pressAnyButtonSprite;
	std::unique_ptr<Sprite> gamestartSprite;
	std::unique_ptr<Sprite> configSprite;
	std::unique_ptr<Sprite> exitgameSprite;
	//時間
	const int FADEIN_FRAME = 150;
	int fadeinFrame = 0;


	//入力判定用
	bool IsUp();
	bool IsDown();
	bool IsEnter();

};

