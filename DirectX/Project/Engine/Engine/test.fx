#ifndef _TEST
#define _TEST


cbuffer TRANSFORM : register(b0)
{
    // row_major : �� �켱���� �б�
    row_major matrix g_matWorld; //matrix or float4x4 (64����Ʈ) 4x4���� ���
    row_major matrix g_matView; // �� ���
    row_major matrix g_matProj; // ���� ���
}

// gpu�� �⺻������ ����� �����ö� �� �켱���� �޸𸮸� �����ϱ� ������
// row_major�� �� �켱���� �ٲ��ش�.



cbuffer SCALAR_PARAM : register(b1) //Material�� ��� ��������
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    
    matrix g_mat_0;
    matrix g_mat_1;
    matrix g_mat_2;
    matrix g_mat_3;
}


Texture2D g_tex_0 : register(t0);   
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);

TextureCube g_texcube_0 : register(t4);
TextureCube g_texcube_1 : register(t5);

Texture2DArray g_texarr_0 : register(t6);
Texture2DArray g_texarr_1 : register(t7);


SamplerState g_sam_0 : register(s0);    // Anisotropic Filter �̹漺 ����
SamplerState g_sam_1 : register(s1);    // Point Filter       2D���� ���̾��̴� ����



// Vertex Shader
struct VTX_IN
{
    float3 vPos : POSITION; // semantic : ��ǲ���̾ƿ� �ۼ��ؾߵȴ�.   
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

VTX_OUT VS_Test(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    // Local Mesh �� ��ǥ�� ����� ��ġ
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    // World ��ǥ�� ī�޶� �����̽��� �̵�
    float4 vViewPos = mul(vWorldPos, g_matView);
    
    // ī�޶� �����̽� ��ǥ�� ������ǥ�� �̵�
    float4 vProjPos = mul(vViewPos, g_matProj);
        
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

// Rasterizer
// ������ ����� ����(Topology) �ȿ� ������ �ȼ��� ���� (�ȼ� ���̴� �ĺ�)
// �ش� �ȼ��� ���� �ȼ� ���̴� ȣ��

float4 PS_Test(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    //vOutColor = _in.vColor;
    
    //if (g_int_0)
    //    vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    //else
    //    vOutColor = float4(0.f, 0.f, 1.f, 1.f);
    
    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);  //�����ϴµ� ���÷� ������Ʈ�� �ʿ�

    return vOutColor;
}





#endif