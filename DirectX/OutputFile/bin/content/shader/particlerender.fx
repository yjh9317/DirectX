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

//SV_Position : Rasterizer������ �� �� ����ϴ� �ø�ƽ, ������ ���� ���̴����� ����Ѵ�.
struct VTX_OUT
{
    uint InstID : SV_InstanceID; // ȣ��� ������ ����(�ε���)�� �������ִ� �ø�ƽ
    // RectMesh�� ���� 6���� ó������ ��� 0�� ������ �� ���� ����6������ 1�� ������ �̷������� ����
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

// 1���� ������ 6���� ���� (RectMeshó�� ���� 4������ 6�� ����)

// point : �� �ϳ��� �ǹ�
// inout == reference�� �ǹ�
// TriangleStream<GS_OUT> : vector ó�� ���
// _output = ������ ���� ����
[maxvertexcount(6)] // 6���� ���ļ� �ﰢ�� �� �ΰ��� �����ؼ� �簢���� �����Ƿ� 6
void GS_ParticleRender(point VTX_OUT _in[1], inout TriangleStream<GS_OUT> _output)
{    
    uint ID = _in[0].InstID;
    
    // �ش� �ν��Ͻ��� ��Ȱ��ȭ ���¸�, ��� ������ ���� ==> �ƹ��ϵ� ����. PS���� ���� discard������ ����
    if (0 == ParticleBuffer[ID].Alive)
        return;

        
    // ��ƼŬ ������ ������ġ ���ϱ�
    float3 vParticleWorldPos = (float3) 0.f;
    
    
    if (POS_INHERIT)
    {
        // ������Ʈ(�θ�)�� ��ġ�� ������̹Ƿ� ������Ʈ ��ġ��ı��� ��
        vParticleWorldPos = g_matWorld._41_42_43  + ParticleBuffer[ID].vPos; // �θ� ������ ��ġ�� �ڱ� �ڽ�(����)�� ��ġ ���
    }
    else
    {
        vParticleWorldPos = ParticleBuffer[ID].vPos;    // ��ӹ��� �����Ƿ� �ڱ� �ڽ��� ��ġ
    }
    
    // World ��ǥ�� View, Proj ���� ��ȯ
    // Bilboard(���ڰ� ī�޶� �ٶ��� �ʴ� ����)�ذ��� ���� ViewSpace���� ����
    float4 vViewPos = mul(float4(vParticleWorldPos, 1.f), g_matView);
    float3 vScale = ParticleBuffer[ID].vScale;
    
    // ViewSpace ���� Point�� Rect�� ����
    // 0 --- 1
    // |  \  |
    // 3 --- 2
    float3 arrRect[4] =
    {
        // ������ ������ ��ġ�� ���
        float3(vViewPos.x - vScale.x / 2.f, vViewPos.y + vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x + vScale.x / 2.f, vViewPos.y + vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x + vScale.x / 2.f, vViewPos.y - vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x - vScale.x / 2.f, vViewPos.y - vScale.y / 2.f, vViewPos.z)
    };
    
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f};
    
    for (int i = 0; i < 4; ++i)
    {
        // arrRect�� ViewSpace�� vPos�� ������ �����Ƿ� ���� ��ı��� ����� output�� �־��ش�.
            output[i].vPosition = mul(float4(arrRect[i], 1.f), g_matProj);
            output[i].InstID = _in[0].InstID;
    }
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    
     //Append�� pushback�� ����
    _output.Append(output[0]);
    _output.Append(output[2]);
    _output.Append(output[3]);
    _output.RestartStrip(); // ��������� �� �ϳ���� �˷��ִ� �Լ� (0,2,3���� �ﰢ���� �̷��)
    
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
    
    //�ش� �ν��Ͻ� ���̵��� �÷�
    return vOutColor;
}



/*
������

1.��Ȱ��ȭ(Alive�� false)�� ��ƼŬ�� �ȼ� ���̴����� ���� �ȼ� �ϳ��ϳ��� ���� Alive�� ������ Ȱ��ȭ üũ�� �ϸ鼭
��ƼŬ�� ��Ȱ��ȭ(discard) �� �� ������ �ȼ��� �ϳ��ϳ� �˻��ϴ°ź��� ����� ���.
2.3D���� ���ڸ� �ٶ󺸾��� �� ���ڰ� ī�޶� �׻� �ٶ󺸵��� �������� �ʾұ� ������ ������ ���� ���ڰ� �񽺵��ϰ� ���δ�.

�� �� �������� �ذ��ϱ� ���� Geometry Shader�� �̿��Ѵ�.
Geometry�� Ư¡ :�� ������ �����Ͽ� �Է½�Ʈ���� �ȿ� �޴´�.

�ذ���
1. �� ������ �����ϰ� �� ���ҵ� �������� �Է½�Ʈ���� �־�� �����ܰ���� ������ ���� ������ �����ܰ踦 ���� �ʴ´�. �̰ɷ� �ȼ� ���̴� ������ ����.
2. ������ View Space���� �����ϸ� ī�޶��� �������� ���ҵǾ� ���ҵ� �������� ī�޶� ���� �ٶ󺻴�.
*/

#endif