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
    
    // vUV는 메쉬를 전체기준으로 0 ~ 1을 받아오고 NxM타일 크기로 확장 (N=4, M=4면 0,0 에서 4,4)
    float2 vUV = _in.vUV * float2(TileCountX, TileCountY);
    
    // 각 타일 위치를 정수단위로 나누기 위해서 정수값을 취한다.
    // 해당 픽셀이 위치하는 타일의 행렬정보를 알아낼 수 있다.
    int2 iTileRowCol = floor(vUV); //floor() : 실수값에서 소수점을 버린 값이 나온다
    
    // 얻은 타일 행렬 정보를 일차원 인덱스로 변환(타일 데이터 배열에 접근하기 위함)
    int TileDataIdx = iTileRowCol.y * TileCountX + iTileRowCol.x;
    
    // 만약 이미지 설정이 안된 타일이면 버림
    if (-1 == arrTileData[TileDataIdx].iImgIdx)   
        discard;
    
    // 타일 데이터에 접근하면 , 각 타일 별로 좌상단 UV값이 들어있다.
     float2 vLeftTopUV = arrTileData[TileDataIdx].vLTUV;
    
    // 0~1 의 UV 를 NxM 타일 크기로 확장 UV의 소수점 부분을 샘플링 용도로 사용한다.
    float2 vImgUV = frac(vUV); // frac() : 실수값에서 정수값을 뺀 소수점값을 얻는다
    
    // 최종 Sample 위치 UV를 계산한다
    float2 vSampleUI = vLeftTopUV + vImgUV * SliceSizeUV;
    
    vOutColor = g_tex_0.Sample(g_sam_1, vSampleUI);
    
    return vOutColor;
}








#endif