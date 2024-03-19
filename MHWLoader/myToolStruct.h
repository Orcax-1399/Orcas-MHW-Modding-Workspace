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
    /// ��ͼid�͵�������id
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
        //����ֶβ��ܸ���
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
        //�����ֻ����
        DualBladesDemonStatus demonStatus;
        //�Ҳ����취�ȱ���ָ���ֱ���enumClass
        ::byte* fullGaugeStatus;
        float* demonGauge;
    };
    }
   namespace ChargeAxe{
    enum class ChargeAxeStatus {
        SWORD = 1,
        AXE = 100,
        //���ֻ������chansawStatus�ֶε��б�
    };
    enum class ChargeAxeChainsawStatus {
        CHAINSAW = 1,
        NOT_CHAINSAW = 0
    };
    struct ChargeAxePtrs {
        int maxBottle;
        //�������س��ܣ�����������ն����ɫ����ƿ���Ǹ�
        float* shieldCharge;
        int* currentBottles;
        //��ܼ�ʱ��
        float* enhancedShieldTimer;
        //�콣��ʱ��
        float* enhancedSwordTimer;
        //�����̬������ƿ�ӵĵ�����ʱ��
        float* chainsawDrainBottleTimer;
        //����̬���߸���̬��ʶ
        ChargeAxeStatus transformStatus;
        //�����̬��ʶ
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
        //��Ȧ�Ľ������ۣ���ģʽ����
        float* innerGauge;
        //��Ȧ�ĳ����������
        float* outerGauge;
        //������״̬��ʱ��
        float* enhancedTimer;
    };
   }
   namespace GunLance{
    struct GunLancePtrs {
        int maxShell;
        //��ǰ�ص���
        int* currentShell;
        //������װ������������1�����У�0����û��
        int* hasWyvernStake;
    };
   }
}