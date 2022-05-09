#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "struct.fx"
#include "func.fx"


// =======================
// Particle Update Shader
// Material의 상수버퍼를 CS에서도 사용가능
#define MaxThreadCount g_int_0
#define MaxParticleCount g_int_1
// =======================

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);

[numthreads(32, 1, 1)]
void CS_Particle(int3 _id : SV_DispatchThreadID)
{
    if (MaxParticleCount < _id.x) //초과된 Thread는 return
        return;
    
    
    if (ParticleBuffer[_id.x].Alive)
    {
        // Random
        float2 vSampleUV = float2(((float) _id.x / (float) MaxThreadCount) + fAccTime, fAccTime);
        
        float3 vOut = GaussianSample(g_noise_cloud, vSampleUV).rgb;
        vOut = (vOut - 0.5f) * 2.f;
        
        // gaussian 5x5 : 중심점을 기준으로 25개 픽셀을 조사해서 5x5필터에서 중앙으로 갈수록 값이 높아지는 필터
        // 마지막 쓰레드의 UV값은 1 , 처음 쓰레드의 UV값은 0
        
        
        
        float3 vDir = float3(vOut.rg, 0.f);
        vDir = normalize(vDir);
        
        ParticleBuffer[_id.x].vPos += vDir * fDT * 50.f;
        ParticleBuffer[_id.x].vColor = float4(0.f, 1.f, 0.f, 1.f);
    }
    
    
}


#endif

// CS에서 Random사용
// 쓰레드는 병렬처리하기(한번에처리) 때문에 난수를 만들 때 시간으로 seed는 안됨

// noise Texture(TV에서 지지직거리는 화면)
// 특정 픽셀을 찍어서 주변 색상을 추출해서 이 값을 0~1 비율로 줄여서 그 값을 난수 만드는데 사용
// 모든 쓰레드들의 차이점인 DispatchThreadID를 활용하여(이 값이 곧 위치)  샘플링 할 위치값을 다르게 한다.

// R,G,B로 이루어진 노이즈는 R, G, B를 이용해서 난수를 생성
// 누적시간과 uv값을 이용하여 Sample하는 위치값을 다르게 설정한다