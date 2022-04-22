#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"

// ==============
// TileMap Shader
// Domain : Mask
// Blend : Default
// Depth : LESS

#define TileCountX g_int_0
#define TileCountY g_int_1

#define SliceSizeUV g_vec2_0


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
    
    // vUV�� �޽��� ��ü�������� 0 ~ 1�� �޾ƿ��� NxMŸ�� ũ��� Ȯ�� (N=4, M=4�� 0,0 ���� 4,4)
    float2 vUV = _in.vUV * float2(TileCountX, TileCountY);
    
    // �� Ÿ�� ��ġ�� ���������� ������ ���ؼ� �������� ���Ѵ�.
    // �ش� �ȼ��� ��ġ�ϴ� Ÿ���� ��������� �˾Ƴ� �� �ִ�.
    int2 iTileRowCol = floor(vUV); //floor() : �Ǽ������� �Ҽ����� ���� ���� ���´�
    
    // ���� Ÿ�� ��� ������ ������ �ε����� ��ȯ(Ÿ�� ������ �迭�� �����ϱ� ����)
    int TileDataIdx = iTileRowCol.y * TileCountX + iTileRowCol.x;
    
    // ���� �̹��� ������ �ȵ� Ÿ���̸� ����
    if (-1 == arrTileData[TileDataIdx].iImgIdx)   
        discard;
    
    // Ÿ�� �����Ϳ� �����ϸ� , �� Ÿ�� ���� �»�� UV���� ����ִ�.
     float2 vLeftTopUV = arrTileData[TileDataIdx].vLTUV;
    
    // 0~1 �� UV �� NxM Ÿ�� ũ��� Ȯ�� UV�� �Ҽ��� �κ��� ���ø� �뵵�� ����Ѵ�.
    float2 vImgUV = frac(vUV); // frac() : �Ǽ������� �������� �� �Ҽ������� ��´�
    
    // ���� Sample ��ġ UV�� ����Ѵ�
    float2 vSampleUI = vLeftTopUV + vImgUV * SliceSizeUV;
    
    vOutColor = g_tex_0.Sample(g_sam_1, vSampleUI);
    
    return vOutColor;
}








#endif