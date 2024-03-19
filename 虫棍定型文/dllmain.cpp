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

struct InsectGlaiveBuffState
{
    bool Red = false;
    bool Yellow = false;
    bool White = false;
    bool Power = false;
    bool Sprite = false;
    bool threeLights = false;
};
InsectGlaiveBuffState states;

static void restoreStatus(weaponStruct::InsectGlaivePtrs igStruct,InsectGlaiveBuffState& buffstates) {
    if (*(igStruct.redLight) <= 0.2) buffstates.Red = false;
    if (*(igStruct.yellowLight) <= 0.2) buffstates.Yellow = false;
    if (*(igStruct.whiteLight) <= 0.2) buffstates.White = false;
    if (*(igStruct.powerBuff) <= 0.2) buffstates.Power = false;
    if (*(igStruct.spriteBuff) <= 0.2) buffstates.Sprite = false;
    if (*(igStruct.redLight) <= 0.2 && *(igStruct.yellowLight) <= 0.2 && *(igStruct.whiteLight) <= 0.2) buffstates.threeLights = false;
}

void onTime() {
    if (tool::GetWeaponInfo().type != toolStruct::PlayerWeaponType::InsectGlaive) {return;}
    auto insectGlaiveStruct = MHweapon::InsectGlaive::getWeaponData();
    restoreStatus(insectGlaiveStruct, states);
    if ((!states.Red)&&(*insectGlaiveStruct.redLight >=20.0))
    {
        sendPlayerMessage("<STYL MOJI_RED_DEFAULT><SIZE 30>虫棍侠已点亮华贵红灯</SIZE></STYL>\0");
        states.Red = true;
    }
    if ((!states.White)&&(*insectGlaiveStruct.whiteLight >= 20.0))
    {
        sendPlayerMessage("<STYL MOJI_WHITE_DEFAULT><SIZE 30>虫棍侠已点亮迅捷白灯</SIZE></STYL>\0");
        states.White = true;
    }
    if ((!states.Yellow) && (*insectGlaiveStruct.yellowLight >= 20.0))
    {
        sendPlayerMessage("<STYL MOJI_YELLOW_DEFAULT><SIZE 30>虫棍侠已点亮沉稳黄灯</SIZE></STYL>\0");
        states.Yellow = true;
    }
    if (!states.threeLights && *(insectGlaiveStruct.redLight) >= 20.0 && *(insectGlaiveStruct.yellowLight) >= 20.0 && *(insectGlaiveStruct.whiteLight) >= 20.0)
    {
        sendPlayerMessage("<SIZE 40><STYL MOJI_YELLOW_DEFAULT>虫棍</STYL><STYL MOJI_RED_DEFAULT>全力</STYL><STYL MOJI_WHITE_DEFAULT>解放</STYL>！</SIZE>\0");
        states.threeLights = true;
    }
    //还原状态
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

