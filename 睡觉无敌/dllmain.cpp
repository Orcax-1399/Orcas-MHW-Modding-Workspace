// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <vector>
#include "ghidra_export.h"
#include "util.h"
#include "loader.h"
#include "myTool.h"
#include "myToolStruct.h"

using namespace std;
using namespace loader;

bool modActivate = false;
tool::onTimeSequenceButtons KeySLP({ 83,76,80 });

static undefined(*HitPtr)(undefined*, undefined*) = (undefined(*)(undefined*, undefined*))0x141F5048B;

void mainFunction() {
    if (modActivate)
    {
        byte* MutekiPtr = tool::GetMutekiStatePtr();
        if (!tool::isInCombatZone()) {
            modActivate = false;
            *MutekiPtr = MutekiState::Normal;
            tool::ShowMessage("睡觉MOD在集会使用会闪退，已阻止");
            return;
        }
        else
        {
            tool::SetFSM(1, 429);
            *MutekiPtr = MutekiState::Hyper;
        }

    }
}

void onTime() {
    if (KeySLP.IsMultipleButtonPressed()) { 
        byte* MutekiPtr = tool::GetMutekiStatePtr();
        modActivate = !modActivate;
        string msg = modActivate ? "睡觉无敌已启动" : "睡觉无敌已关闭";
        tool::ShowMessage(msg);
        if (!modActivate) { *MutekiPtr = MutekiState::Normal; }
    }
    mainFunction();
}

void hitHook() {
    MH_Initialize();
    HookLambda(HitPtr,
        [](auto rcx, auto rdx) {
            //受击统计，清除计数器交给逻辑代码，不在这里执行
    //        Hit.HitCount += 1;
    //Hit.HitPlot = *((void**)(rdx + 0x8));
    //如果设定了无敌，则设置rcx为0，后续交给程序自己执行
    if (modActivate) rcx = 0;

    auto ret = original(rcx, rdx);
    return ret;
        });
    MH_ApplyQueued();
}

__declspec(dllexport) extern bool Load()
{
    hitHook();
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

