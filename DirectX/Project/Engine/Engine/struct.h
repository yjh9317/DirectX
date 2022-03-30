#pragma once


struct Vertex		//3차원 공간에서 사용할 구조체
{
	Vec3 vPos;		// 정점의 좌표
	Vec4 vColor;	// 정점의 색상 정보
};

typedef Vertex Vtx;


// ================
// 상수버퍼 대응 구조체
// ================

struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
};


struct tScalarParam
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];
};