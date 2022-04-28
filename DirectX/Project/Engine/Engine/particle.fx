#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "struct.fx"

// Particle Update Shader
RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);

[numthreads(32,1,1)]
void CS_Particle(int3 _id : SV_DispatchThreadID)
{
    
    ParticleBuffer[_id.x].vPos;
}

#endif 