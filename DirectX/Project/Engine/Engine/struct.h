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

// ===========
// TileMap 타일 하나의 정보
// ===========

struct tTileData
{
	int  iImgIdx;
	Vec2 vLTUV;
	int  iPadding;

	// iImgIdx를 -1로 초기화
	tTileData()
		:iImgIdx(-1)
		,iPadding(0)
	{}
};


// ==========
// Animtion2D
// ==========
struct tAnim2DFrame
{
	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vOffset;
	float	fDuration;
};

// ========
// Particle
// ========

struct tParticle
{
	Vec3	vPos;	// 위치
	Vec3	vScale;	// 크기
	Vec3	vDir;	// 방향
	Vec4	vColor;	// 색상
	
	int		Alive;	// 생사
	Vec2	Padding;

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

	int bTex[10];
	int iPadding[2];
};

struct tAnim2D
{
	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vBackgroundSize;
	Vec2    vOffset;

	int     useAnim2D;
	float   Atlas_Width;
	float   Atlas_Height;
	float	Anim2D_Padding;
};

// 전역 데이터이기 때문에 한번만 잡아주면 된다.
struct tGlobal
{
	Vec2  vResolution;	// 렌더타겟의 렌더링 해상도
	float fDT;			// delta time(프레임 간격간의 시간)
	float fAccTime;		// 게임을 킨 이후로 증가하는 시간

};