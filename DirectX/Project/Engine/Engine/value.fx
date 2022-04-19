#ifndef _VALUE
#define _VALUE

cbuffer TRANSFORM : register(b0)
{
    // row_major : 행 우선으로 읽기
    row_major matrix g_matWorld; //matrix or float4x4 (64바이트) 4x4
    row_major matrix g_matView; // 뷰 행렬
    row_major matrix g_matProj; // 투영 행렬
    

    row_major matrix g_matWV;
    row_major matrix g_matWVP;
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

cbuffer ANIM2D : register(b2) //Animation의 상수 레지스터
{
    int     g_useAnim2D;         //애니메이션의 사용여부
    float   g_Atlas_Width;
    float   g_Atlas_Height;
    
    float2 g_vLT;       // 아틀라스에서 좌상단 UV 좌표
    float2 g_vSlice;    // 자를 사이즈
    float2 g_BaseSize;
    
    float3 g_Anim2D_Padding;   
}


// Material를 통해서 전달받는 텍스쳐 레지스터
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);

TextureCube g_texcube_0 : register(t6);
TextureCube g_texcube_1 : register(t7);

Texture2DArray g_texarr_0 : register(t8);
Texture2DArray g_texarr_1 : register(t9);

// 애니메이션
Texture2D g_Atlas : register(t10);



SamplerState g_sam_0 : register(s0); // Anisotropic Filter
SamplerState g_sam_1 : register(s1); // Point Filter

#endif
