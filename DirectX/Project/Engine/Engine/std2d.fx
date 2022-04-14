#ifndef _STD2D
#define _STD2D

#include "value.fx"


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


// =========================
// Std2D Shader
// g_float_0 : Mask Limit
// g_tex_0 : Output Texture
// Rasterizer : CULL_NONE
// BlendState : Default
// DepthStencilState : LESS
// DOMAIN : Masked
// =========================
VTX_OUT VS_Std2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    if (vOutColor.a <= g_float_0)
    {
        discard;
    }
    
    return vOutColor;
}

// ========================
// Std2DAlphaBlend
// BlendState           : Alpha Blend
// DepthStencilState    : No_Write
// DOMAIN               : OPAQUE
//=========================
VTX_OUT VS_Std2DAlpha(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2DAlpha(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
       
    return vOutColor;
}













// ==================
// Collider2D Shader
// g_int_0 : Collision
// ==================
VTX_OUT VS_Collider2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_Collider2D(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
       
    if (g_int_0)
    {
        vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    }
    else
    {
        vOutColor = float4(0.f, 1.f, 0.f, 1.f);
    }
    
    // 보간된 내부 정점의 값을 이용해서 (범위 (0,0) ~ (1,1)) 충돌체의 크기를 조절가능
    // 재질을 통해서 값을 전달받아서 조절도 가능함.
    // 퍼센트라 충돌체가 커지면 그만큼 커져서 해상도,크기등 미리 값을 알고 있어야 정확히 사용가능
    
    //if (0.1f <= _in.vUV.x && _in.vUV.x <= 0.9f && 0.1f <= _in.vUV.y && _in.vUV.y <= 0.9f)
    //{
    //    discard;
    //}
    
    //discard; : 모든 픽셀을 버린다, 파이프라인가다가 discard가 나올 때 중단된다.
    //clip(-1) == discard
        
    return vOutColor;
}







#endif
