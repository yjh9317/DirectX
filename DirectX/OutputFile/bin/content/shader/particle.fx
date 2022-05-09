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
// =======================

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);

[numthreads(32, 1, 1)]
void CS_Particle(int3 _id : SV_DispatchThreadID)
{
    if (MaxParticleCount < _id.x) //�ʰ��� Thread�� return
        return;
    
    
    if (ParticleBuffer[_id.x].Alive)
    {
        // Random
        float2 vSampleUV = float2(((float) _id.x / (float) MaxThreadCount) + fAccTime, fAccTime);
        
        float3 vOut = GaussianSample(g_noise_cloud, vSampleUV).rgb;
        vOut = (vOut - 0.5f) * 2.f;
        
        // gaussian 5x5 : �߽����� �������� 25�� �ȼ��� �����ؼ� 5x5���Ϳ��� �߾����� ������ ���� �������� ����
        // ������ �������� UV���� 1 , ó�� �������� UV���� 0
        
        
        
        float3 vDir = float3(vOut.rg, 0.f);
        vDir = normalize(vDir);
        
        ParticleBuffer[_id.x].vPos += vDir * fDT * 50.f;
        ParticleBuffer[_id.x].vColor = float4(0.f, 1.f, 0.f, 1.f);
    }
    
    
}


#endif

// CS���� Random���
// ������� ����ó���ϱ�(�ѹ���ó��) ������ ������ ���� �� �ð����� seed�� �ȵ�

// noise Texture(TV���� �������Ÿ��� ȭ��)
// Ư�� �ȼ��� �� �ֺ� ������ �����ؼ� �� ���� 0~1 ������ �ٿ��� �� ���� ���� ����µ� ���
// ��� ��������� �������� DispatchThreadID�� Ȱ���Ͽ�(�� ���� �� ��ġ)  ���ø� �� ��ġ���� �ٸ��� �Ѵ�.

// R,G,B�� �̷���� ������� R, G, B�� �̿��ؼ� ������ ����
// �����ð��� uv���� �̿��Ͽ� Sample�ϴ� ��ġ���� �ٸ��� �����Ѵ