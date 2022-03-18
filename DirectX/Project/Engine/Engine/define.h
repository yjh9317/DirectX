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

#define DEVICE CDevice::GetInst()->GetDevice().Get()
#define CONTEXT CDevice::GetInst()->GetDeviceContext.Get()


typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;
