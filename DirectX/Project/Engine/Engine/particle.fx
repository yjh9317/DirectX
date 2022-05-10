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
#define WorldPos            g_vec4_0.xyz
// =======================

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<tParticleData> DataBuffer : register(u1);
[numthreads(32, 1, 1)]
void CS_Particle(int3 _id : SV_DispatchThreadID)
{
    if (MaxParticleCount < _id.x) //초과된 Thread는 return
        return;
    
    // 모든 쓰레드들이 병렬처리로 동시처리하려하지만, 먼저 호출된 쓰레드에서 통과한 애들만 쓰레드 동기화 함수를 호출
    
    // 동작 방식: 모든 쓰레드는 병렬처리를 하기 때문에 InterlockedExchange가 들어가기 직전까지 위 세 변수들은 값이 같다
    // 그리고나서 첫번째 쓰레드가 호출되면 DataBuffer[0].iAliveCount는 1에서 0으로 바뀌고
    // 그다음 쓰레드부터는 첫번째 쓰레드가 호출되고 나서 DataBuffer[0].iAliveCount는 0이 되면서 if문의 조건에 맞지 않게 된다.

    
    int iOriginValue = DataBuffer[0].iAliveCount;       // 교체되기 전의 원본값
    int iInputValue = DataBuffer[0].iAliveCount - 1;    // 목적지와 바꿀 교체할 값
    int iOutValue = 0;                                  // 목적지의 값을 받아옴
    
    InterlockedExchange(DataBuffer[0].iAliveCount, iInputValue, iOutValue);
    
    if (iOriginValue == iOutValue)
    {
        ParticleBuffer[_id.x].Alive = 1;
        
        // 랜덤한 위치 부여        
        float2 vSampleUV = float2(((float) _id.x / (float) MaxThreadCount) + fAccTime, fAccTime);
        vSampleUV.y += sin(vSampleUV.x * 3.141592f * 2.f) * 0.5f;
        float3 vOut = GaussianSample(g_noise_cloud, vSampleUV).rgb;
        
        
        ParticleBuffer[_id.x].vPos = WorldPos + vOut * 100.f;

    }
    
    
    //if (1 == DataBuffer[0].iAliveCount)
    //{
    //    ParticleBuffer[_id.x].Alive = 1;
    //    ParticleBuffer[_id.x].vScale = float3(50.f, 50.f, 1.f);
    //    ParticleBuffer[_id.x].vPos = float3(0.f, 0.f, 0.f);
        
        
    //}
    
    //if (ParticleBuffer[_id.x].Alive)
    //{
    //    // Random
    //    float2 vSampleUV = float2(((float) _id.x / (float) MaxThreadCount) + fAccTime, fAccTime);
        
    //    float3 vOut = GaussianSample(g_noise_cloud, vSampleUV).rgb;        
    //    vOut = (vOut - 0.5f) * 2.f;
                
    //    float3 vDir = float3(vOut.rg, 0.f);
    //    vDir = normalize(vDir);        
        
    //    ParticleBuffer[_id.x].vPos += vDir * fDT * 50.f ;        
    //    ParticleBuffer[_id.x].vColor = float4(0.f, 1.f, 0.f, 1.f);
    //}    
}
    
    


#endif

// gaussian 5x5 : 중심점을 기준으로 25개 픽셀을 조사해서 5x5필터에서 중앙으로 갈수록 값이 높아지는 필터
// 마지막 쓰레드의 UV값은 1 , 처음 쓰레드의 UV값은 0

// CS에서 Random사용
// 쓰레드는 병렬처리하기(한번에처리) 때문에 난수를 만들 때 시간으로 seed는 안됨

// noise Texture(TV에서 지지직거리는 화면)
// 특정 픽셀을 찍어서 주변 색상을 추출해서 이 값을 0~1 비율로 줄여서(UV값으로 변환) 그 값을 난수 만드는데 사용
// 모든 쓰레드들의 차이점인 DispatchThreadID를 활용하여(이 값이 곧 위치)  샘플링 할 위치값을 다르게 한다.

// R,G,B로 이루어진 노이즈는 R, G, B를 이용해서 난수를 생성
// 누적시간과 uv값을 이용하여 Sample하는 위치값을 다르게 설정한다



// 쓰레드 동기화 함수에 대한 링크
// https://docs.microsoft.com/en-us/windows/win32/direct3d11/direct3d-11-advanced-stages-cs-atomic-functions

// 쓰레드 동기화 함수들이 동작하는 방식
// InterlockedExchange(목적지, 변경하고자하는 value값, 바뀐 원래 값)
