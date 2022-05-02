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
    
    uint InstID : SV_InstanceID; // 호출된 정점의 순서(인덱스)를 저장해주는 시멘틱
    // RectMesh의 정점 6개에 처음에는 모두 0이 들어오고 그 다음 정점6개에는 1이 들어오고 이런식으로 증가
};

struct VTX_OUT
{
    float4 vPosition : SV_Position; //SV_Position : Rasterizer직전에 줄때 사용하는 시멘틱
    float2 vUV : TEXCOORD;
    
    float4 vColor : COLOR;
    float fAlive : FOG; //float 하나 보낼때 사용하는 시맨틱
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

// 스레드간의 동기화 : 병렬처리를 하므로 한번에 처리하는 방식에서 문제가 생기는걸 막아야함.

// Bilboard문제 : 입자들이 카메라(시점)에 따라 정면을 바라보지 않을 수 있다.
// View Space까지 이동후 점을 분할하여 Bilboard 해결
// void GS_ParticleRender();

#endif