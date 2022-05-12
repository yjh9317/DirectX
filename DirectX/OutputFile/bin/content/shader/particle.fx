#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "struct.fx"
#include "func.fx"


// =======================
// Particle Update Shader
// Material의 상수버퍼를 CS에서도 사용가능
#define MaxThreadCount      g_int_0
#define MaxParticleCount    g_int_1

#define CreateRange         g_float_0
#define WorldPos            float3(g_float_1, g_float_2, g_float_3)

#define MinLifeTime         g_vec2_0.x
#define MaxLifeTime         g_vec2_0.y

#define MinSpeed            g_vec2_1.x
#define MaxSpeed            g_vec2_1.y

#define StartColor          g_vec4_0
#define EndColor            g_vec4_1
#define StartScale          g_vec4_2
#define EndScale            g_vec4_3

// =======================

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<tParticleData> DataBuffer : register(u1);

[numthreads(32, 1, 1)]
void CS_Particle(int3 _id : SV_DispatchThreadID)
{
    if (MaxParticleCount < _id.x) //초과된 Thread는 return
        return;
    
    // 모든 쓰레드들이 병렬처리로 동시처리하려하지만, 먼저 호출된 쓰레드에서 통과한 애들만 쓰레드 동기화 함수를 호출
    
    // InterlockedExhange 동작 방식: 모든 쓰레드는 병렬처리를 하기 때문에 InterlockedExchange가 들어가기 직전까지 위 세 변수들은 값이 같다
    // 그리고나서 첫번째 쓰레드가 호출되면 DataBuffer[0].iAliveCount는 1에서 0으로 바뀌고
    // 그다음 쓰레드부터는 첫번째 쓰레드가 호출되고 나서 DataBuffer[0].iAliveCount는 0이 되면서 if문의 조건에 맞지 않게 된다.

      // 파티클이 비활성화 되어있다면...
    if (0 == ParticleBuffer[_id.x].Alive)
    {
        int iOriginValue = DataBuffer[0].iAliveCount; // 교체되기 전의 원본값
        
        
        // Alive 기회가 있으면 반복
        while(0 < iOriginValue)
        {
            
            int iInputValue = iOriginValue - 1; // 목적지와 바꿀 교체할 값
            int iOutValue = 0; // 목적지의 값을 받아옴
    
    
            //InterlockedExchange(DataBuffer[0].iAliveCount, iInputValue, iOutValue);
            InterlockedCompareExchange(DataBuffer[0].iAliveCount, iOriginValue, iInputValue, iOutValue);
    
        // 활성화 성공한 경우 , Alive를 True로 변경
            if (iOriginValue == iOutValue)
            {
                ParticleBuffer[_id.x].Alive = 1;
                break;
            }
            
            iOriginValue = DataBuffer[0].iAliveCount;
 
            // 처음에 성공한 쓰레드가 iOriginValue를 -1을 줄이고 다음 쓰레드가 들어가기전에 iOriginValue값을 현재  DataBuffer[0].iAliveCount값으로 갱신
            
            
            // 여러개의 쓰레드가 순서를 정하지 않고 InetrlocekdExchange로 한 쓰레드가 호출(랜덤으로 진입)
            // 그 다음 남은 쓰레드들중에서 호출되는데 순서없이 진입하기 때문에 호출됐던 쓰레드가 다시 호출되고 호출되지 않는 쓰레드들은 다시 호출되지 않을 수도 있다

            // 이 문제를 해결하기 위해 InetrlockedCompareExchange함수로 원본값과 비교하여 성공하면 Alive
            
        }
        
        
        // 활성화 된 파티클의 초기값을 랜덤하게 부여
        if(ParticleBuffer[_id.x].Alive)
        {
            // 랜덤한 위치 부여
            float2 vSampleUV = float2(((float) _id.x / (float) MaxThreadCount) + fAccTime, fAccTime);
            vSampleUV.y += sin(vSampleUV.x * 3.141592f * 2.f) * 0.5f;
            float3 vOut = GaussianSample(g_noise_cloud, vSampleUV).rgb;
                        
            float3 vRandom =
            {
                GaussianSample(g_noise_01, vSampleUV + float2(0.1f, 0.f)).r
                , GaussianSample(g_noise_01, vSampleUV + float2(0.2f, 0.f)).r
                , GaussianSample(g_noise_01, vSampleUV + float2(0.3f, 0.f)).r
            };
                        
            
            ParticleBuffer[_id.x].vPos = WorldPos + (vOut - 0.5f) * CreateRange;
            ParticleBuffer[_id.x].vScale = float3(10.f, 10.f, 1.f);
            ParticleBuffer[_id.x].vDir = normalize(vRandom - 0.5f);
                        
            ParticleBuffer[_id.x].m_fCurTime = 0.f;
            ParticleBuffer[_id.x].m_fMaxTime = MinLifeTime + (MaxLifeTime - MinLifeTime) * vOut.r;
        }
    }
    // 활성화 된 파티클
    else
    {
        // 생존시간 누적
        ParticleBuffer[_id.x].m_fCurTime += fDT;
        
        // 최대 생존시간을 넘어선 경우
        if (ParticleBuffer[_id.x].m_fMaxTime <= ParticleBuffer[_id.x].m_fCurTime)
        {
            ParticleBuffer[_id.x].Alive = 0;
            return;
        }
                
        float fLifeRatio = ParticleBuffer[_id.x].m_fCurTime / ParticleBuffer[_id.x].m_fMaxTime;
        
        float fSpeed = MinSpeed + (MaxSpeed - MinSpeed) * fLifeRatio;
        float4 vColor = StartColor + (EndColor - StartColor) * fLifeRatio;
        float3 vScale = StartScale + (EndScale - StartScale) * fLifeRatio;
        
        
        ParticleBuffer[_id.x].vPos += ParticleBuffer[_id.x].vDir * fSpeed * fDT;
        ParticleBuffer[_id.x].vColor = vColor;
        ParticleBuffer[_id.x].vScale = vScale;
    }
    
   
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
