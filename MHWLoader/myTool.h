#pragma once
#include "ghidra_export.h"
#include "util.h"
#include <vector>
#include <map>
#include <windows.h>
#include "basicTool.h"
#include "loader.h"
#include "myToolStruct.h"
#include <deque>
#include <chrono>
using namespace std;
using namespace basicTool;
using namespace loader;
using namespace toolStruct;


namespace tool {
    /// <summary>
    /// ����༶ƫ�ƣ�ptr�ǻ�ַ�����洫��һ��ƫ���飬���һ����ƫ��Ҳ��������
    /// �������ce�����൱�ڶ༶ָ������ټ�һ��0��offset
    /// ���������ܶ�ʹ�� **cheatEnginePtrOffset**������
    /// </summary>
    /// <param name="ptr"></param>
    /// <param name="v"></param>
    /// <returns></returns>
    void* multiLevelPointerOffset(long long ptr, vector<int> v) {
        void* tmpPtr = *(undefined**)ptr;
        for (int offset : v)
        {
            tmpPtr = *offsetPtr<void*>(tmpPtr, offset);
            if (tmpPtr == nullptr)
            {
                return 0;
            }
        }
        return tmpPtr;
    }

    /// <summary>
    /// ��ԭ��CE�еĶ༶ָ��ƫ�Ʒ���
    /// ��CEһ����ָ��ƫ�Ʒ��������������ܶ��滻Ϊ�����
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="ptr">��һ��8�ֽ�ָ��</param>
    /// <param name="offsets">��ƫ���飬������ʽ</param>
    /// <returns></returns>
    template<class T>
    T* cheatEnginePtrOffset(long long ptr, vector<int> offsets) {
        uint64_t* internalPtr = (uint64_t*)ptr;
        for (int offset : offsets)
        {
            internalPtr = (uint64_t*)*internalPtr;
            if (internalPtr == nullptr) { return nullptr; }
            internalPtr = offsetPtr<uint64_t>(internalPtr, offset);
        }
        return reinterpret_cast<T*>(internalPtr);
    }


    /// <summary>
    /// ���һ���ɵ����������Ƿ����ĳ��ֵ
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="arr"></param>
    /// <param name="target"></param>
    /// <returns></returns>
    template<typename T>
    inline bool isIn(vector<T> arr, T target) { return (find(arr.begin(), arr.end(), target) != arr.end()) ? true : false; }

    ///ֱ������FSM
    void SetFSM(int FSMTYPE, int FSMID) {
        void* CoordinatesPlot = *(undefined**)MH::Player::PlayerBasePlot;
        void* PlayerPlot = *offsetPtr<void*>(CoordinatesPlot, 0x50);
        *offsetPtr<int>(PlayerPlot, 0x6284) = FSMTYPE;
        *offsetPtr<int>(PlayerPlot, 0x6288) = FSMID;
        *offsetPtr<int>(PlayerPlot, 0x628C) = FSMTYPE;
        *offsetPtr<int>(PlayerPlot, 0x6290) = FSMID;
        return;
    }
    ///��ȡFSM��Target��Id������һ��Vector��0��Target��1��Id
    FsmInfo GetFSMInfo() {
        FsmInfo fsmInfo;
        void* CoordinatesPlot = *(undefined**)MH::Player::PlayerBasePlot;
        void* CoordinatesPlot1 = *offsetPtr<void*>(CoordinatesPlot, 0x50);
        //LOG(WARN) << *offsetPtr<int>(CoordinatesPlot1, 0x6274);
        //LOG(WARN) << *offsetPtr<int>(CoordinatesPlot1, 0x6278);
        fsmInfo.fsmTarget = *offsetPtr<int>(CoordinatesPlot1, 0x6274);
        fsmInfo.fsmId = *offsetPtr<int>(CoordinatesPlot1, 0x6278);
        return fsmInfo;
    }

