#pragma once
#include "CComponent.h"

// 카메라는 하나의 오브젝트를 만들어서 카메라역할을 수행
// View Space(Camera Space): 카메라의 위치,방향을 기준으로 물체들의 Transform값을 적용
// View Space에서는 카메라의 위치가 원점, 카메라가 바라보는 정면이 z축이다.

class CGameObject;

class CCamera :
    public CComponent
{
private:
    vector<CGameObject*>    m_vecForward;   // 불투명 물체
    vector<CGameObject*>    m_vecMasked;    // 투명 , 불투명 물체
    vector<CGameObject*>    m_vecOpaque;    // 반투명 물체
    
    Matrix                  m_matView;   // View행렬, 이 행렬을 곱하면 View Space로 넘어온다.
    Matrix                  m_matProj;   // 투영 행렬(View Space로 넘어온 물체를 -1 ~ 1범위로 압축할 때 필요한 행렬)

    UINT                    m_iLayerMask;   
    int                     m_iCamIdx;


public:
    void SetCameraAsMain();
    void CheckLayerMask(int _iLayerIdx);
    void CheckLayerMask(const wstring& _strLayerName);
    void CheckLayerMaskAll()  { m_iLayerMask = 0xffffffff; }


    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

private:
    void SortGameObject();  // Shader Domain에 따른 물체 분류 (이벤트 처리)


public:
    virtual void finalupdate() override;
    void render_forward();
    void render_masked();
    void render_opaque();
    

    CLONE(CCamera)

        
public:
    CCamera();
    CCamera(const CCamera& _origin);
    ~CCamera();

    friend class CRenderMgr;
};

