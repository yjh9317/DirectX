#pragma once


struct tEventInfo
{
	EVENT_TYPE	eType;
	DWORD_PTR	lParam;
	DWORD_PTR	wParam;
};


struct Vertex
{
	Vec3 vPos;		// 좌표	
	Vec4 vColor;	// 색상

	Vec2 vUV;

};

typedef Vertex Vtx;


// Animation 2D

struct tAnim2DFrame
{
	Vec2 _vLT;			// 좌상단
	Vec2 _vSlice;		// 길이
	float fDuration;	// 지속시간
};



// ===================
// 상수버퍼 대응 구조체
// ===================
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

struct tScalarParam
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];
};

struct tAnim2D
{
	int     useAnim2D;         //애니메이션의 사용여부
	float   Atlas_Width;
	float   Atlas_Height;
	Vec2 vLT;
	Vec2 vSlice;
	Vec2 vBaseSize;
	Vec3 Anim2D_Padding;
};