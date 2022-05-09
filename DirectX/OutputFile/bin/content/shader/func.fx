#ifndef _FUNC
#define _FUNC

#include "value.fx"

//HLSL에서도 전역변수를 선언할때는 static을 사용

static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};



int IsBind(in Texture2D _tex)
{
    uint width = 0;
    uint height = 0;
    _tex.GetDimensions(width, height);
    
    if (0 != width || 0 != height)
        return 1;
   
    return 0;
}



float4 GaussianSample(in Texture2D _noisetex, float2 _vUV)
{
    float4 vOutColor = (float4) 0.f;
    
    if(0.f < _vUV.x)
    {
        _vUV.x = frac(_vUV.x);
    }
    
    if (0.f < _vUV.y) 
    {
        _vUV.y = frac(_vUV.y);
    }
    
    // NoiseTexture 해상도를 이용해서 픽셀 인덱스(정수)를 알아낸다.
    int2 iPixelIdx = (int2) (vNoiseResolution * _vUV);
    iPixelIdx -= int2(2, 2); // 좌상단 픽셀 위치값을 알아냄.
    
    // 해당 픽셀을 중심으로 Gaussain Filter를 적용해서 색상의 평균치를 계산한다.
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            //_noisetex값에서 나온 색상과 Ga ussain Filter의 값을 곱한다.
            int2 idx = int2(iPixelIdx.x + j, iPixelIdx.y + i); 
            vOutColor += _noisetex[idx] * GaussianFilter[j][i]; //HLSL에서는 1차원배열 형식으로만 사용
            
            // 모든 픽셀(5x5)에 Gaussain Filter를 곱해서 주변 색상의 평균치값을 구함.
        }
    }
    
    return vOutColor;
}


#endif