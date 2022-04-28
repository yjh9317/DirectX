#pragma once


struct tEventInfo
{
	EVENT_TYPE	eType;
	DWORD_PTR	lParam;
	DWORD_PTR	wParam;
};


struct Vertex
{
	Vec3 vPos;		// ��ǥ	
	Vec4 vColor;	// ����

	Vec2 vUV;

};

typedef Vertex Vtx;

// ===========
// TileMap Ÿ�� �ϳ��� ����
// ===========

struct tTileData
{
	int  iImgIdx;
	Vec2 vLTUV;
	int  iPadding;

	// iImgIdx�� -1�� �ʱ�ȭ
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
	Vec3	vPos;	// ��ġ
	Vec3	vScale;	// ũ��
	Vec3	vDir;	// ����
	Vec4	vColor;	// ����
	
	int		Alive;	// ����
	Vec2	Padding;

};


// ===================
// ������� ���� ����ü
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
	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vBackgroundSize;
	Vec2    vOffset;

	int     useAnim2D;
	float   Atlas_Width;
	float   Atlas_Height;
	float	Anim2D_Padding;
};