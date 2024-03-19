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


tool::ConditionChecker checker;

void onTime() {
    if (tool::GetWeaponInfo().type != toolStruct::PlayerWeaponType::InsectGlaive) {return;}
    auto insectGlaiveStruct = MHweapon::InsectGlaive::getWeaponData();
    //还原状态
    if (checker.CheckNumericCondition<float>(*insectGlaiveStruct.redLight,"RedLight",20.0,0.2))
    {
        sendPlayerMessage("<STYL MOJI_RED_DEFAULT><SIZE 30>虫棍侠已点亮华贵红灯</SIZE></STYL>\0");
    }
    if (checker.CheckNumericCondition<float>(*insectGlaiveStruct.whiteLight, "WhiteLight", 20.0, 0.2))
    {
        sendPlayerMessage("<STYL MOJI_WHITE_DEFAULT><SIZE 30>虫棍侠已点亮迅捷白灯</SIZE></STYL>\0");
    }
    if (checker.CheckNumericCondition<float>(*insectGlaiveStruct.yellowLight, "YellowLight", 20.0, 0.2))
    {
        sendPlayerMessage("<STYL MOJI_YELLOW_DEFAULT><SIZE 30>虫棍侠已点亮沉稳黄灯</SIZE></STYL>\0");
    }
    if (checker.CheckMultipleNumericCondition<float>({ *insectGlaiveStruct.redLight ,*insectGlaiveStruct.whiteLight,*insectGlaiveStruct.yellowLight }, "TripleLight", { 30.0,30.0,30.0 }, {0.2,0.2,0.2}))
    {
        sendPlayerMessage("<SIZE 40><STYL MOJI_YELLOW_DEFAULT>虫棍</STYL><STYL MOJI_RED_DEFAULT>全力</STYL><STYL MOJI_WHITE_DEFAULT>解放</STYL>！</SIZE>\0");
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

