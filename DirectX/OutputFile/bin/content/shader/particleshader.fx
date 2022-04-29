#ifndef _PARTICLESHADER
#define _PARTICLESHADER

#include "value.fx"
#include "struct.fx"

// value.fx WorldPos는 입자의 소유주인 오브젝트
// 입자의 위치는 오브젝트와 부모자식으로 하면 입자들이 오브젝트가 이동하면 같이 이동하고
// 입자 본인의 좌표를 월드로 사용하면 오브젝트가 이동하면서 입자를 스폰하여 이동한 흔적에 입자가 남는다.


// ====================
// ParticleRenderShader
// Mesh     : RectMesh
// Domain   : Opaque (반투명)
// Blend    : AlphaBlend
// DepthStencil : Less(Default)
// Rasterizer   : CULL_NONE
// ====================


// Parameter
#define PARTICLE_INDEX g_int_0  // 입자의 인덱스
#define POS_INHERIT g_int_1     // 입자 상속

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
    
    // 파티클 정점의 월드위치 구하기
    float3 vParticleWorldPos = (float3) 0.f;
    
    if(POS_INHERIT)
    {
        // 오브젝트(부모)의 위치에 상대적이므로 오브젝트 위치행렬까지 곱
        float3 vParticleWorldPos = _in.vPos * ParticleBuffer[PARTICLE_INDEX].vScale + ParticleBuffer[PARTICLE_INDEX].vPos + g_matWorld._41_42_43;;
    }
    else
    {
        float3 vParticleWorldPos = _in.vPos * ParticleBuffer[PARTICLE_INDEX].vScale + ParticleBuffer[PARTICLE_INDEX].vPos;
    }
    
    // World좌표를 View,Proj로 변환
    float4 vViewPos = mul(float4(vParticleWorldPos, 1.f), g_matView);
    output.vPosition = mul(vViewPos, g_matProj);
    
    return output;
}

float4 PS_ParticleRender(VTX_OUT _in) : SV_Target   //반환타입이 float4인데 렌더타겟에 출력
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    
    return vOutColor;
}

#endif