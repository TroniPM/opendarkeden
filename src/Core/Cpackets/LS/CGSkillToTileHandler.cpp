//////////////////////////////////////////////////////////////////////////////
// Filename    : CGSkillToTileHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//////////////////////////////////////////////////////////////////////////////

#include "CGSkillToTile.h"

#ifdef __GAME_SERVER__
	#include "GamePlayer.h"
	#include "SkillHandlerManager.h"
	#include "Creature.h"
	#include "ZoneUtil.h"
	
	#include "skill/Sniping.h"

	#include "Gpackets/GCSkillFailed1.h"

	//#define __PROFILE_SKILLS__

	#ifdef __PROFILE_SKILLS__
		#include "Profile.h"
	#endif
#endif	// __GAME_SERVER__

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CGSkillToTileHandler::execute (CGSkillToTile* pPacket , Player* pPlayer)
	 throw (Error)
{
	__BEGIN_TRY __BEGIN_DEBUG_EX
		
#ifdef __GAME_SERVER__

	Assert(pPacket != NULL);
	Assert(pPlayer != NULL);

	try 
	{
		// Ư����� �׽�Ʈ�� ���ؼ� �ӽ÷� �־�δ� �ڵ��̴�.
		GamePlayer* pGamePlayer = dynamic_cast<GamePlayer*>(pPlayer);

		Assert(pGamePlayer != NULL);	// by sigi

		if (pGamePlayer->getPlayerStatus() == GPS_NORMAL) 
		{
			Creature* pCreature = pGamePlayer->getCreature();
			Assert(pCreature != NULL);	// by sigi

			Zone* pZone = pCreature->getZone();
			Assert(pZone != NULL);
		
			SkillType_t SkillType = pPacket->getSkillType();

			// ���� ���������� ��� ��� �Ұ�. by sigi. 2002.11.14
			ZoneLevel_t ZoneLevel = pZone->getZoneLevel(pCreature->getX(), pCreature->getY());
			if (ZoneLevel & COMPLETE_SAFE_ZONE)
			{
				GCSkillFailed1 _GCSkillFailed1;
				_GCSkillFailed1.setSkillType(SkillType);
				pPlayer->sendPacket(&_GCSkillFailed1);

				return;
			}

			disableFlags( pCreature, pZone, SkillType);
		
			if (pCreature->isSlayer()) 
			{
				Slayer*    pSlayer    = dynamic_cast<Slayer*>(pCreature);
				SkillSlot* pSkillSlot = ((Slayer *)pCreature)->hasSkill(SkillType);
				bool       bSuccess   = true;

				if (pSkillSlot == NULL) bSuccess = false;
				if (!isAbleToUseTileSkill(pSlayer)) bSuccess = false;

/*				if (pCreature->isSlayer() && pCreature->isFlag(Effect::EFFECT_CLASS_SNIPING_MODE))
				{
					Slayer* pSlayer = dynamic_cast<Slayer*>(pCreature);
					Assert(pSlayer != NULL);
					g_Sniping.checkRevealRatio(pSlayer, 20, 10);
				} */

				if (bSuccess)
				{
					SkillHandler* pSkillHandler = g_pSkillHandlerManager->getSkillHandler(SkillType);
					Assert(pSkillHandler != NULL);

					#ifdef __PROFILE_SKILLS__
						beginProfileEx(SkillTypes2String[SkillType]);
						pSkillHandler->execute(pSlayer, pPacket->getX(), pPacket->getY(), pSkillSlot, pPacket->getCEffectID());
						endProfileEx(SkillTypes2String[SkillType]);
					#else
						pSkillHandler->execute(pSlayer, pPacket->getX(), pPacket->getY(), pSkillSlot, pPacket->getCEffectID());
					#endif
				}
				else
				{
					GCSkillFailed1 _GCSkillFailed1;
					_GCSkillFailed1.setSkillType(SkillType);
					pPlayer->sendPacket(&_GCSkillFailed1);
				}
			} 
			else if (pCreature->isVampire()) 
			{
				Vampire*          pVampire          = dynamic_cast<Vampire*>(pCreature);
				VampireSkillSlot* pVampireSkillSlot = pVampire->hasSkill(SkillType);
				bool              bSuccess          = true;

				if (SkillType == SKILL_EAT_CORPSE && pVampire->isFlag(Effect::EFFECT_CLASS_TRANSFORM_TO_WOLF))
				{
					SkillHandler* pSkillHandler = g_pSkillHandlerManager->getSkillHandler(SKILL_EAT_CORPSE);
					Assert(pSkillHandler != NULL);
					pSkillHandler->execute(pVampire, pPacket->getX(), pPacket->getY(), pVampireSkillSlot, pPacket->getCEffectID());
					return;
				}

				if (pVampireSkillSlot == NULL) bSuccess = false;
				if (!isAbleToUseTileSkill(pVampire)) bSuccess = false;

/*				if (pVampire->isFlag(Effect::EFFECT_CLASS_INVISIBILITY))
				{
					addVisibleCreature(pZone, pVampire, true);
				}

                if (pVampire->isFlag(Effect::EFFECT_CLASS_EXTREME))
	            {
	            	EffectManager * pEffectManager = pVampire->getEffectManager();
	            	Assert( pEffectManager != NULL );
	            	Effect * pEffect = pEffectManager->findEffect( Effect::EFFECT_CLASS_EXTREME );
	            	if ( pEffect != NULL ) {
	            		pEffect->setDeadline(0);
	            	}
				} */
				if (bSuccess)
				{
					SkillHandler* pSkillHandler = g_pSkillHandlerManager->getSkillHandler(SkillType);
					Assert(pSkillHandler != NULL);

					#ifdef __PROFILE_SKILLS__
						beginProfileEx(SkillTypes2String[SkillType]);
						pSkillHandler->execute(pVampire, pPacket->getX(), pPacket->getY(), pVampireSkillSlot, pPacket->getCEffectID());
						endProfileEx(SkillTypes2String[SkillType]);
					#else
						pSkillHandler->execute(pVampire, pPacket->getX(), pPacket->getY(), pVampireSkillSlot, pPacket->getCEffectID());
					#endif
				}
				else
				{
					GCSkillFailed1 _GCSkillFailed1;
					_GCSkillFailed1.setSkillType(SkillType);
					pPlayer->sendPacket(&_GCSkillFailed1);
				}
			} 
			else if (pCreature->isOusters()) 
			{
				Ousters*          pOusters          = dynamic_cast<Ousters*>(pCreature);
				OustersSkillSlot* pOustersSkillSlot = pOusters->hasSkill(SkillType);
				bool              bSuccess          = true;

				if (pOustersSkillSlot == NULL) bSuccess = false;
				if (!isAbleToUseTileSkill(pOusters)) bSuccess = false;

				if (bSuccess)
				{
					SkillHandler* pSkillHandler = g_pSkillHandlerManager->getSkillHandler(SkillType);
					Assert(pSkillHandler != NULL);

					#ifdef __PROFILE_SKILLS__
						beginProfileEx(SkillTypes2String[SkillType]);
						pSkillHandler->execute(pOusters, pPacket->getX(), pPacket->getY(), pOustersSkillSlot, pPacket->getCEffectID());
						endProfileEx(SkillTypes2String[SkillType]);
					#else
						pSkillHandler->execute(pOusters, pPacket->getX(), pPacket->getY(), pOustersSkillSlot, pPacket->getCEffectID());
					#endif
				}
				else
				{
					GCSkillFailed1 _GCSkillFailed1;
					_GCSkillFailed1.setSkillType(SkillType);
					pPlayer->sendPacket(&_GCSkillFailed1);
				}
			} 
		}
	} 
	catch(Throwable & t) 
	{
		//cout << t.toString() << endl;
	}

#endif	// __GAME_SERVER__
		
	__END_DEBUG_EX __END_CATCH
}

