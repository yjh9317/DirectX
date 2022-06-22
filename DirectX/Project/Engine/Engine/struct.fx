#ifndef _STRUCT
#define _STRUCT


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
	
    int     Alive; // 생사
    float   m_fCurTime;
    float   m_fMaxTime;

};

struct tParticleData
{
    int     iAliveCount;
    float3  vPadding;
};

struct tLightColor
{
    float4 vDiff;
    float4 vSpec;
    float4 vAmb;
};

struct tLightInfo
{
    tLightColor color;
    float3 vLightDir;
    int iLightType;
    float3 vWorldPos;
    float fAngle;
    float fRange;
    float3 vPadding;
};
#endif
