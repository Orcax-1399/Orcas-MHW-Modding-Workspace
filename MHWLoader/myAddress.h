#pragma once
#include "ghidra_export.h"
#include "util.h"
#include "loader.h"

namespace addressResources {
	static undefined(*HitPtr)(undefined*, undefined*) = (undefined(*)(undefined*, undefined*))0x141F5048B;
	/// <summary>
	/// ����1*������ָ��
	/// ����2���˺�
	/// ����3*��������λָ��
	/// ����4���Ƿ��������ֽ�bool��
	/// ����5���Ƿ񱩻������ֽ�bool��
	/// ����6��δ֪0
	/// ����7��δ֪1
	/// ����8��δ֪2
	/// ����9������ID
	/// </summary>
	void (*DealDamageFunction)(void*, int, void*, BOOL, BOOL, int, int, char, int) = (void (*)(void*, int, void*, BOOL, BOOL, int, int, char, int))0x141CC51B0;
	typedef void (*DealDamageFunctionType)(void* target, int damage, void* position, BOOL isTenderized, BOOL isCrit, int unk0, int unk1, char unk2, int attackId);
	

}