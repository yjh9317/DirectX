#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"

// ==============
// TileMap Shader
// Domain : Opaque
// Blend : Default
// Depth : LESS
#define LeftTopUV   g_vec2_0
#define SliceSizeUV g_vec2_1
// ==============

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

VTX_OUT VS_TileMap(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_TileMap(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    float2 vUV = _in.vUV * SliceSizeUV + LeftTopUV;
    
    vOutColor = g_tex_0.Sample(g_sam_1, vUV);
    
    return vOutColor;
}








#endif