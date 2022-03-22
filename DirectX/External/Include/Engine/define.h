#pragma once


#define SINGLE(TYPE) private:\
	TYPE();\
	~TYPE();\
public:\
	static TYPE* GetInst()\
	{\
		static TYPE mgr;\
		return &mgr;\
	}

#define DT CTimeMgr::GetInst()->GetDT()

#define KEY_TAP(key) KEY_STATE::TAP ==CKeyMgr::GetInst()->GetKeyState(key)
#define KEY_PRESSED(key) KEY_STATE::PRESSED ==CKeyMgr::GetInst()->GetKeyState(key)
#define KEY_AWAY(key) KEY_STATE::AWAY ==CKeyMgr::GetInst()->GetKeyState(key)


#define DEVICE CDevice::GetInst()->GetDevice().Get()
#define CONTEXT CDevice::GetInst()->GetDeviceContext().Get()


typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;
