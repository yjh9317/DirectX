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
    uint InstID : SV_InstanceID;
};

//SV_Position : Rasterizer직전에 줄 때 사용하는 시멘틱, 마지막 정점 세이더에서 줘야한다.
struct VTX_OUT
{
    uint InstID : SV_InstanceID; // 호출된 정점의 순서(인덱스)를 저장해주는 시멘틱
    // RectMesh의 정점 6개에 처음에는 모두 0이 들어오고 그 다음 정점6개에는 1이 들어오고 이런식으로 증가
};


VTX_OUT VS_ParticleRender(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    
    output.InstID = _in.InstID;
    
    return output;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint InstID : SV_InstanceID;
};

// 1개의 정점을 6개로 분할 (RectMesh처럼 점은 4개지만 6번 접근)

// point : 점 하나를 의미
// inout == reference를 의미
// TriangleStream<GS_OUT> : vector 처럼 사용
// _output = 정점을 받을 변수
[maxvertexcount(6)] // 6번에 걸쳐서 삼각형 면 두개를 생성해서 사각형을 찍으므로 6
void GS_ParticleRender(point VTX_OUT _in[1], inout TriangleStream<GS_OUT> _output)
{    
    uint ID = _in[0].InstID;
    
    // 해당 인스턴스가 비활성화 상태면, 출력 정점이 없다 ==> 아무일도 없다. PS까지 가서 discard하지도 않음
    if (0 == ParticleBuffer[ID].Alive)
        return;

        
    // 파티클 정점의 월드위치 구하기
    float3 vParticleWorldPos = (float3) 0.f;
    
    
    if (POS_INHERIT)
    {
        // 오브젝트(부모)의 위치에 상대적이므로 오브젝트 위치행렬까지 곱
        vParticleWorldPos = g_matWorld._41_42_43  + ParticleBuffer[ID].vPos; // 부모 정점의 위치에 자기 자신(입자)의 위치 계산
    }
    else
    {
        vParticleWorldPos = ParticleBuffer[ID].vPos;    // 상속받지 않으므로 자기 자신의 위치
    }
    
    // World 좌표를 View, Proj 으로 변환
    // Bilboard(입자가 카메라를 바라보지 않는 문제)해결을 위해 ViewSpace가서 분할
    float4 vViewPos = mul(float4(vParticleWorldPos, 1.f), g_matView);
    float3 vScale = ParticleBuffer[ID].vScale;
    
    // ViewSpace 에서 Point를 Rect로 분할
    // 0 --- 1
    // |  \  |
    // 3 --- 2
    float3 arrRect[4] =
    {
        // 분할한 정점의 위치를 계산
        float3(vViewPos.x - vScale.x / 2.f, vViewPos.y + vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x + vScale.x / 2.f, vViewPos.y + vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x + vScale.x / 2.f, vViewPos.y - vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x - vScale.x / 2.f, vViewPos.y - vScale.y / 2.f, vViewPos.z)
    };
    
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f};
    
    for (int i = 0; i < 4; ++i)
    {
        // arrRect는 ViewSpace의 vPos를 가지고 있으므로 투영 행렬까지 계산후 output에 넣어준다.
            output[i].vPosition = mul(float4(arrRect[i], 1.f), g_matProj);
            output[i].InstID = _in[0].InstID;
    }
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    
     //Append는 pushback과 동일
    _output.Append(output[0]);
    _output.Append(output[2]);
    _output.Append(output[3]);
    _output.RestartStrip(); // 여기까지가 면 하나라고 알려주는 함수 (0,2,3으로 삼각형을 이룬다)
    
    _output.Append(output[2]);
    _output.Append(output[0]);
    _output.Append(output[1]);
    _output.RestartStrip();
}

float4 PS_ParticleRender(GS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor;
    }
    else
    {
        vOutColor = ParticleBuffer[_in.InstID].vColor;
    }
    
    //해당 인스턴스 아이디값의 컬러
    return vOutColor;
}



/*
문제점

1.비활성화(Alive가 false)된 파티클은 픽셀 세이더까지 가서 픽셀 하나하나가 변수 Alive의 값으로 활성화 체크를 하면서
파티클을 비활성화(discard) 할 수 있지만 픽셀을 하나하나 검사하는거부터 비용이 든다.
2.3D에서 입자를 바라보았을 때 입자가 카메라를 항상 바라보도록 설정하지 않았기 때문에 옆에서 보면 입자가 비스듬하게 보인다.

이 두 문제점을 해결하기 위해 Geometry Shader를 이용한다.
Geometry의 특징 :한 정점을 분할하여 입력스트림을 안에 받는다.

해결방법
1. 한 정점을 분할하고 그 분할된 정점들을 입력스트림에 넣어야 다음단계까지 가지만 넣지 않으면 다음단계를 가지 않는다. 이걸로 픽셀 셰이더 이전에 끝냄.
2. 정점을 View Space에서 분할하면 카메라의 시점에서 분할되어 분할된 정점들이 카메라를 향해 바라본다.
*/

#endif