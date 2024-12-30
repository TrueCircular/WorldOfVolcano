#pragma once
#include "Component.h"

class Transform : public Component, public enable_shared_from_this<Transform>
{
	using Super = Component;
public:
	Transform();
	virtual ~Transform(); 
private:
	shared_ptr<Transform>			_parent;
	vector<shared_ptr<Transform>>	_children;
	vector<TransformMetaData>		_transfromMetaDataList;
private:
	//local
	Vec3 _localScale = { 1.f,1.f,1.f };
	Vec3 _localRotation = { 0.f,0.f,0.f };
	Vec3 _localPosition = { 0.f,0.f,0.f };
	//world
	Vec3 _scale = { 1,1,1 };
	Vec3 _rotation = { 0,0,0 };
	Vec3 _position = { 0,0,0 };
public:
	bool _isItemTransform = false;
private:
	Matrix _matLocal = Matrix::Identity;
	Matrix _matWorld = Matrix::Identity;
public:
	static Vec3 QuatToEulerAngles(Quaternion q);
public:
	bool HasParent() { return _parent != nullptr; }
public:
	shared_ptr<Transform> GetParent() const { return _parent; }
	const vector<shared_ptr<Transform>>& GetChildren() { return _children; }
public:
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	void AddChild(shared_ptr<Transform> child);
public:
	const Vec3& GetLocalScale() const { return _localScale; }
	const Vec3& GetLocalRotation() const { return _localRotation; }
	const Vec3& GetLocalPosition() const { return _localPosition; }
	const Vec3& GetScale() const { return _scale; }
	const Vec3& GetRotation() const { return _rotation; }
	const Vec3& GetPosition() const { return _position; }
	Vec3& GetPositionRef() { return _position; }
	const Matrix& GetWorldMatrix() const { return _matWorld; }
public:
	const Vec3& GetUpVector() const { return _matWorld.Up(); }
	const Vec3& GetRightVector() const { return _matWorld.Right(); }
	const Vec3& GetLookVector() const { return _matWorld.Backward(); }
public:
	//local
	void SetLocalScale(Vec3 scale) { _localScale = scale; UpdateTransform(); }
	void SetLocalRotation(Vec3 rot) { _localRotation = rot; UpdateTransform();}
	void SetLocalPosition(Vec3 pos) { _localPosition = pos; UpdateTransform();}
	//world
	void SetScale(Vec3 scale);
	void SetRotation(Vec3 rot);
	void SetPosition(Vec3 pos);
	void SetLookVector(Vec3 look) { _matWorld.Backward() = look; }
	void SetLocalMatrix(Matrix mat) { _matLocal = mat; Quaternion qr; _matLocal.Decompose(_localScale, qr, _localPosition); _localRotation = QuatToEulerAngles(qr); UpdateTransform(); }
private:
	void PreorderTransfroms(const shared_ptr<Transform>& node, int32 localIndex, int32 parentIndex);
public:
	virtual void LoadMetaData(wstring& metaPath) override;
	virtual void SaveMetaData(wstring& metaPath) override;
public:
	virtual void Update() override;
	void UpdateTransform();
};

