#ifndef _PARTICLERENDER
#define _PARTICLERENDER

#include "value.fx"
#include "struct.fx"

// ====================
// ParticleRenderShader
// Mesh     : RectMesh
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
    
    uint InstID : SV_InstanceID; // 호출된 정점의 순서(인덱스)를 저장해주는 시멘틱
    // RectMesh의 정점 6개에 처음에는 모두 0이 들어오고 그 다음 정점6개에는 1이 들어오고 이런식으로 증가
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};


VTX_OUT VS_ParticleRender(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
  
    
    // 파티클 정점의 월드위치 구하기
    float3 vParticleWorldPos = (float3) 0.f;
    
    if (POS_INHERIT)
    {
        // 오브젝트(부모)의 위치에 상대적이므로 오브젝트 위치행렬까지 곱
        vParticleWorldPos = _in.vPos * ParticleBuffer[_in.InstID].vScale + ParticleBuffer[_in.InstID].vPos + g_matWorld._41_42_43;
    }
    else
    {
        vParticleWorldPos = _in.vPos * ParticleBuffer[_in.InstID].vScale + ParticleBuffer[_in.InstID].vPos;
    }
    
    // World 좌표를 View, Proj 으로 변환
    float4 vViewPos = mul(float4(vParticleWorldPos, 1.f), g_matView);
    output.vPosition = mul(vViewPos, g_matProj);
    
    return output;
}

float4 PS_ParticleRender(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    
    return vOutColor;
}

#endif