// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <vector>
#include "ghidra_export.h"
#include "loader.h"
#include "util.h"
#include "myTool.h"
#include <thread>

using namespace std;
using namespace loader;
using namespace tool;

bool modActivate = false;
OnTimeButton keyEXPL;
void keyCheck() {
    Sleep(10000);
    while (true)
    {
        Sleep(50);
        if (keyEXPL.IsMutipleButtonPressed({ 69,88,80,76 })) {// E X P L四个键 
            modActivate = !modActivate;
            string msg = modActivate ? "爆炸开火启动" : "爆炸开火关闭";
            ShowMessage(msg);
        }

    }
}

void onTime() {
    Sleep(10000);
    while (true)
    {
        Sleep(100);
        if (!modActivate) continue;
        if (!isInCombatZone())
        {
            ShowMessage("当前不在战斗场地，mod自动关闭");
            modActivate = false;
            continue;
        }
        PlayerWeaponInfo weaponInfo = GetWeaponInfo();
        if (weaponInfo.type == PlayerWeaponType::Bow || weaponInfo.type == PlayerWeaponType::HeavyBowGun || weaponInfo.type == PlayerWeaponType::LightBowGun)
        {
            if (GetFSMInfo().fsmTarget != 3) continue; //判断是不是拔刀
            void* explodeFirePtr = ptrOffsetCalc(0x145011760, { 0x50, 0xE470 });
            if (explodeFirePtr == nullptr) continue;
            *offsetPtr<int>(explodeFirePtr, 0x190) = 1;//爆炸开火核心代码
        }
        else
        {
            ShowMessage("当前武器类型不是弓或者弩，mod自动关闭");
            modActivate = false;
            continue;
        }
    }
}

__declspec(dllexport) extern bool Load()
{

    /*MH_Initialize();
    HookLambda(MH::World::MapClockLocal,
        [](auto clock, auto clock2) {
            auto ret = original(clock, clock2);
    onTime();
    return ret;
        });
    MH_ApplyQueued();*/
    thread th1(onTime);
    th1.detach();
    thread kc(keyCheck);
    kc.detach();
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

