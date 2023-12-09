#pragma once
#include "Particle/Emitter.h"
#include "Object/3d/Object3d.h"
#include "Math/Vector3.h"

class AttachEmitter
{
public:
	AttachEmitter(const std::string& texName);
	~AttachEmitter(){};

	void Add(const Vector3& _point1, const Vector3& _point2);

	void Update();

	void Draw();

private:

	std::unique_ptr<Emitter> particle;
};

