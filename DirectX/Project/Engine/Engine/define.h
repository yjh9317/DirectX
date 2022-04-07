#pragma once


#define SINGLE(TYPE) friend class CSingleton<TYPE>;\
					 private:\
						TYPE();\
						~TYPE();

#define DT CTimeMgr::GetInst()->GetDT()

#define KEY_TAP(key) KEY_STATE::TAP == CKeyMgr::GetInst()->GetKeyState(key)
#define KEY_PRESSED(key) KEY_STATE::PRESSED == CKeyMgr::GetInst()->GetKeyState(key)
#define KEY_AWAY(key) KEY_STATE::AWAY == CKeyMgr::GetInst()->GetKeyState(key)

#define DEVICE CDevice::GetInst()->GetDevice().Get()
#define CONTEXT CDevice::GetInst()->GetDeviceContext().Get()

#define MAX_LAYER 32

#define SAFE_DELETE(p) if(nullptr != p) delete p; p = nullptr;
#define CLONE(type) virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) virtual type* Clone() { return nullptr; }





typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;


enum class RES_TYPE	//�ٸ� ���Ҹ��� �����ϴ� ���ҽ�Ÿ�Ժ��� ��ܿ� ����. ������� �����ǰ� �����Ǳ� ����
{
	// ������ ���ҽ�
	PREFAB,
	MESHDATA,
	MATERIAL,



	// ���� ���ҽ�
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

	// Rasterizer option
enum class RS_TYPE
{
	CULL_BACK,	// Default
	CULL_FRONT,
	CULL_NONE, 
	WIRE_FRAME, 
	END,
};

// DepthStencil Option
enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,

	GREATER,		// ���̰� �� �־�� ���
	GREATER_EQUAL,

	NO_TEST,		// ���� �׽�Ʈ ���� ����, ���̴� ���
	NO_WRITE,		// ���� �׽�Ʈ ���� , ���̸� ������� ����

	NO_TEST_NO_WRITE,	// ���� �׽�Ʈ ���� ����, ���� ������� ����
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


enum class SCALAR_PARAM
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

enum class TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,

	TEX_CUBE_0,
	TEX_CUBE_1,
		
	TEX_ARR_0,
	TEX_ARR_1,

	END,
};


enum class PIPELINE_STAGE
{
	VS = 0x01,
	HS = 0x02,
	DS = 0x04,
	GS = 0x08,
	PS = 0x10,	

	NO_PS = VS | HS | DS | GS,
	ALL = VS | HS | DS | GS | PS,
};


enum class EVENT_TYPE
{
	CREATE_OBJ,			// lParam : Object Adress, wParam : Layer Index
	DELETE_OBJ,			// lParam : Object Adress
	ADD_CHILD,			// lParam : Parent Object, wParam : Child Object
	STAGE_CHANGE,		// lParam : Next Stage Enum
	CHANGE_AI_STATE,	// lParam : FSM Adress, wParam : Next State Type

	END,
};