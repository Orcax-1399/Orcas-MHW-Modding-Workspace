// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <MinHook.h>
#include "myAddress.h"
#include "myTool.h"
#include "ghidra_export.h"
#include "loader.h"
#include <random>

using namespace addressResources;
using namespace loader;
DealDamageFunctionType dealDamageOriginFunction = nullptr;
std::mt19937 engine(static_cast<unsigned long>(time(0)));

// 定义一个分布，这里是0到99的均匀分布
std::uniform_int_distribution<int> distribution(0, 100);
tool::Timer shoutOutTimer;
int originalAtk = 0;

int randomNumber = 0;

void RestoreAtk(void* monsterPointer, int damage, void* position, BOOL isTenderize, BOOL isCrit, int unk0, int unk1, char unk2, int attackID) {
    auto dealdamage = damage;
    auto tenderize = isTenderize;
    auto critical = isCrit;
    int* wpAtkPtr = tool::cheatEnginePtrOffset<int>(0x1450106F8, { 0x810 ,0xA0 ,0x58 ,0x7ED8 ,0x76B0 ,0x1D14 });
    *wpAtkPtr = originalAtk;
    dealDamageOriginFunction(monsterPointer, dealdamage, position, tenderize, critical, unk0, unk1, unk2, attackID);
}

void DisplayHook() {
    auto TargetFunction = (DealDamageFunctionType)0x141CC51B0;
    auto successful = MH_CreateHook(
        TargetFunction, static_cast<DealDamageFunctionType>(&RestoreAtk), reinterpret_cast<LPVOID*>(&dealDamageOriginFunction)
    );
    if (successful == MH_OK)
    {
        LOG(WARN) << "DamageHookSuccess!";
        MH_EnableHook(TargetFunction);
    }
}

int probabilityJudgement() {
    int* wpAtkPtr = tool::cheatEnginePtrOffset<int>(0x1450106F8, { 0x810 ,0xA0 ,0x58 ,0x7ED8 ,0x76B0 ,0x1D14 });
    originalAtk = *wpAtkPtr;
    randomNumber = distribution(engine);
    auto fsms = tool::GetFSMInfo();
    if ((tool::GetWeaponInfo().type != toolStruct::PlayerWeaponType::GreatSword)||(fsms.fsmTarget!=3))
    {
        return 0;
    }
    if (randomNumber < 30)
    {
        *wpAtkPtr = 0;
        if (shoutOutTimer.isCooledDown("shoutOutMinCoolDown", 2000))
        {
            tool::sendPlayerMessage("<STYL MOJI_RED_DEFAULT><SIZE 40>1！</SIZE></STYL>");
        }
    };
    if (randomNumber >= 85)
    {

        *wpAtkPtr = originalAtk * 2;
        if (shoutOutTimer.isCooledDown("shoutOutCoolDown", 10000))
        {
            tool::sendPlayerMessage("<STYL MOJI_RED_DEFAULT><SIZE 25>斯国一</SIZE></STYL>");
        }

    };
    if (randomNumber >= 95)
    {
        *wpAtkPtr = originalAtk * 4;
        if (shoutOutTimer.isCooledDown("shoutOutMaxCoolDown", 10000))
        {
            tool::sendPlayerMessage("<STYL MOJI_RED_DEFAULT><SIZE 35>昂哔哔叭波</SIZE></STYL>");
        }
    };
}


__declspec(dllexport) extern bool Load()
{
    MH_Initialize();
    HookLambda(doDamageFunctionPtr, 
        [](auto rcx, auto rdx) {
            probabilityJudgement();
            auto ret = original(rcx, rdx);
            return ret;
        });
    DisplayHook();
    MH_ApplyQueued();
    return true;
}

BOOL APIENTRY DllMain( HMODULE hModule,
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

