//////////////////////////////////////////////////////////////////////////////
// Filename    : Shape.cpp
// Written by  : excel96
// Description : 
// �α��� ȭ�鿡�� �����̾��� �ܾ��� �����ִ� �� �ʿ��� �Լ���.
// ������ Ÿ�Կ� ���� �Ѹ�� ����� �����ϴ� �Լ����̴�.
//////////////////////////////////////////////////////////////////////////////

#include "Shape.h"

PantsType getPantsType(ItemType_t IType)
	throw()
{
	switch (IType)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			return PANTS1;
		case 4:
		case 5:
		case 6:
		case 7:
			return PANTS2;
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
			return PANTS3;
		default:
			throw ("getPantsType() : unknown pants type");
			break;
	}

	return PANTS_BASIC;
}

JacketType getJacketType(ItemType_t IType)
	throw()
{
	switch (IType)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			return JACKET1;
		case 4:
		case 5:
		case 6:
		case 7:
			return JACKET2;
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
			return JACKET3;
		default:
			throw ("getJacketType() : unknown jacket type");
			break;
	}

	return JACKET_BASIC;
}

HelmetType getHelmetType(ItemType_t IType)
	throw()
{
	switch (IType)
	{
		case 0:
		case 1:
		case 2:
			return HELMET1;
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
			return HELMET2;
		default:
			throw ("getHelmetType() : unknown helmet type");
			break;
	}

	return HELMET_NONE;
}

ShieldType getShieldType(ItemType_t IType)
	throw()
{
	switch (IType)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			return SHIELD1;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
			return SHIELD2;
		default:
			throw ("getShieldType() : unknown shield type");
			break;
	}

	return SHIELD_NONE;
}

VampireCoatType getVampireCoatType(ItemType_t IType)
	throw()
{
	switch (IType)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			return VAMPIRE_COAT1;
		case 4:
		case 5:
		case 6:
		case 7:
			return VAMPIRE_COAT2;
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
			return VAMPIRE_COAT3;

		default:
			throw ("getVampireCoatType() : unknown VampireCoat type");
			break;
	}

	return VAMPIRE_COAT_BASIC;
}

MotorcycleType getMotorcycleType(ItemType_t IType)
	throw()
{
	switch (IType)
	{
		case 0:
		case 5:
			return MOTORCYCLE1;
		case 1:
			return MOTORCYCLE2;
		default:
			throw ("getMotorcycleType() : unknown Motorcycle type");
			break;
	}

	return MOTORCYCLE_NONE;
}

OustersCoatType getOustersCoatType(ItemType_t IType)
	throw()
{
	switch ( IType )
	{
		case 0:
		case 1:
		case 2:
			return OUSTERS_COAT1;
		case 3:
		case 4:
		case 5:
			return OUSTERS_COAT2;
		case 6:
		case 7:
		case 8:
		case 9:
			return OUSTERS_COAT3;
	}

	return OUSTERS_COAT_BASIC;
}

OustersArmType getOustersArmType(ItemType_t IType)
	throw()
{
	switch ( IType )
	{
		case 0:
			return OUSTERS_ARM_GAUNTLET;
	}

	return OUSTERS_ARM_GAUNTLET;
}