    /// <summary>
    /// ���һЩ������Ϣ��������������λ��
    /// </summary>
    /// <returns>������Ϣ(��ͼid����������)</returns>
    WorldInfo GetWorldInfo() {
        WorldInfo worldInfo;
        coordinate wayPointCoordinate{ -1,-1,-1 };
        worldInfo.mapId = -1;
        worldInfo.wayPointCoordinate = wayPointCoordinate;
        void* mapPtr = multiLevelPointerOffset(0x145011760, { 0x50, 0x7D20 });
        if (mapPtr == nullptr) { return worldInfo; }
        worldInfo.mapId = *offsetPtr<int>(mapPtr, 0xB88);
        void* worldDataPtr = multiLevelPointerOffset(0x1451ca0c0, { 0x48, 0x58, 0x58, 0x40 });
        if (worldDataPtr == nullptr) { return worldInfo; }
        wayPointCoordinate.x = *offsetPtr<float>(worldDataPtr, 0x2D0);
        wayPointCoordinate.y = *offsetPtr<float>(worldDataPtr, 0x2D4);
        wayPointCoordinate.z = *offsetPtr<float>(worldDataPtr, 0x2D8);
        worldInfo.wayPointCoordinate = wayPointCoordinate;
        return worldInfo;
    }

    /// <summary>
    /// �ж��Ƿ���ս�����أ�������֮��ľͷ���false
    /// </summary>
    /// <returns>����</returns>
    bool isInCombatZone() {
        WorldInfo worldinfo = GetWorldInfo();
        vector<int> excludeMapId = { 301,302,303,305,306,503,506 };
        return !isIn(excludeMapId, worldinfo.mapId);
    }
    
    /// <summary>
    /// ��ȡ��ǰ��ҵ��������ͺ�����id
    /// </summary>
    /// <returns></returns>
    PlayerWeaponInfo GetWeaponInfo() {
        PlayerWeaponInfo weaponInfo;
        weaponInfo.type = PlayerWeaponType::ErrorWeapon;
        weaponInfo.id = NULL;
        weaponInfo.sharpness = nullptr;
        void* weaponType = multiLevelPointerOffset(0x145011760, { 0x50,0xc0,0x8,0x78 });
        if (weaponType == nullptr) { return weaponInfo; }
        weaponInfo.type = static_cast<toolStruct::PlayerWeaponType>(*offsetPtr<int>(weaponType, 0x2e8));
        weaponInfo.id = *offsetPtr<int>(weaponType, 0x2ec);
        void* weaponSharpness = multiLevelPointerOffset(0x145011760, { 0x50 ,0x76b0 });
        if (weaponSharpness == nullptr) { return weaponInfo; }
        weaponInfo.sharpness = offsetPtr<int>(weaponSharpness, 0x20F8);
        return weaponInfo;
    }
    void* GetWeaponEntityPtr() {
        auto weaponEntity = tool::multiLevelPointerOffset(0x145011760, { 0x50, 0x76B0 });
        return weaponEntity;
    }
    ///��������״ָ̬�룬û�õ����ǿ�ָ��
    int* GetPlayerStatePtr() {
        void* playerStatePtr = multiLevelPointerOffset(0x145011760, { 0x50 , 0xE470 });
        if (playerStatePtr == nullptr) { return nullptr; }
        return offsetPtr<int>(playerStatePtr, 0x0B);
    }
    /// <summary>
    /// �����޵�״ָ̬��
    /// </summary>
    /// <returns></returns>
    ::byte* GetMutekiStatePtr() {
        void* MutekiStatePtr = multiLevelPointerOffset(0x145011760, { 0x50 ,0x6A8 });
        if (MutekiStatePtr == nullptr) { return nullptr; }
        return offsetPtr<::byte>(MutekiStatePtr, 0x198);
    }
    ///����LMTָ�룬ֱ�ӻ�ȡ�Ͳ�������
    int* GetLmtPtr() {
        void* lmtAddr = multiLevelPointerOffset(0x145011760, { 0x50 , 0x468 });
        if (!lmtAddr) { return 0; }
        return offsetPtr<int>(lmtAddr, 0xE9C4);
    }
    ///���ذε�״̬��ַָ��
    int* GetWeaponDrawState() {
        void* weaponDrawPtr = multiLevelPointerOffset(0x145011760, { 0x50 });
        if (weaponDrawPtr == nullptr) { return nullptr; }
        return offsetPtr<int>(weaponDrawPtr, 0x76A8);
    }
    ///��ȡ���һ����Ϣ
    string GetLastMessage() {
        void* MessagePlot = *(undefined**)MH::World::Message;
        if (MessagePlot != nullptr) {
            string Message = offsetPtr<char>(MessagePlot, 0xC0);
            //Message = strcpy(Message, offsetPtr<char>(MessagePlot, 0xC0));
            *offsetPtr<char>(MessagePlot, 0xC0) = *"";
            return UTF8_To_string(Message);
        }
        else
        {
            return "";
        }
    }
    /// <summary>
    /// ���namespace�����˴�LuaEngine������һЩ����
    /// </summary>
    namespace LuaEngine {
        static void* GetPlot(void* plot, const std::vector<int>& bytes) {
            void* Plot = plot;
            //�����ַ
            if ((long long)plot > 0x140000000 && (long long)plot < 0x14579b000) {
                Plot = *(undefined**)plot;
            }
            for (int i : bytes) {
                if (Plot != nullptr) {
                    Plot = *offsetPtr<undefined**>((undefined(*)())Plot, i);
                }
                else {
                    return nullptr;
                }
            }
            return Plot;
        }
        static bool CheckWindows() {
            HWND wnd = GetForegroundWindow();
            HWND mhd = FindWindow(L"MT FRAMEWORK", L"MONSTER HUNTER: WORLD(421652)");
            if (wnd == mhd)
                return true;
            else
                return false;
        }
    }
    /// <summary>
    /// ����Ǵ�LuaEngine��͵���ģ����ð���
    /// </summary>
    namespace Chronoscope {
        struct ChronoscopeData {
            float StartTime = 0;
            float EndTime = 0;
        };

