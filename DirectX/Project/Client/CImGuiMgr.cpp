#include "pch.h"
#include "CImGuiMgr.h"

#include <Engine/CDevice.h>
#include <Engine/CGameObject.h>
#include <Engine/CSceneMgr.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include "UI.h"
#include "ParamUI.h"


CImGuiMgr::CImGuiMgr()
{

}

CImGuiMgr::~CImGuiMgr()
{
    Safe_Del_Map(m_mapUI);
}

void CImGuiMgr::init(HWND _hwnd)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hwnd);             // Engine의 윈도우에 init
    ImGui_ImplDX11_Init(DEVICE, CONTEXT);    // Engine의 Device,context에 init


    // 기본 UI 들 생성
    CreateUI();
}

void CImGuiMgr::progress()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Param Init
    ParamUI::KeyCount = 0;

    // UI Update
    for (auto& pair : m_mapUI)
    {
        pair.second->update();
    }

    // UI Render
    for (auto& pair : m_mapUI)
    {
        pair.second->render();
    }

    bool bOpen = true;
    ImGui::ShowDemoWindow(&bOpen);


    // Delegate 호출, Delegate가 있으면 호출하고 삭제
    for (size_t i = 0; i < m_vecDelegate.size(); ++i)
    {
        (m_vecDelegate[i].pInst->*m_vecDelegate[i].pFunc)(m_vecDelegate[i].dwParam);
    }

    m_vecDelegate.clear();

}

void CImGuiMgr::render()
{
    // Rendering
    // UI 클래스에서 update_render한 정보들로 Render
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());    //ImGui가 윈도우 창 안에 있을때

    // Update and Render additional Platform Windows

    // 도킹버전의 ImGui는 윈도우 밖에서도 imgui를 사용할 수 있다.
    // 만약 ImGui가 윈도우 밖으로 빠져나갈 경우 동일한사이즈의 새로운 윈도우와 렌더타겟텍스쳐,스왑체인를 생성하고
    // 새로 생긴 윈도우를 렌더링을 한다, ImGui::UpdatePlatformWindows(); -> 윈도우가 창밖으로 빠져나갈때 호출
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void CImGuiMgr::clear()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

#include "InspectorUI.h"
#include "ResourceUI.h"
#include "SceneOutliner.h"
#include "ListUI.h"

void CImGuiMgr::CreateUI()
{
    CGameObject* pTargetObj = CSceneMgr::GetInst()->FindObjectByName(L"MainCamera");

    // InspectorUI 생성
    InspectorUI* pUI = new InspectorUI;
    pUI->SetTargetObject(pTargetObj);
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    // ResourceUI 생성
    ResourceUI* pResUI = new ResourceUI;
    m_mapUI.insert(make_pair(pResUI->GetName(), pResUI));

    // SceneOutLiner
    SceneOutliner* pSceneOutliner = new SceneOutliner;
    m_mapUI.insert(make_pair(pSceneOutliner->GetName(), pSceneOutliner));

    // ListUI 생성
    ListUI* pListUI = new ListUI;
    pListUI->Deactivate();
    pListUI->SetModalUI(true);
    m_mapUI.insert(make_pair(pListUI->GetName(), pListUI));
}

UI* CImGuiMgr::FindUI(const string& _strKey)
{
    map<string, UI*>::iterator iter = m_mapUI.find(_strKey);

    if (m_mapUI.end() == iter)
    {
        return nullptr;
    }

    return iter->second;
}


