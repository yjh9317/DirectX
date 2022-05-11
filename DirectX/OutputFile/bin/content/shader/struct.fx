#ifndef _STRUCT
#define _STRUCT

#include "value.fx"

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


#endif
