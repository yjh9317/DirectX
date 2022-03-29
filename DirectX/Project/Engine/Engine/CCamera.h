#pragma once
#include "CComponent.h"

// ī�޶�� �ϳ��� ������Ʈ�� ���� ī�޶����� ����
// View Space(Camera Space): ī�޶��� ��ġ,������ �������� ��ü���� Transform���� ����
// View Space������ ī�޶��� ��ġ�� ����, ī�޶� �ٶ󺸴� ������ z���̴�.


// Camera�� ������ �°� ��ü�� ��ġ��Ű���� World Space�� �ִ� ��ü����
// Camera Object�� World�� ���� �������� �ٲ� Transform��(ȸ��,�̵�,ũ��)��ŭ 
// �Ȱ��� �����ϸ� �ȴ�.


class CCamera :         
    public CComponent
{
private:
    Matrix m_matView;   //View����� ���ϸ� View Space�� �Ѿ�´�.

public:
    virtual void finalupdate() override;

public:
    CCamera();
    ~CCamera();
};