        //���ڵ�ʱ��
        float NowTime = 0;
        //��ʱ���б�
        map<string, ChronoscopeData> ChronoscopeList;
        //��Ӽ�ʱ��(ʱ������ʱ�����ƣ��Ƿ񸲸�)
        static bool AddChronoscope(float duration, string name, bool Overlay = false) {
            if (ChronoscopeList.find(name) == ChronoscopeList.end() || Overlay) {
                ChronoscopeList[name].EndTime = NowTime + duration;
                ChronoscopeList[name].StartTime = NowTime;
                return true;
            }
            else
                return false;
        }
        //ɾ����ʱ��
        static void DelChronoscope(string name) {
            if (ChronoscopeList.find(name) != ChronoscopeList.end()) {
                ChronoscopeList.erase(name);
            }
        }
        //����ʱ���Ƿ����
        static bool CheckPresenceChronoscope(string name) {
            if (ChronoscopeList.find(name) != ChronoscopeList.end()) {
                return true;
            }
            return false;
        }
        //����ʱ���Ƿ����
        static bool CheckChronoscope(string name) {
            if (ChronoscopeList.find(name) != ChronoscopeList.end()) {
                if (ChronoscopeList[name].EndTime < NowTime) {
                    DelChronoscope(name);
                    return true;
                }
                else
                    return false;
            }
            return false;
        }
        //��ʱ�����³���
        static void chronoscope() {
            void* TimePlot = LuaEngine::GetPlot(*(undefined**)MH::Player::PlayerBasePlot, { 0x50, 0x7D20 });
            if (TimePlot != nullptr)
                NowTime = *offsetPtr<float>(TimePlot, 0xC24);
        }
    }

