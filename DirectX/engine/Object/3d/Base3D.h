#pragma once
#include "Object/ObjectBase.h"
#include "Model.h"
#include "Object/3d/collider/CollisionInfo.h"

class BaseCollider;
class Camera;
class LightGroup;

class Base3D : public ObjectBase
{
public:

	/// <summary>
	/// �J�����̃Z�b�g
	/// </summary>
	/// <param name="_camera">�J����</param>
	static void SetCamera(Camera* _camera) { Base3D::camera = _camera; }

	/// <summary>
	/// ���C�g�O���[�v�̃Z�b�g
	/// </summary>
	/// <param name="_light">���C�g�O���[�v</param>
	static void SetLightGroup(LightGroup* _light) { Base3D::light = _light; }

public:

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �s��̍X�V
	/// </summary>
	virtual void UpdateWorldMatrix();

	/// <summary>
	/// �����蔻��̕`��
	/// </summary>
	virtual void ColliderDraw();

	/// <summary>
	/// �R���C�_�[�̍폜
	/// </summary>
	virtual void DeleteCollider();

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="_info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo& _info) {}

protected:
	//�J����
	static Camera* camera;
	//���C�g
	static LightGroup* light;

	// �R���C�_�[
	BaseCollider* collider = nullptr;
	// ���[�J�����W�ϊ��s��
	XMMATRIX matTrans = {};
	// �T�C�Y�ϊ��s��
	XMMATRIX matScale = {};
	// �p�x�ϊ��s��
	XMMATRIX matRot = {};
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld = {};
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	// �e�I�u�W�F�N�g
	Base3D* parent = nullptr;

	//�u���[���̗L��
	bool isBloom;
	//�g�D�[���̗L��
	bool isToon;
	//�A�E�g���C���̗L��
	bool isOutline;
	//���C�e�B���O
	bool isLight;
	//�A�E�g���C���F
	XMFLOAT3 outlineColor;

public:

	const XMFLOAT3& GetPosition() { return position; }
	const XMFLOAT3& GetRotation() { return rotation; }
	const XMFLOAT3& GetScale() { return scale; }
	BaseCollider* GetCollider() { return collider; }
	const XMMATRIX& GetMatRot() { return matRot; }
	const XMMATRIX& GetMatWorld() { return matWorld; }
	const XMFLOAT3 GetWorldPosition();
	bool GetBloom() { return isBloom; }
	void SetParent(Base3D* _parent) { parent = _parent; }
	void SetPosition(const XMFLOAT3& _position) { position = _position; }
	void SetRotation(const XMFLOAT3& _rotation) { rotation = _rotation; }
	void SetScale(const XMFLOAT3& _scale) { scale = _scale; }
	void SetCollider(BaseCollider* _collider);
	void SetMatWorld(const XMMATRIX& _matWorld) { matWorld = _matWorld; }
	void SetBloom(bool _isBloom) { isBloom = _isBloom; }
	void SetToon(bool _isToon) { isToon = _isToon; }
	void SetLight(bool _isLight) { isLight = _isLight; }
	void SetOutline(bool _isOutline) { isOutline = _isOutline; }
	void SetOutlineColor(const XMFLOAT3& _outlineColor) { outlineColor = _outlineColor; }

};