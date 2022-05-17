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
    
    // Animation 정보가 있는 경우
    if (g_useAnim2D)
    {
        
        float2 vUV = _in.vUV * g_vBackgroundSize; // Background 픽셀의 UV
        vUV = vUV - (g_vBackgroundSize - g_vSlice) / 2.f + g_vLT - g_vOffset;
        
        // g_vBackgroundSize - g_vSlice /2.f는 사용하려는 이미지를 가운데로 놓기위해
        // Background를 vLT에서 시작하게 하는게 아니라 왼쪽으로 이동시킴.-> - (g_vBackgroundSize - g_vSlice) / 2.f
        
        // 오른쪽으로 이동하려고 하면 아틀라스에서 가져오는 Background를 왼쪽으로 옮겨야 그 안에 있는 캐릭터는
        // 캐릭터는 Background안에서 오른쪽으로 이동하므로 오프셋을 마이너스 해줘야 한다.
        
        
        if (vUV.x < g_vLT.x || g_vLT.x + g_vSlice.x < vUV.x
            || vUV.y < g_vLT.y || g_vLT.y + g_vSlice.y < vUV.y)
        {
            // 캐릭터의 UV밖에 있으면 discard 처분
            discard;
        }
        
        vOutColor = g_Atlas.Sample(g_sam_1, vUV); // Minmap
    }
    else
    {
        // 없으면 Material를 이용
        if (g_btex_0)
        {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
        else
        {
            vOutColor = float4(1.f, 0.f, 1.f, 1.f);
        }
    
    }
        // uint width = 0, height = 0;
        // g_tex_0.GetDimensions(width, height);
        // GetDimensions : 해당 텍스쳐의 가로세로 정보를 가져오는 함수
        // 하지만 좋은 방법은 아님. 모든 픽셀에 대해서 검사도 해야하기 때문에 비용이 듬.
        
    
    if (vOutColor.a <= 0.f)
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
