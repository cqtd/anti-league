#pragma once

#define OFFSET(NAME, OFFSET) constexpr unsigned long NAME = OFFSET

namespace offset
{
	namespace instance
	{
		OFFSET(LocalPlayer, 0x34E0280);
		
		OFFSET(ListManager, 0x1C3B63C);
		OFFSET(MinionManager, 0x1C3D790);
		
		OFFSET(Renderer, 0x3507670);
		OFFSET(ViewMatrix, 0x3504990);
		
		OFFSET(UnderMouseObject, 0x34D0254);
	}

	namespace object
	{
		OFFSET(Index,	0x20);
		OFFSET(Team,	0x4C);
		OFFSET(Name,	0x6C);

		OFFSET(Position, 0x1D8);
		OFFSET(Visibility, 0x48C);
	}

	namespace attackable_unit
	{
		OFFSET(Max, 0x10);
		
		OFFSET(Mana, 0x298);
		OFFSET(MaxMana, Max + Mana);

		OFFSET(Health, 0xDC4);
		OFFSET(MaxHealth, Max + Health);

		OFFSET(MaxMovementSpeed, 0x12C4);
		OFFSET(AttackRange, 0x12CC);

		OFFSET(BaseAD, 0x146C);
		OFFSET(BonusAD, 0x13EC);
	}

	namespace ai_hero
	{
		OFFSET(ChampionName, 0x312C);
	}

#ifdef _EXTERNAL

	namespace function
	{
		// 10.16
		OFFSET(IsMinion, 0x1CB320);
		OFFSET(IsHero, 0x1CB2E0);
		OFFSET(IsMissile, 0x1CB340);
		OFFSET(IsTurret, 0x1CB4D0);
		OFFSET(IsTargetable, 0x1EEFF0);
		OFFSET(IsAlive, 0x1B75C0);
		OFFSET(IsTroy, 0x1CB1E0);
	}
	
#endif
}