    void SceneSwitch(float nowTime, void (*callBackFunction)()) {
        void* TimePlot = LuaEngine::GetPlot(*(undefined**)MH::Player::PlayerBasePlot, { 0x50, 0x7D20 });
        //void* TimePlot = multiLevelPointerOffset(MH::Player::PlayerBasePlot,{ 0x50, 0x7D20 })
        if (TimePlot != nullptr && nowTime > *offsetPtr<float>(TimePlot, 0xC24)) {
            callBackFunction();
        }
    }
    /// <summary>
    /// ��ȡ��ǰ����id������״̬,����һ��map������["id"] �� ["state"]
    /// </summary>
    /// <returns></returns>
    QuestInfo GetQuestInfo() {
        QuestInfo questInfo;
        void* questBase = *(undefined**)0x14500CAF0;
        questInfo.QuestId = offsetPtr<int>(questBase, 0x4c);
        questInfo.QuestState = offsetPtr<int>(questBase, 0x54);
        return questInfo;
    }
    /// <summary>
    /// ����һ��ActionFrame���ͣ�������ǰ��������Ϣ
    /// </summary>
    /// <returns></returns>
    ActionFrame GetActionFrameInfo() {
        ActionFrame actionFrame;
        void* playerPlot = *(undefined**)MH::Player::PlayerBasePlot;
        void* playerBase = *offsetPtr<void*>(playerPlot, 0x50);
        void* frameBase = *offsetPtr<void*>(playerBase, 0x468);
        actionFrame.frame = offsetPtr<float>(frameBase, 0x10C);
        actionFrame.frameEnd = offsetPtr<float>(frameBase, 0x114);
        int frameRateParam1 = *offsetPtr<int>(playerBase, 0x10);
        void* frameRateParam2 = *(undefined**)0x145121688;
        void* tmp = offsetPtr<void*>(frameRateParam2, frameRateParam1 * 0xf8);
        actionFrame.frameSpeedRate = offsetPtr<float>(tmp, 0xc9);
        return actionFrame;
    }

    void SetQuestState(int state) {
        void* questBase = *(undefined**)0x14500CAF0;
        int* questState = offsetPtr<int>(questBase, 0x38);
        *questState = state;
    }
    /// <summary>
    /// ����Ϸ���������һ����Ϣ
    /// </summary>
    /// <param name="message"></param>
    /// <returns></returns>
    static int ShowMessage(string message) {
        MH::Chat::ShowGameMessage(*(undefined**)MH::Chat::MainPtr, (undefined*)&string_To_UTF8(message)[0], -1, -1, 0);
        return 0;
    }
    typedef struct GuiChat { //���ԭ�ⲻ������hp�Ľṹ
        long long* vtable_ref;
        long long ukptrs[42];
        int chatIndex;
        int unk;
        int isTextBarVisible;
        char space;
        char chatBuffer[256];
    };
    bool sendPlayerMessage(string message) {
        auto strUTF8 = string_To_UTF8(message);
        char msgBuffer[256] = {};
        if (strUTF8.size() > 255) {
            strUTF8 = strUTF8.substr(0, 255);
        }
        ::copy(strUTF8.begin(), strUTF8.end(), msgBuffer);
        long long playerChatBase = 0x1451C2400;
        /*bool* playerChatIdentifier = offsetPtr<bool>(multiLevelPointerOffset(playerChatBase, { 0x13fd0 }), 0x325E);
        GuiChat* chat = offsetPtr<GuiChat>(multiLevelPointerOffset(playerChatBase, { 0x13fd0 }), 0x28f8);*/
        bool* playerChatIdentifier = cheatEnginePtrOffset<bool>(playerChatBase, { 0x13fd0,0x325E });
        GuiChat* chat = cheatEnginePtrOffset<GuiChat>(playerChatBase, { 0x13fd0,0x28f8 });
        memcpy(chat->chatBuffer, msgBuffer, 256);
        *playerChatIdentifier = true;
        return true;
    }

