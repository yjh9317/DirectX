#ifndef _STRUCT
#define _STRUCT

#include "value.fx"

// 쉐이더에서 사용할 구조체를 모아놓은 파일

struct tTileData
{
    int iImgIdx;
    float2 vLTUV;
    int iPadding;
};

struct tParticle
{
    float3 vPos; // 위치
    float3 vScale; // 크기
    float3 vDir; // 방향
    float4 vColor; // 색상
	
    int Alive; // 생사
    float2 Padding;

};


#endif
