#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "struct.fx"
#include "func.fx"


// =======================
// Particle Update Shader
// Material�� ������۸� CS������ ��밡��
#define MaxThreadCount g_int_0
#define MaxParticleCount g_int_1
#define WorldPos            g_vec4_0.xyz
// =======================

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<tParticleData> DataBuffer : register(u1);
[numthreads(32, 1, 1)]
void CS_Particle(int3 _id : SV_DispatchThreadID)
{
    if (MaxParticleCount < _id.x) //�ʰ��� Thread�� return
        return;
    
    // ��� ��������� ����ó���� ����ó���Ϸ�������, ���� ȣ��� �����忡�� ����� �ֵ鸸 ������ ����ȭ �Լ��� ȣ��
    
    // ���� ���: ��� ������� ����ó���� �ϱ� ������ InterlockedExchange�� ���� �������� �� �� �������� ���� ����
    // �׸����� ù��° �����尡 ȣ��Ǹ� DataBuffer[0].iAliveCount�� 1���� 0���� �ٲ��
    // �״��� ��������ʹ� ù��° �����尡 ȣ��ǰ� ���� DataBuffer[0].iAliveCount�� 0�� �Ǹ鼭 if���� ���ǿ� ���� �ʰ� �ȴ�.

    
    int iOriginValue = DataBuffer[0].iAliveCount;       // ��ü�Ǳ� ���� ������
    int iInputValue = DataBuffer[0].iAliveCount - 1;    // �������� �ٲ� ��ü�� ��
    int iOutValue = 0;                                  // �������� ���� �޾ƿ�
    
    InterlockedExchange(DataBuffer[0].iAliveCount, iInputValue, iOutValue);
    
    if (iOriginValue == iOutValue)
    {
        ParticleBuffer[_id.x].Alive = 1;
        
        // ������ ��ġ �ο�        
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

// gaussian 5x5 : �߽����� �������� 25�� �ȼ��� �����ؼ� 5x5���Ϳ��� �߾����� ������ ���� �������� ����
// ������ �������� UV���� 1 , ó�� �������� UV���� 0

// CS���� Random���
// ������� ����ó���ϱ�(�ѹ���ó��) ������ ������ ���� �� �ð����� seed�� �ȵ�

// noise Texture(TV���� �������Ÿ��� ȭ��)
// Ư�� �ȼ��� �� �ֺ� ������ �����ؼ� �� ���� 0~1 ������ �ٿ���(UV������ ��ȯ) �� ���� ���� ����µ� ���
// ��� ��������� �������� DispatchThreadID�� Ȱ���Ͽ�(�� ���� �� ��ġ)  ���ø� �� ��ġ���� �ٸ��� �Ѵ�.

// R,G,B�� �̷���� ������� R, G, B�� �̿��ؼ� ������ ����
// �����ð��� uv���� �̿��Ͽ� Sample�ϴ� ��ġ���� �ٸ��� �����Ѵ�



// ������ ����ȭ �Լ��� ���� ��ũ
// https://docs.microsoft.com/en-us/windows/win32/direct3d11/direct3d-11-advanced-stages-cs-atomic-functions

// ������ ����ȭ �Լ����� �����ϴ� ���
// InterlockedExchange(������, �����ϰ����ϴ� value��, �ٲ� ���� ��)
