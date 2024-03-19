// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <vector>
#include "ghidra_export.h"
#include "loader.h"
#include "util.h"
#include "myTool.h"
#include "myToolStruct.h"

using namespace std;
using namespace loader;
using namespace tool;
using namespace toolStruct;
OnTimeButton key97,key98;

void onTime() {
    auto QuestInfo = GetQuestInfo();
    int* QuestId = QuestInfo.QuestId;
    int* QuestState = QuestInfo.QuestState;
    if (*QuestId==-1)
    {
        return ;
    }
    if (key97.IsButtonPressed(120))
    {
        LOG(INFO) << "Key97 Pressed";
        SetQuestState(3);
        ShowMessage("瞬间完成任务");
    }
    if (key98.IsButtonPressed(121))
    {
        LOG(INFO) << "Key98 Pressed";
        SetQuestState(5);
        ShowMessage("瞬间结算");
    }

}

__declspec(dllexport) extern bool Load()
{

    MH_Initialize();
    HookLambda(MH::World::MapClockLocal,
        [](auto clock, auto clock2) {
            auto ret = original(clock, clock2);
    onTime();
    return ret;
        });
    MH_ApplyQueued();
    return true;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Load();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

