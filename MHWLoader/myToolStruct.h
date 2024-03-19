#pragma once

namespace toolStruct {
    enum MutekiState
    {
        Normal = 1,
        Escape = 2,
        Hyper = 10,
    };
    enum class PlayerWeaponType
    {
        ErrorWeapon = -1,
        GreatSword = 0,
        SwordShield = 1,
        DualBlades = 2,
        LongSword = 3,
        Hammer = 4,
        HuntingHorn = 5,
        Lance = 6,
        Gunlance = 7,
        SwitchAxe = 8,
        ChargeBlade = 9,
        InsectGlaive = 10,
        Bow = 11,
        HeavyBowGun = 12,
        LightBowGun = 13,
    };
    struct QuestInfo
    {
        int* QuestId;
        int* QuestState;
    };
    struct ActionFrame
    {
        float* frame;
        float* frameEnd;
        float* frameSpeedRate;
    };
    struct PlayerWeaponInfo
    {
        PlayerWeaponType type;
        int id;
        int* sharpness;
    };

    struct coordinate
    {
        float x;
        float y;
        float z;
    };

    /// <summary>
    /// 地图id和导航坐标id
    /// </summary>
    struct WorldInfo {
        int mapId;
        coordinate wayPointCoordinate;
        
    };
    struct FsmInfo {
        int fsmTarget;
        int fsmId;
    };
    
}
namespace weaponStruct {
    namespace InsectGlaive {

    
    struct InsectGlaivePtrs
    {
        float* redLight;
        float* yellowLight;
        float* whiteLight;
        float* powerBuff;
        float* spriteBuff;
    };
    }
    namespace GreatSword {

    
    struct GreatSwordPtrs {
        int* chargeLevel;
    };
    }
    namespace Bow{
    enum class BowChargeLevel {
        Lv0 = 0xFFFFFFFF,
        Lv1 = 1,
        Lv2 = 2,
        Lv3 = 3
    };
    }
    struct BowPtrs {
        //这个字段不能更改
        int* chargeLevel;
    };
    namespace Hammer{
    struct HammerPtrs {
        int* hyperChargeStatus;
        int* chargeLevel;
    };
    }
    namespace DualBlades{
    enum class DualBladesDemonStatus {
        NORMAL = 0,
        DEMON = 1,
    };
    enum DualBladesDemonGauge {
        FULL_GAUGE = 1,
        NOT_FULL = 0,
    };
    struct DualBladesPtrs {
        //这个是只读的
        DualBladesDemonStatus demonStatus;
        //找不到办法既保留指针又保留enumClass
        ::byte* fullGaugeStatus;
        float* demonGauge;
    };
    }
   namespace ChargeAxe{
    enum class ChargeAxeStatus {
        SWORD = 1,
        AXE = 100,
        //这个只能用于chansawStatus字段的判别
    };
    enum class ChargeAxeChainsawStatus {
        CHAINSAW = 1,
        NOT_CHAINSAW = 0
    };
    struct ChargeAxePtrs {
        int maxBottle;
        //盾牌隐藏充能，就是你蓄力斩到红色轮廓瓶子那个
        float* shieldCharge;
        int* currentBottles;
        //红盾计时器
        float* enhancedShieldTimer;
        //红剑计时器
        float* enhancedSwordTimer;
        //电锯形态下消耗瓶子的倒数计时器
        float* chainsawDrainBottleTimer;
        //剑形态或者斧形态标识
        ChargeAxeStatus transformStatus;
        //电锯形态标识
        ChargeAxeChainsawStatus chansawStatus;
    };
   }
   namespace SwitchAxe{
    enum class SwitchAxeStatus {
        SWORD = 117,
        AXE = 1,
    };
    struct  SwitchAxePtrs {
        SwitchAxeStatus transformStatus;
        //内圈的剑能量槽，剑模式消耗
        float* innerGauge;
        //外圈的超出力激活槽
        float* outerGauge;
        //超出力状态计时器
        float* enhancedTimer;
    };
   }
   namespace GunLance{
    struct GunLancePtrs {
        int maxShell;
        //当前载弹量
        int* currentShell;
        //龙杭炮装填数量，反正1就是有，0就是没有
        int* hasWyvernStake;
    };
   }
}