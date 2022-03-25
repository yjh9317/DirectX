#ifndef _TEST
#define _TEST

cbuffer TRANSFORM : register(b0)
{
    float4 g_Pos;
}

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
    
    float3 vFinalPos = _in.vPos + g_Pos.xyz;
    
    output.vPosition = float4(vFinalPos, 1.f);
    output.vColor = _in.vColor;
    
    return output;
}


// Rasterizer
// ������ ����� ����(Topology) �ȿ� ������ �ȼ��� ���� (�ȼ� ���̴� �ĺ�)
// �ش� �ȼ��� ���� �ȼ� ���̴� ȣ��


float4 PS_Test(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = _in.vColor;
    
    return vOutColor;
}











#endif