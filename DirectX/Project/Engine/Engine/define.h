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

#define KEY_TAP(key) KEY_STATE::TAP == CKeyMgr::GetInst()->GetKeyState(key)
#define KEY_PRESSED(key) KEY_STATE::PRESSED == CKeyMgr::GetInst()->GetKeyState(key)
#define KEY_AWAY(key) KEY_STATE::AWAY == CKeyMgr::GetInst()->GetKeyState(key)


#define DEVICE CDevice::GetInst()->GetDevice().Get()
#define CONTEXT CDevice::GetInst()->GetDeviceContext().Get()

#define MAX_LAYER 32

#define SAFE_DELETE(p) if(nullptr != p) delete p; p = nullptr;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;


enum class RES_TYPE
{
	MATERIAL,
	MESHDATA,
	MESH,
	TEXTURE,
	SOUND,
	GRAPHICS_SHADER,
	COMPUTE_SHADER,
	END,
};


enum class CB_TYPE
{
	TRANSFORM,		// b0
	MATERIAL_PARAM, // b1
	GLOBAL,			// b2
	END,
};

enum class COMPONENT_TYPE
{
	TRANSFORM,		// 위치 ,크기 , 회전 (Location)

	COLLIDER2D,		// 2D 충돌체
	COLLIDER3D,		// 3D 충돌체

	ANIMATOR2D,		// 2D Sprite Animation
	ANIMATOR3D,		// 3D Bone Skinning Animation

	BOUNDINGBOX,	// Picking,	FrustumCulling

	//render

	MESHRENDER,			// Mesh Renderer
	TILEMAP,			// 2D Tile Map Renderer
	PARTICLESYSTEM,		// 입자 렌더링
	LANDSCAPE,			// 지형
	DECAL,				// 데칼 오브젝트

	// Update
	SCRIPT,				// 로직, 엔진쪽에서 구현하지 않고 개발자가 구현해야할 컴포넌트


	END,
};