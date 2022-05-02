#ifndef _PARTICLERENDER
#define _PARTICLERENDER

#include "value.fx"
#include "struct.fx"

// ====================
// ParticleRenderShader
// Mesh     : PointMesh
// Domain   : Opaque
// Blend    : AlphaBlend
// DepthStencil : Less
// Rasterizer : CULL_NONE


// Parameter
#define PARTICLE_INDEX  g_int_1
#define POS_INHERIT     g_int_0
// ====================

StructuredBuffer<tParticle> ParticleBuffer : register(t16);

struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    uint InstID : SV_InstanceID; // ȣ��� ������ ����(�ε���)�� �������ִ� �ø�ƽ
    // RectMesh�� ���� 6���� ó������ ��� 0�� ������ �� ���� ����6������ 1�� ������ �̷������� ����
};

struct VTX_OUT
{
    float4 vPosition : SV_Position; //SV_Position : Rasterizer������ �ٶ� ����ϴ� �ø�ƽ
    float2 vUV : TEXCOORD;
    
    float4 vColor : COLOR;
    float fAlive : FOG; //float �ϳ� ������ ����ϴ� �ø�ƽ
};


VTX_OUT VS_ParticleRender(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
  
    
    // ��ƼŬ ������ ������ġ ���ϱ�
    float3 vParticleWorldPos = (float3) 0.f;
    
    if (POS_INHERIT)
    {
        // ������Ʈ(�θ�)�� ��ġ�� ������̹Ƿ� ������Ʈ ��ġ��ı��� ��
        vParticleWorldPos = _in.vPos * ParticleBuffer[_in.InstID].vScale + ParticleBuffer[_in.InstID].vPos + g_matWorld._41_42_43;
    }
    else
    {
        vParticleWorldPos = _in.vPos * ParticleBuffer[_in.InstID].vScale + ParticleBuffer[_in.InstID].vPos;
    }
    
    // World ��ǥ�� View, Proj ���� ��ȯ
    float4 vViewPos = mul(float4(vParticleWorldPos, 1.f), g_matView);
    output.vPosition = mul(vViewPos, g_matProj);
    output.vColor = ParticleBuffer[_in.InstID].vColor;
    output.fAlive = (float) ParticleBuffer[_in.InstID].Alive;
    
    return output;
}

float4 PS_ParticleRender(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    if (0.f == _in.fAlive)
        discard;
    
    vOutColor = _in.vColor;
    
    return vOutColor;
}

// �����尣�� ����ȭ : ����ó���� �ϹǷ� �ѹ��� ó���ϴ� ��Ŀ��� ������ ����°� ���ƾ���.

// Bilboard���� : ���ڵ��� ī�޶�(����)�� ���� ������ �ٶ��� ���� �� �ִ�.
// View Space���� �̵��� ���� �����Ͽ� Bilboard �ذ�
// void GS_ParticleRender();

#endif