#pragma once
#include "UI.h"

#include "TreeUI.h"

class CGameObject;

class SceneOutliner :
    public UI
{
private:
    TreeUI* m_TreeUI;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void Reset();
    void ObjectClicked(DWORD_PTR _dw);

    void AddGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode);  // 오브젝트를 트리에 넣는 재귀 함수
    void PressDelete(DWORD_PTR _dw);    // 눌렸을때 삭제하는 Event Delegate
    void DragAndDropDelegate(DWORD_PTR _dwDrag, DWORD_PTR _dwDrop);
    void ResDrop(DWORD_PTR _resPtr);

public:
    SceneOutliner();
    ~SceneOutliner();
};

