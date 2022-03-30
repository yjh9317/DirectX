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
	SCALAR_PARAM,	// b1
	GLOBAL,			// b2
	END,
};

enum class RS_TYPE
{
	CULL_BACK,	// Default
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

enum class COMPONENT_TYPE
{
	TRANSFORM,	// ��ġ, ũ��, ȸ�� (Location)

	CAMERA,		// ȭ���� ��� ī�޶� ����

	COLLIDER2D, // 2D �浹ü
	COLLIDER3D, // 3D �浹ü

	ANIMATOR2D, // 2D Sprite Animation
	ANIMATOR3D, // 3D Bone Skinning Animation

	BOUNDINGBOX, // Picking, FrustumCulling

	// renderer
	MESHRENDER,		// Mesh Renderer
	TILEMAP,		// 2D Tile Map Renderer
	PARTICLESYSTEM, // ���� ������
	LANDSCAPE,		// ���� ������
	DECAL,			// ��Į ������Ʈ

	// Update
	SCRIPT,			// ����

	END,
};


enum class SCALAR_PARAM	//Material�� �߰� ���
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,

	END,
};