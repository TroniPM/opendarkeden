//////////////////////////////////////////////////////////////////////////////
// Filename    : Untransform.cpp
// Written by  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "Untransform.h"
#include "Gpackets/GCSkillToSelfOK1.h"
#include "Gpackets/GCSkillToSelfOK3.h"
#include "Gpackets/GCSkillFailed1.h"
#include "Gpackets/GCDeleteObject.h"
#include "Gpackets/GCRemoveEffect.h"
#include "ZoneUtil.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void Untransform::execute(Vampire * pVampire)
	throw(Error)
{
	__BEGIN_TRY

	//cout << "TID[" << Thread::self() << "]" << getSkillHandlerName() << " Begin" << endl;

	Assert(pVampire != NULL);

	try 
	{
		Zone* pZone = pVampire->getZone();
		Assert(pZone != NULL);
		addUntransformCreature(pZone, pVampire, true);
	} 
	catch(Throwable & t) 
	{
		executeSkillFailException(pVampire, getSkillType());
	}

	//cout << "TID[" << Thread::self() << "]" << getSkillHandlerName() << " End" << endl;

	__END_CATCH

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void Untransform::execute(Ousters * pOusters)
	throw(Error)
{
	__BEGIN_TRY

	//cout << "TID[" << Thread::self() << "]" << getSkillHandlerName() << " Begin" << endl;

	Assert(pOusters != NULL);

	try 
	{
		if ( pOusters->isFlag(Effect::EFFECT_CLASS_SUMMON_SYLPH) )
		{
			Zone* pZone = pOusters->getZone();
			Assert(pZone != NULL);

			Effect* pEffect = pOusters->findEffect(Effect::EFFECT_CLASS_SUMMON_SYLPH);
			if ( pEffect != NULL ) pEffect->setDeadline(0);

			GCSkillToSelfOK1 gcOK1;

			gcOK1.setSkillType( SKILL_UN_TRANSFORM );
			pOusters->getPlayer()->sendPacket(&gcOK1);

			// EffectSummonSylph�� unaffect���� �� ���ش�.

			// ���� ����Ʈ ���ݴٰ� �����ְ�
//			GCRemoveEffect removeEffect;
//			removeEffect.setObjectID(pOusters->getObjectID());
//			removeEffect.addEffectList(Effect::EFFECT_CLASS_SUMMON_SYLPH);
//			pZone->broadcastPacket(pOusters->getX(), pOusters->getY(), &removeEffect);
//			pOusters->getPlayer()->sendPacket(&removeEffect);
//
//			GCModifyInformation gcMI;
//
//			// ���� ���� �ٽ� ����ؼ� �����ְ�
//			OUSTERS_RECORD prev;
//			pOusters->getOustersRecord(prev);
//			pOusters->initAllStat();
//			pOusters->addModifyInfo(prev, gcMI);

//			pOusters->getPlayer()->sendPacket(&gcMI);
		}
		else
		{
			GCSkillFailed1 gcFail;
			gcFail.setSkillType( SKILL_UN_TRANSFORM );
			pOusters->getPlayer()->sendPacket(&gcFail);
		}
	} 
	catch(Throwable & t) 
	{
//		Ŭ���̾�Ʈ���� -_- �̷� �� ������ ������ �� �Ф�
//		executeSkillFailException(pOusters, getSkillType());
	}

	//cout << "TID[" << Thread::self() << "]" << getSkillHandlerName() << " End" << endl;

	__END_CATCH

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void Untransform::execute(Monster* pMonster)
	throw(Error)
{
	__BEGIN_TRY

	//cout << "TID[" << Thread::self() << "]" << getSkillHandlerName() << " Begin" << endl;

	Assert(pMonster != NULL);

	try 
	{
		Zone* pZone = pMonster->getZone();
		Assert(pZone != NULL);
		addUntransformCreature(pZone, pMonster, true);
	} 
	catch(Throwable & t) 
	{
		//cout << t.toString() << endl;
	}
	
	//cout << "TID[" << Thread::self() << "]" << getSkillHandlerName() << " End" << endl;

	__END_CATCH

}

Untransform g_Untransform;
