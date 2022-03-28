#ifndef _TEST
#define _TEST

cbuffer TRANSFORM : register(b0)
{
    // row_major : 행 우선으로 읽기
    row_major matrix g_matWorld;     //matrix or float4x4 (64바이트) 4x4형태 행렬
}

// gpu는 기본적으로 행렬을 가져올때 열 우선으로 메모리를 열거하기 때문에
// row_major로 행 우선으로 바꿔준다.


// Texture2D g_tex_0 : register(t0);
// StructuredBuffer<float4> g_buffer : register(t1);
// sampler g_sam : register(s0);
// RWStructuredBuffer<float4> g_rwbuffer : register(u0);




// Vertex Shader
struct VTX_IN
{
    float3 vPos : POSITION; // semantic    
    float4 vColor : COLOR;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
};

VTX_OUT VS_Test(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    float4 vFinalPos = mul(float4(_in.vPos, 1.f), g_matWorld);   // 4*4 행렬과 위치를 곱해서 최종위치
    
    // 정점(float3)을 4*4행렬 로 맞춰주기위해 float4로 바꿔주면서 마지막 값은 행렬곱해서 위치값을 저장해주기 위해 1.f로 세팅
   
    
    output.vPosition = vFinalPos;
    output.vColor = _in.vColor;
    
    return output;
}


// Rasterizer
// 정점이 만드는 도형(Topology) 안에 들어오는 픽셀을 검출 (픽셀 쉐이더 후보)
// 해당 픽셀들 마다 픽셀 쉐이더 호출

float4 PS_Test(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = _in.vColor;
    
    return vOutColor;
}




#endif