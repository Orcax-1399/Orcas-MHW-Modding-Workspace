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


void onTime() {
    vector<int> fsmTable = GetFSM();
    //LOG(WARN) << fsmTable[0];
    //LOG(WARN) << fsmTable[1];
    vector<int> clutchActionTable{ 677, 678, 679 };
    if (find(clutchActionTable.begin(), clutchActionTable.end(), fsmTable[1]) != clutchActionTable.end())
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