    class OnTimeButton
    {
    public:
        bool IsButtonPressed(int keyID) {
            if (!LuaEngine::CheckWindows()) { return false; }
            if (GetAsyncKeyState(keyID)) {
                if (lockmap.count(keyID) == 0) {
                    lockmap[keyID] = true;
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                auto it = lockmap.find(keyID);
                if (it != lockmap.end()) {
                    lockmap.erase(it);
                }
                return false;
            }
        }
        //std::string KeyIDsToString(const std::vector<int>& keyIDs) {
        //    std::string keyString;
        //    for (int keyID : keyIDs) {
        //        keyString += std::to_string(keyID) + ";";
        //    }
        //    return keyString;
        //}
        //bool IsMutipleButtonPressed(vector<int> keyIDs) {
        //    if (!LuaEngine::CheckWindows()) { return false; }
        //    auto KeyString = KeyIDsToString(keyIDs);
        //    bool allKeysPressed = true;
        //    for (int key : keyIDs) {
        //        if (!GetAsyncKeyState(key)) {
        //            allKeysPressed = false;
        //            break;
        //        }
        //    }
        //    if (allKeysPressed) {
        //        if (groupLockMap.find(KeyString) == groupLockMap.end()) {
        //            groupLockMap[KeyString] = true;
        //            return true; // ���а�������������֮ǰδ����
        //        }
        //        return false; // ������
        //    }
        //    else {
        //        groupLockMap.erase(KeyString); // ����
        //        return false;
        //    }
        //}



    private:
        int button;
        ::map<int, bool> lockmap;
    };
    class onTimeSequenceButtons {
    public:
        onTimeSequenceButtons(const std::vector<int>& expectedSequence)
            : expectedSequence(expectedSequence) {}

        // ��鲢���°���״̬�������⵽���������У��򷵻�true
        bool IsMultipleButtonPressed() {
            using namespace std::chrono;
            if (!LuaEngine::CheckWindows()) { return false; }
            steady_clock::time_point now = steady_clock::now();
            if (duration_cast<milliseconds>(now - lastTriggerTime) < triggerInterval) {
                return false;
            }
            CheckAndUpdateKeyState();
            if (CheckSequenceAndReset()) {
                lastTriggerTime = steady_clock::now(); // ����ʱ���
                return true;
            }
            return false;
        }
    private:
        std::deque<int> keyPressSequence; // ���ڼ�¼����˳��
        std::map<int, bool> keyStateMap; // ����ÿ��������״̬
        std::vector<int> expectedSequence; // �����İ�������
        std::chrono::steady_clock::time_point lastTriggerTime;
        const std::chrono::milliseconds triggerInterval{ 500 }; // 500������


        void UpdateKeyPressSequence(int key) {
            if (keyPressSequence.size() >= expectedSequence.size()) {
                keyPressSequence.pop_front();
            }
            keyPressSequence.push_back(key);
        }

        bool IsKeyPressed(int key) {
            return GetAsyncKeyState(key) & 0x8000;
        }

        void CheckAndUpdateKeyState() {
            for (int key : expectedSequence) {
                bool isCurrentlyPressed = IsKeyPressed(key);
                if (isCurrentlyPressed && !keyStateMap[key]) {
                    UpdateKeyPressSequence(key);
                    keyStateMap[key] = true;
                }
                else if (!isCurrentlyPressed) {
                    keyStateMap[key] = false;
                }
            }
        }

        bool CheckSequenceAndReset() {
            if (IsSequenceMatch()) {
                keyPressSequence.clear();
                for (auto& keyState : keyStateMap) {
                    keyState.second = false;
                }
                return true;
            }
            return false;
        }

        bool IsSequenceMatch() {
            if (keyPressSequence.size() != expectedSequence.size()) {
                return false;
            }
            for (size_t i = 0; i < expectedSequence.size(); ++i) {
                if (keyPressSequence[i] != expectedSequence[i]) {
                    return false;
                }
            }
            return true;
        }
    };
    class ConditionChecker {
    private:
        struct ConditionState {
            double lockThreshold;
            double unlockThreshold;
        };

        std::map<std::string, ConditionState> numericConditions;

        struct TextConditionState {
            std::string lockCondition;
        };

