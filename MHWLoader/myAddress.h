#pragma once
#include "ghidra_export.h"
#include "util.h"
#include "loader.h"

namespace addressResources {
	static undefined(*HitPtr)(undefined*, undefined*) = (undefined(*)(undefined*, undefined*))0x141F5048B;
	/// <summary>
	/// 参数1*：怪物指针
	/// 参数2：伤害
	/// 参数3*：攻击部位指针
	/// 参数4：是否软化（四字节bool）
	/// 参数5：是否暴击（四字节bool）
	/// 参数6：未知0
	/// 参数7：未知1
	/// 参数8：未知2
	/// 参数9：攻击ID
	/// </summary>
	void (*DealDamageFunction)(void*, int, void*, BOOL, BOOL, int, int, char, int) = (void (*)(void*, int, void*, BOOL, BOOL, int, int, char, int))0x141CC51B0;
	typedef void (*DealDamageFunctionType)(void* target, int damage, void* position, BOOL isTenderized, BOOL isCrit, int unk0, int unk1, char unk2, int attackId);
	

}