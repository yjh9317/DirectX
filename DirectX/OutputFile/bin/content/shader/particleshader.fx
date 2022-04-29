#ifndef _PARTICLESHADER
#define _PARTICLESHADER

#include "value.fx"
#include "struct.fx"

// value.fx WorldPos�� ������ �������� ������Ʈ
// ������ ��ġ�� ������Ʈ�� �θ��ڽ����� �ϸ� ���ڵ��� ������Ʈ�� �̵��ϸ� ���� �̵��ϰ�
// ���� ������ ��ǥ�� ����� ����ϸ� ������Ʈ�� �̵��ϸ鼭 ���ڸ� �����Ͽ� �̵��� ������ ���ڰ� ���´�.


// ====================
// ParticleRenderShader
// Mesh     : RectMesh
// Domain   : Opaque (������)
// Blend    : AlphaBlend
// DepthStencil : Less(Default)
// Rasterizer   : CULL_NONE
// ====================


// Parameter
#define PARTICLE_INDEX g_int_0  // ������ �ε���
#define POS_INHERIT g_int_1     // ���� ���

StructuredBuffer<tParticle> ParticleBuffer : register(t16);

struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VTX_OUT VS_ParticleRender(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    // ��ƼŬ ������ ������ġ ���ϱ�
    float3 vParticleWorldPos = (float3) 0.f;
    
    if(POS_INHERIT)
    {
        // ������Ʈ(�θ�)�� ��ġ�� ������̹Ƿ� ������Ʈ ��ġ��ı��� ��
        float3 vParticleWorldPos = _in.vPos * ParticleBuffer[PARTICLE_INDEX].vScale + ParticleBuffer[PARTICLE_INDEX].vPos + g_matWorld._41_42_43;;
    }
    else
    {
        float3 vParticleWorldPos = _in.vPos * ParticleBuffer[PARTICLE_INDEX].vScale + ParticleBuffer[PARTICLE_INDEX].vPos;
    }
    
    // World��ǥ�� View,Proj�� ��ȯ
    float4 vViewPos = mul(float4(vParticleWorldPos, 1.f), g_matView);
    output.vPosition = mul(vViewPos, g_matProj);
    
    return output;
}

float4 PS_ParticleRender(VTX_OUT _in) : SV_Target   //��ȯŸ���� float4�ε� ����Ÿ�ٿ� ���
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    
    return vOutColor;
}

#endif