        std::map<std::string, TextConditionState> textConditions;


        struct MultipleConditionState {
            vector<float> lockThresholdArray;
            vector<float> unlockThresholdArray;
        };

        map<string, MultipleConditionState> multipleConditions;

        struct SimpleNumericCondition {
            int lockCondition;
        };

        map <string, SimpleNumericCondition> simpleNumericConditions;
    public:
        // ��ֵ�������
        template<typename T>
        bool CheckNumericCondition(T value, const std::string& identifier, T lockThreshold, T unlockThreshold) {
            auto it = numericConditions.find(identifier);
            if (it != numericConditions.end()) {
                // ����ҵ��˱�ʶ��������Ƿ�Ӧ�ý���
                if (value < it->second.unlockThreshold) {
                    numericConditions.erase(it);  // ��������map���Ƴ�
                    return false;
                }
            }
            else {
                // ���û�ҵ���ʶ��������Ƿ�Ӧ������
                if (value > lockThreshold) {
                    // ��������ӵ�map��
                    numericConditions[identifier] = { static_cast<double>(lockThreshold), static_cast<double>(unlockThreshold) };
                    return true;
                }
            }
            return false;
        }
        template<typename T>
        bool CheckMultipleNumericCondition(const vector<T>& arr, const string& identifier, const vector<T>& lockThresholdArr, const vector<T>& unlockThresholdArr) {
            // ����Ƿ��Ѿ�����
            auto it = multipleConditions.find(identifier);
            if (it != multipleConditions.end()) {
                // ������������Ƿ������������
                bool unlock = true;
                for (size_t i = 0; i < arr.size(); ++i) {
                    if (!(arr[i] < unlockThresholdArr[i])) {
                        unlock = false;
                        break;
                    }
                }
                if (unlock) {
                    // ���������������map���Ƴ�
                    multipleConditions.erase(it);
                }
                return false;
            }
            else {
                // δ����������Ƿ�������������
                bool lock = true;
                for (size_t i = 0; i < arr.size(); ++i) {
                    if (!(arr[i] > lockThresholdArr[i])) {
                        lock = false;
                        break;
                    }
                }
                if (lock) {
                    // ����������������ӵ�map��
                    MultipleConditionState state{ vector<float>(lockThresholdArr.begin(), lockThresholdArr.end()), vector<float>(unlockThresholdArr.begin(), unlockThresholdArr.end()) };
                    multipleConditions[identifier] = state;
                    return true;
                }
            }
            return false;
        }
        /// <summary>
        /// �������ּ��
        /// </summary>
        /// <param name="value"></param>
        /// <param name="identifier"></param>
        /// <param name="lockCondition"></param>
        /// <returns></returns>
        bool CheckSimpleNumericCondition(const int& value, const std::string& identifier, const int& lockCondition) {
            auto it = simpleNumericConditions.find(identifier);
            if (it != simpleNumericConditions.end()) {
                // ����ҵ��˱�ʶ��������ǰֵ���������������������
                if (value != it->second.lockCondition) {
                    simpleNumericConditions.erase(it);  // ��������map���Ƴ�
                    return false;
                }
            }
            else {
                // ���û�ҵ���ʶ��������ǰֵ��������������������
                if (value == lockCondition) {
                    // ��������ӵ�map��
                    simpleNumericConditions[identifier] = { lockCondition };
                    return true;
                }
            }
            return false;
        }
        // �ı��������
        bool CheckTextCondition(const std::string& value, const std::string& identifier, const std::string& lockCondition) {
            auto it = textConditions.find(identifier);
            if (it != textConditions.end()) {
                // ����ҵ��˱�ʶ��������ǰֵ���������������������
                if (value != it->second.lockCondition) {
                    textConditions.erase(it);  // ��������map���Ƴ�
                    return false;
                }
            }
            else {
                // ���û�ҵ���ʶ��������ǰֵ��������������������
                if (value == lockCondition) {
                    // ��������ӵ�map��
                    textConditions[identifier] = { lockCondition };
                    return true;
                }
            }
            return false;
        }
    };
    namespace MHweapon {
        namespace InsectGlaive {
            weaponStruct::InsectGlaive::InsectGlaivePtrs getWeaponData() {
                weaponStruct::InsectGlaive::InsectGlaivePtrs emptyPtrStuct{};
                void* weaponEntity = tool::multiLevelPointerOffset(0x145011760, { 0x50, 0x76B0 });
                if (weaponEntity == nullptr) { return emptyPtrStuct; }
                float* redLight = offsetPtr<float>(weaponEntity, 0x2368);
                float* whiteLight = offsetPtr<float>(weaponEntity, 0x236C);
                float* yellowLight = offsetPtr<float>(weaponEntity, 0x2370);
                float* powerBuff = offsetPtr<float>(weaponEntity, 0x4058);
                float* spriteBuff = offsetPtr<float>(weaponEntity, 0x405C);
                emptyPtrStuct.redLight = redLight;
                emptyPtrStuct.yellowLight = yellowLight;
                emptyPtrStuct.whiteLight = whiteLight;
                emptyPtrStuct.powerBuff = powerBuff;
                emptyPtrStuct.spriteBuff = spriteBuff;
                return emptyPtrStuct;
            }
        }
        namespace GreatSword {
            weaponStruct::GreatSword::GreatSwordPtrs getWeaponData() {
                weaponStruct::GreatSword::GreatSwordPtrs emptyPtrStruct{};
                auto weaponEntity = tool::GetWeaponEntityPtr();
                if (weaponEntity == nullptr) { return emptyPtrStruct; }
                auto chargeLevel = offsetPtr<int>(weaponEntity, 0x2358);
                emptyPtrStruct.chargeLevel = chargeLevel;
                return emptyPtrStruct;
            }
        }
        namespace DualBlades {
            weaponStruct::DualBlades::DualBladesPtrs getWeaponData() {
                weaponStruct::DualBlades::DualBladesPtrs emptyPtrStruct{};
                auto weaponEntity = tool::GetWeaponEntityPtr();
                if (weaponEntity == nullptr) { return emptyPtrStruct; }
                auto demonStatus = *offsetPtr<weaponStruct::DualBlades::DualBladesDemonStatus>(weaponEntity, 0x2368);
                auto fullGaugeStatus = offsetPtr<::byte>(weaponEntity, 0x2369);
                auto demonGauge = offsetPtr<float>(weaponEntity, 0x236C);
                emptyPtrStruct.demonGauge = demonGauge;
                emptyPtrStruct.fullGaugeStatus = fullGaugeStatus;
                emptyPtrStruct.demonStatus = demonStatus;
                return emptyPtrStruct;
            }
        }
        namespace SwitchAxe {
            weaponStruct::SwitchAxe::SwitchAxePtrs getWeaponData() {
                weaponStruct::SwitchAxe::SwitchAxePtrs emptyPtrStruct{};
                auto weaponEntity = tool::GetWeaponEntityPtr();
                if (weaponEntity == nullptr) { return emptyPtrStruct; }
                auto transformStatus = *offsetPtr<::byte>(weaponEntity, 0xAD4);
                auto innerGauge = offsetPtr<float>(weaponEntity, 0x2350);
                auto outerGauge = offsetPtr<float>(weaponEntity, 0x2360);
                auto enhancedTimer = offsetPtr<float>(weaponEntity, 0x2364);
                emptyPtrStruct.transformStatus = static_cast<weaponStruct::SwitchAxe::SwitchAxeStatus>(transformStatus);
                emptyPtrStruct.innerGauge = innerGauge;
                emptyPtrStruct.outerGauge = outerGauge;
                emptyPtrStruct.enhancedTimer = enhancedTimer;
                return emptyPtrStruct;
            }
        }
        namespace GunLance {
            weaponStruct::GunLance::GunLancePtrs getWeaponData() {
                weaponStruct::GunLance::GunLancePtrs emptyPtrStruct{};
                auto weaponEntity = tool::GetWeaponEntityPtr();
                if (weaponEntity == nullptr) { return emptyPtrStruct; }
                auto maxShell = *offsetPtr<int>(weaponEntity, 0x2368);
                auto currentShell = offsetPtr<int>(weaponEntity, 0x2369);
                auto hasWyvernStake = offsetPtr<int>(weaponEntity, 0x236C);
                emptyPtrStruct.maxShell = maxShell;
                emptyPtrStruct.currentShell = currentShell;
                emptyPtrStruct.hasWyvernStake = hasWyvernStake;
                return emptyPtrStruct;
            }
        }
        namespace Hammer {
            weaponStruct::Hammer::HammerPtrs getWeaponData() {
                weaponStruct::Hammer::HammerPtrs emptyPtrStruct{};
                auto weaponEntity = tool::GetWeaponEntityPtr();
                if (weaponEntity == nullptr) { return emptyPtrStruct; }
                auto chargeLevel = offsetPtr<int>(weaponEntity, 0x2368);
                auto hyperChargeStatus = offsetPtr<int>(weaponEntity, 0x2369);
                emptyPtrStruct.hyperChargeStatus = hyperChargeStatus;
                emptyPtrStruct.chargeLevel = chargeLevel;
                return emptyPtrStruct;
            }
        }
        namespace Bow {
            weaponStruct::BowPtrs getWeaponData() {
                weaponStruct::BowPtrs emptyPtrStruct{};
                auto weaponEntity = tool::GetWeaponEntityPtr();
                if (weaponEntity == nullptr) { return emptyPtrStruct; }
                auto chargeLevel = offsetPtr<int>(weaponEntity, 0x23D4);
                emptyPtrStruct.chargeLevel = chargeLevel;
                return emptyPtrStruct;
            }
        }
        namespace ChargeAxe {
            weaponStruct::ChargeAxe::ChargeAxePtrs getWeaponData() {
                weaponStruct::ChargeAxe::ChargeAxePtrs emptyPtrStruct{};
                auto weaponEntity = tool::GetWeaponEntityPtr();
                if (weaponEntity == nullptr) { return emptyPtrStruct; }
                //auto maxBottle = tool::cheatEnginePtrOffset<int>(0x1450EA510, { 0x110,0x98,0x58,0x5F98 });
                auto maxBottlePtr = tool::cheatEnginePtrOffset<int>(0x1450EA510, { 0x110,0x98,0x58,0x5F98 });
                emptyPtrStruct.maxBottle = maxBottlePtr ? *maxBottlePtr : 0;

                auto transformStatus = *offsetPtr<::byte>(weaponEntity, 0xAD4);
                auto shieldCharge = offsetPtr<float>(weaponEntity, 0x2370);
                auto currentBottles = offsetPtr<int>(weaponEntity, 0x2374);
                auto enhancedSheildTimer = offsetPtr<float>(weaponEntity, 0x2378);
                auto enhancedSwordTimer = offsetPtr<float>(weaponEntity, 0x237C);
                auto chainsawBottleTimer = offsetPtr<float>(weaponEntity, 0x2470);
                auto chainsawStatus = *offsetPtr<::byte>(weaponEntity, 0x2475);

                emptyPtrStruct.transformStatus = static_cast<weaponStruct::ChargeAxe::ChargeAxeStatus>(transformStatus);
                emptyPtrStruct.shieldCharge = shieldCharge;
                emptyPtrStruct.currentBottles = currentBottles;
                emptyPtrStruct.enhancedShieldTimer = enhancedSheildTimer;
                emptyPtrStruct.enhancedSwordTimer = enhancedSwordTimer;
                emptyPtrStruct.chainsawDrainBottleTimer = chainsawBottleTimer;
                emptyPtrStruct.chansawStatus = static_cast<weaponStruct::ChargeAxe::ChargeAxeChainsawStatus>(chainsawStatus);
                return emptyPtrStruct;
            }
        }
    }
}