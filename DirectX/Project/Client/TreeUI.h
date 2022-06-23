#pragma once
#include "UI.h"

#include <Engine/CKeyMgr.h>

class TreeUI;

class TreeNode
{
private:
    TreeUI*             m_pTreeUI;      // 이 TreeNode를 관리하는 UI
    TreeNode*           m_pParent;      // 부모노드
    vector<TreeNode*>   m_vecChild;     // 자식노드
    bool                m_bLeaf;        // 단말노드
    bool                m_bSelected;    // 선택(클릭)되면 표시

    string              m_strName;      // 이름
    DWORD_PTR           m_dwData;       // 포인터를 받을 8바이트 정수 


public:
    void update();
    void render_update();

public:
    void AddChild(TreeNode* _pChild)
    {
        _pChild->m_pParent = this;
        m_vecChild.push_back(_pChild);
        m_bLeaf = false;
    }

    const string& GetName() { return m_strName; }
    DWORD_PTR GetData() { return m_dwData; }
    TreeNode* GetParent() { return m_pParent; }

public:
    TreeNode();
    TreeNode(const string& _strName, DWORD_PTR _dwData);
    ~TreeNode();

    friend class TreeUI;
};


typedef void(UI::* CLICKED)(DWORD_PTR);                 //클릭 Delegate
typedef void(UI::* DRAG_DROP)(DWORD_PTR, DWORD_PTR);    //더블클릭 Delegate
typedef CLICKED KEY_FUNC;


struct tTreeKey 
{
    KEY eKey;
    UI* pInst;
    CLICKED pFunc;
};


class TreeUI :
    public UI
{
private:
    TreeNode* m_pRootNode;
    TreeNode* m_pSelectedNode;          //이전에 선택된 노드
    TreeNode* m_pDragNode;              // 드래그노드
    TreeNode* m_pDropNode;              // 드랍노드

    const bool  m_bUseDummyRoot;        // 더미노드
    bool        m_bShowDummy;           // 더미가 보일지 체크
    bool        m_bUseFrame;            // 프레임(두껍게하는거)체크

    bool                m_bUseDragDropSelf; // 내부->내부 Drag Drop 체크
    bool                m_bUseDragDropOuter; // 외부 -> 내부 Drag Drop 체크

    // Clicked Delegate
    UI* m_pCInst;
    CLICKED     m_CFunc;

    // Double Clicked
    UI* m_pDBCInst;
    CLICKED     m_DBCFunc;

    // Drag And Drop
    UI* m_pDADInst;
    DRAG_DROP   m_DADFunc;

    // Key Binding Delegate
    vector<tTreeKey>    m_vecKeyBind;



public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void ShowDummyRoot(bool _bTrue) { m_bShowDummy = _bTrue; }
    void UseFrame(bool _b) { m_bUseFrame = _b; }
    void UseDragDropOuter(bool _b) { m_bUseDragDropOuter = _b; }
    void UseDragDropSelf(bool _b) { m_bUseDragDropSelf = _b; }
    TreeNode* AddTreeNode(TreeNode* _pParentNode, const string& _strName, DWORD_PTR _dwData = 0);
    TreeNode* GetDummyNode() { return m_pRootNode; }


    void SetClickedDelegate(UI* _pInst, CLICKED _Func) { m_pCInst = _pInst; m_CFunc = _Func; }      
    void SetDoubleClickedDelegate(UI* _pInst, CLICKED _Func) { m_pDBCInst = _pInst; m_DBCFunc = _Func; }
    void SetDragAndDropDelegate(UI* _pInst, DRAG_DROP _Func) { m_pDADInst = _pInst; m_DADFunc = _Func; }
    void SetKeyBinding(KEY _eKey, UI* _pInst, KEY_FUNC _Func);

    void Clear();
private:
    void SetSelectedNode(TreeNode* _pNode);
    void SetDBClickedNode(TreeNode* _pNode);

public:
    TreeUI(bool _bDummyRoot);
    ~TreeUI();

    friend class TreeNode;
};

