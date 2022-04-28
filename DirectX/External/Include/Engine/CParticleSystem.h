#pragma once
#include "CRenderComponent.h"

#include "CStructuredBuffer.h"
#include "CParticleShader.h"

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleShader>    m_CS;           // 파티클 업데이트 쉐이더

    CStructuredBuffer*  m_ParticleBuffer;   // 파티클 구조화 버퍼
    UINT                m_iMaxCount;        // 파티클 최대 개수

private:
    virtual void finalupdate() override;
    virtual void render() override;

    
    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _origin);
    ~CParticleSystem();
};

