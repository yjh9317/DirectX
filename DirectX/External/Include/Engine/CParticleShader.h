#pragma once
#include "CComputeShader.h"

#include "CStructuredBuffer.h"

// Particle을 업데이트 해주는 셰이더

class CParticleUpdateShader :
    public CComputeShader
{
private:
    CStructuredBuffer* m_ParticleBuffer;

public:
    void SetParticleBuffer(CStructuredBuffer* _buffer)   {  m_ParticleBuffer = _buffer;  }

public:
    virtual void UpdateData() override;
    virtual void Clear() override;

public:
    CParticleUpdateShader();
    ~CParticleUpdateShader();
};

