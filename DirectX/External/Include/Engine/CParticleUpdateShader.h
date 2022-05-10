#pragma once
#include "CComputeShader.h"

#include "CStructuredBuffer.h"

// Particle을 업데이트 해주는 셰이더

class CParticleUpdateShader :
    public CComputeShader
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    CStructuredBuffer* m_ParticleDataBuffer;

public:
    void SetParticleBuffer(CStructuredBuffer* _buffer) { m_ParticleBuffer = _buffer; }
    void SetParticleDataBuffer(CStructuredBuffer* _buffer) { m_ParticleDataBuffer = _buffer; }
    void SetObjectWorldPos(Vec3 _vWorldPos) { m_Param.v4Arr[0] = _vWorldPos; }

public:
    virtual void UpdateData() override;
    virtual void Clear() override;

public:
    CParticleUpdateShader();
    ~CParticleUpdateShader();
};

