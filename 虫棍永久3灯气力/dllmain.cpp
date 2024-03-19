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

OnTimeButton key221;
bool functionActivate = false;

void mainFunc() {
    
    if (!functionActivate) //提前返回
    {
        return;
    }
    PlayerWeaponInfo weaponInfo = GetWeaponInfo();
    if (weaponInfo.type == PlayerWeaponType::ErrorWeapon) //如果拿不到武器类型，返回
    {
        LOG(WARN) << "Can't Get Weapon Type";
        return;
    }
    if (weaponInfo.type != PlayerWeaponType::InsectGlaive)  //拿到不是虫棍，mod关闭
    {
        ShowMessage("当前不是虫棍，三灯MOD自动关闭");
        functionActivate = false;
        return;

    }
    auto fsmList = GetFSMInfo(); //判断fsm状态；;
    if (fsmList.fsmTarget != 3)
    {
        return;
    }
    void* weaponEntity = multiLevelPointerOffset(0x145011760, { 0x50, 0x76B0 });
    if (weaponEntity == nullptr) { return; }
    float* redLight = offsetPtr<float>(weaponEntity, 0x2368);
    float* yellowLight = offsetPtr<float>(weaponEntity, 0x236C);
    float* whiteLight = offsetPtr<float>(weaponEntity, 0x2370);
    float* powerBuff = offsetPtr<float>(weaponEntity, 0x4058);
    float* spriteBuff = offsetPtr<float>(weaponEntity, 0x405C);
    *redLight = (float)3600.0;
    *yellowLight = (float)3600.0;
    *whiteLight = (float)3600.0;
    *powerBuff = (float)3600.0;
    *spriteBuff = (float)3600.0;

}


void onTime() {
    if (key221.IsButtonPressed(221))
    {
        functionActivate = !functionActivate;
        string msg;
        msg = functionActivate ? "永久三灯启用" : "永久三灯关闭";
        ShowMessage(msg);
    }
    mainFunc();
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

