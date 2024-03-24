// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <vector>
#include "ghidra_export.h"
#include "loader.h"
#include "util.h"
#include "myTool.h"

using namespace std;
using namespace loader;
using namespace tool;

onTimeSequenceButtons fsmSequence({70,83,77});
tool::ConditionChecker fsmUpdateChecker;
auto modActivation = false;

void mainFunction() {
    auto fsmInfo = tool::GetFSMInfo();
    auto isLoop = fsmUpdateChecker.CheckRepeatCondition<int>(fsmInfo.fsmId, "fsmID");
    if (isLoop)
    {
        ::stringstream fsmss;
        fsmss << "FsmID: " << fsmInfo.fsmId << " FsmTarget: " << fsmInfo.fsmTarget;
        ShowMessage(fsmss.str());
    }
}

void onTime() {
    if (fsmSequence.IsMultipleButtonPressed())
    {
        modActivation = !modActivation;
        ::stringstream ss;
        ss << "fsm查看器: " << (modActivation ? "启动" : "关闭");
        ShowMessage(ss.str());
    }
    if (modActivation)
    {
        mainFunction();
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

