#pragma once
#include "CComponent.h"

// ī�޶�� �ϳ��� ������Ʈ�� ���� ī�޶����� ����
// View Space(Camera Space): ī�޶��� ��ġ,������ �������� ��ü���� Transform���� ����
// View Space������ ī�޶��� ��ġ�� ����, ī�޶� �ٶ󺸴� ������ z���̴�.

class CCamera :
    public CComponent
{
private:
    Matrix m_matView;   // View���, �� ����� ���ϸ� View Space�� �Ѿ�´�.
    Matrix m_matProj;   // ���� ���(View Space�� �Ѿ�� ��ü�� -1 ~ 1������ ������ �� �ʿ��� ���)

public:
    virtual void finalupdate() override;

    CLONE(CCamera)
public:
    CCamera();
    ~CCamera();
};

