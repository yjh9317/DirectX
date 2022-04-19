#pragma once
#include "CEntity.h"

#include "CTexture.h"

class CAnimator2D;

class CAnimation2D :
    public CEntity
{
private:
    vector<tAnim2DFrame>    m_vecFrm;       // 프레임 정보
    Ptr<CTexture>           m_pAtlasTex;    // 텍스쳐
    CAnimator2D*            m_pOwner;
    int                     m_iCurFrmIdx;
    bool                    m_fAccTime;
    bool                    m_bFinish;
    
public:
    void finalupdate();

    virtual void UpdateData() override;

public:
    void Create(Ptr<CTexture> _Atlas, Vec2 _vLT, Vec2 _vSlice, Vec2 _vStep, float _fDuration, int _iFrameCount);
    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_fAccTime = 0.f;
        m_bFinish = false;
        m_iCurFrmIdx = 0;
    }

public:
    CLONE(CAnimation2D)
    
public:
    CAnimation2D();
    ~CAnimation2D();

    friend class CAnimator2D;
};

