// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <vector>
#include "ghidra_export.h"
#include "util.h"
#include "loader.h"
#include "myTool.h"

using namespace std;
using namespace loader;
using namespace tool;

onTimeSequenceButtons clutchButtons({ 67 ,76 ,85 });
auto modActivation = false;

void onTime() {
    auto fsm = GetFSMInfo();
    vector<int> clutchActionTable{ 677, 678, 679 };
    if (clutchButtons.IsMultipleButtonPressed())
    {
        modActivation = !modActivation;
        ::stringstream ss;
        ss << "瞬移飞翔爪mod: " << (modActivation ? "开启" : "关闭");
        ShowMessage(ss.str());
    }

    if (modActivation && isIn<int>(clutchActionTable, fsm.fsmId))
    {
        SetFSM(1, 680);
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
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        return Load();
    return TRUE;
}

