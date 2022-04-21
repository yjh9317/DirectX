#pragma once
#include "CRenderComponent.h"

#include "CComponent.h"




// SharedMtrl,DynamicMtrl

// ������Ʈ���� �ڱ⸸�� ������ ������ �ִ� ���·� ����� ������(�޸𸮵� �׷��� ũ�� ����) ����ȭ�� ���ؼ� �׷��� ������ ����.
// ���������������ο��� �߻��ϴ� Draw call�� ���̱�����(cpu�� gpu���� �޸� ��������� ŭ)
// �������� �� �������� ���� ������ IA�ܰ迡�� ���̴��� ���� ����, Material(������۸� �ű�� �������� ���ε�)��  ������Ʈ �ϳ� ȣ���� ������ �غ��������.
// 
// 2D������ ���� ���͸���� ������ �� ������ �� �� ����.
// 3D������ instancing�� ���� A�� B�� ���ӿ�����Ʈ�� �׸��� ������ ���������ΰ����� �ִµ� ���� ���͸����� ����ϰ�������
// �߰��� ���������� ����ϴ� ������Ʈ���� ��� �ϰ�ó���ϴ� ���� instancing(�޽��� ���ƾ���)


class CMeshRender :
    public CRenderComponent
{

public:

public:
    virtual void finalupdate() override;
    virtual void render() override;

    CLONE(CMeshRender)

public:
    CMeshRender();
    ~CMeshRender();
};

