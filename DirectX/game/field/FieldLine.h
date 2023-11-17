#pragma once
#include "Object/3d/PrimitiveObject3D.h"
#include "Math/Timer.h"

class FieldLine
{
private:

	enum class MoveDirection {
		x,
		x_,
		y,
		y_,
		z,
		z_,
		size
	};

public:

	FieldLine();
	~FieldLine(){};


	static void StaticInitialize();

	void Update();

	static void StaticUpdate();

	static void StaticDraw();

	static void FrameReset() {
		object->ResetVertex();
	}

	bool GetIsEnd() { return isEnd; }

private:

	static std::unique_ptr<PrimitiveObject3D> object;

	std::array<std::unique_ptr<Engine::Timer>, 2> timer;
	MoveDirection direction;
	DirectX::XMFLOAT3 spos;
	DirectX::XMFLOAT3 epos;
	bool isEnd;

};

