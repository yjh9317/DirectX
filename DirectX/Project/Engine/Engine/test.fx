#ifndef _TEST
#define _TEST


cbuffer TRANSFORM : register(b0)
{
    // row_major : 행 우선으로 읽기
    row_major matrix g_matWorld; //matrix or float4x4 (64바이트) 4x4형태 행렬
    row_major matrix g_matView; // 뷰 행렬
    row_major matrix g_matProj; // 투영 행렬
}

// gpu는 기본적으로 행렬을 가져올때 열 우선으로 메모리를 열거하기 때문에
// row_major로 행 우선으로 바꿔준다.



cbuffer SCALAR_PARAM : register(b1) //Material의 상수 레지스터
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
    
    // Local Mesh 의 좌표를 월드로 배치
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    // World 좌표를 카메라 스페이스로 이동
    float4 vViewPos = mul(vWorldPos, g_matView);
    
    // 카메라 스페이스 좌표를 투영좌표로 이동
    float4 vProjPos = mul(vViewPos, g_matProj);
        
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;
    
    return output;
}

// Rasterizer
// 정점이 만드는 도형(Topology) 안에 들어오는 픽셀을 검출 (픽셀 쉐이더 후보)
// 해당 픽셀들 마다 픽셀 쉐이더 호출

float4 PS_Test(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    //vOutColor = _in.vColor;
    
    if (g_int_0)
        vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    else
        vOutColor = float4(0.f, 0.f, 1.f, 1.f);
    
    return vOutColor;
}









#endif