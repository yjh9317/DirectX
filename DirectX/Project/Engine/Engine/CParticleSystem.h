#pragma once
#include "CRenderComponent.h"

#include "CStructuredBuffer.h"
#include "CParticleUpdateShader.h"

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleUpdateShader>      m_CS;           // ��ƼŬ ������Ʈ ���̴�

    CStructuredBuffer*              m_ParticleBuffer;   // ��ƼŬ ����ȭ ����
    CStructuredBuffer*              m_DataBuffer;       // �߰� ������ ����(��ƼŬ AliveCount)
    UINT                            m_iMaxCount;        // ��ƼŬ �ִ� ����

    int                             m_bPosInherit;      // ������Ʈ ��ǥ�� ����
    int                             m_iAliveCount;      // �̹� �����ӿ� ����(Ȱ��ȭ) �� ��ƼŬ ��


    float                           m_fMinLifeTime;     // ��ƼŬ�� �ּ� ���� �ð�
    float                           m_fMaxLifeTime;     // ��ƼŬ�� �ִ� ���� �ð�

    float                           m_fStartSpeed;      // ��ƼŬ�� �ּ� �ӷ�
    float                           m_fEndSpeed;        // ��ƼŬ�� �ִ� �ӷ�

    Vec4                            m_vStartColor;      // ��ƼŬ �ʱ� ����
    Vec4                            m_vEndColor;        // ��ƼŬ ���� ����

    Vec3                            m_vStartScale;      // ��ƼŬ �ʱ� ũ��
    Vec3                            m_vEndScale;        // ��ƼŬ ���� ũ��

    float                           m_fParticleCreateDistance;  // ��ƼŬ ���� ����
    float                           m_fParticleCreateTerm;      // ��ƼŬ ���� ����
    float                           m_fAccTime;

public:
    virtual void finalupdate() override;
    virtual void render()   override;

    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _origin);
    ~CParticleSystem();
};

