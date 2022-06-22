#ifndef _STRUCT
#define _STRUCT


// ���̴����� ����� ����ü�� ��Ƴ��� ����

struct tTileData
{
    int iImgIdx;
    float2 vLTUV;
    int iPadding;
};

struct tParticle
{
    float3 vPos; // ��ġ
    float3 vScale; // ũ��
    float3 vDir; // ����
    float4 vColor; // ����
	
    int     Alive; // ����
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
