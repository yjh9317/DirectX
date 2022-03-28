#ifndef _TEST
#define _TEST

cbuffer TRANSFORM : register(b0)
{
    // row_major : �� �켱���� �б�
    row_major matrix g_matWorld;     //matrix or float4x4 (64����Ʈ) 4x4���� ���
}

// gpu�� �⺻������ ����� �����ö� �� �켱���� �޸𸮸� �����ϱ� ������
// row_major�� �� �켱���� �ٲ��ش�.


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
    
    float4 vFinalPos = mul(float4(_in.vPos, 1.f), g_matWorld);   // 4*4 ��İ� ��ġ�� ���ؼ� ������ġ
    
    // ����(float3)�� 4*4��� �� �����ֱ����� float4�� �ٲ��ָ鼭 ������ ���� ��İ��ؼ� ��ġ���� �������ֱ� ���� 1.f�� ����
   
    
    output.vPosition = vFinalPos;
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