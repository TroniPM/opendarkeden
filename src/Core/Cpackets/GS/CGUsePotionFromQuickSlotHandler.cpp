//////////////////////////////////////////////////////////////////////////////
// Filename    : CGUsePotionFromQuickSlotHandler.cc
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "CGUsePotionFromQuickSlot.h"

#ifdef __GAME_SERVER__
	#include "GamePlayer.h"
	#include "Zone.h"
	#include "Slayer.h"
	#include "Ousters.h"
	#include "Inventory.h"
	#include "Item.h"
	#include "ItemInfo.h"
	#include "ItemUtil.h"
	#include <math.h>
	#include "EffectHPRecovery.h"
	#include "EffectMPRecovery.h"

	#include "item/Belt.h"
	#include "item/OustersArmsband.h"
	#include "item/Potion.h"
	#include "item/Pupa.h"
	#include "item/ComposMei.h"

	#include "Gpackets/GCHPRecoveryStartToSelf.h"
	#include "Gpackets/GCHPRecoveryStartToOthers.h"
	#include "Gpackets/GCMPRecoveryStart.h"
	#include "Gpackets/GCUseOK.h"
	#include "Gpackets/GCCannotUse.h"
#endif

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CGUsePotionFromQuickSlotHandler::execute (CGUsePotionFromQuickSlot* pPacket , Player* pPlayer)
	throw (ProtocolException, Error)
{
	__BEGIN_TRY __BEGIN_DEBUG_EX

#ifdef __GAME_SERVER__

	Assert(pPacket != NULL);
	Assert(pPlayer != NULL);
	GamePlayer* pGamePlayer = dynamic_cast<GamePlayer*>(pPlayer);
	Creature*   pCreature   = pGamePlayer->getCreature();

	try 
	{
		if ( pCreature->isSlayer() )
		{
			GamePlayer* pGamePlayer = dynamic_cast<GamePlayer*>(pPlayer);
			Creature*   pCreature   = pGamePlayer->getCreature();

			if (!pCreature->isSlayer()) return;

			// �ڸ� ���¶�� ����� �� ����.
			if (pCreature->isFlag(Effect::EFFECT_CLASS_COMA))
			{
				GCCannotUse _GCCannotUse;
				_GCCannotUse.setObjectID(pPacket->getObjectID());
				pGamePlayer->sendPacket(&_GCCannotUse);
				return;
			}

			Slayer* pSlayer = dynamic_cast<Slayer*>(pCreature);
			Zone*   pZone   = pSlayer->getZone();
			Item*   pBelt   = pSlayer->getWearItem(Slayer::WEAR_BELT);
			
			if (pBelt == NULL)
			{
				GCCannotUse _GCCannotUse;
				_GCCannotUse.setObjectID(pPacket->getObjectID());
				pGamePlayer->sendPacket(&_GCCannotUse);
				return;
			}

			// ��Ʈ�� �κ��丮�� �޾ƿ´�.
			Inventory* pBeltInventory = ((Belt*)pBelt)->getInventory();
			
			// SlotID�� �޴´�.
			SlotID_t SlotID = pPacket->getSlotID();

			// ������ ������ �ʰ��ϸ� ����� �� ����.
			if (SlotID >= pBeltInventory->getWidth())
			{
				GCCannotUse _GCCannotUse;
				_GCCannotUse.setObjectID(pPacket->getObjectID());
				pGamePlayer->sendPacket(&_GCCannotUse);
				return;
			}
			
			// ��Ʈ�� ���� ������ �������� �޾ƿ´�.
			Item* pBeltItem = pBeltInventory->getItem(SlotID, 0);
			
			// �������� ������ �翬�� ����� �� ����.
			if (pBeltItem == NULL)
			{
				GCCannotUse _GCCannotUse;
				_GCCannotUse.setObjectID(pPacket->getObjectID());
				pGamePlayer->sendPacket(&_GCCannotUse);
				return;
			}
			
			// ������ �ִ� �������� Object�� �޴´�.
			ObjectID_t ItemObjectID = pBeltItem->getObjectID();

			// �����Ͱ� ��ġ���� �ʰų�, ������ �ƴ϶�� ����� �� ����.
			if (ItemObjectID != pPacket->getObjectID() || pBeltItem->getItemClass() != Item::ITEM_CLASS_POTION)
			{
				GCCannotUse _GCCannotUse;
				_GCCannotUse.setObjectID(pPacket->getObjectID());
				pGamePlayer->sendPacket(&_GCCannotUse);
				return;
			}

			HP_t    MaxHP        = pSlayer->getHP(ATTR_MAX);
			HP_t    CurrentHP    = pSlayer->getHP(ATTR_CURRENT);
			MP_t    MaxMP        = pSlayer->getMP(ATTR_MAX);
			MP_t    CurrentMP    = pSlayer->getMP(ATTR_CURRENT);
			Potion* pPotion      = dynamic_cast<Potion*>(pBeltItem);

			// ���Ͽ� ȸ���Ǵ� ��
			int		HPQuantity = pPotion->getHPQuantity();
			int		MPQuantity = pPotion->getMPQuantity();

			// ������ ���� �ΰ�.
			int		HPDelayProvider = pPotion->getHPDelay();
			int		MPDelayProvider = pPotion->getMPDelay();

			Attr_t INT = pSlayer->getINT();

			HP_t    PotionHPAmount = 0, PotionMPAmount = 0;

			PotionHPAmount = pPotion->getHPAmount();
			PotionMPAmount = pPotion->getMPAmount();

			int     HPAmount     = min(MaxHP - CurrentHP , (int)PotionHPAmount);
			int     MPAmount     = min(MaxMP - CurrentMP , (int)(pPotion->getMPAmount()* (1 + (double)((double)INT / 300.0))));
			bool    notRecoverHP = false;
			bool    notRecoverMP = false;

			// Activation Effect�� �ɷ��ִٸ� ȸ���ӵ��� 2�谡 �ȴ�.
			if (pSlayer->isFlag(Effect::EFFECT_CLASS_ACTIVATION))
			{
				if ( pPotion->getItemType() >= 14 && pPotion->getItemType() <= 17 )
				{
				}
				else
				{
					HPDelayProvider = (HPDelayProvider>>1);
					MPDelayProvider = (MPDelayProvider>>1);

					HPDelayProvider = max(HPDelayProvider, 1);
					MPDelayProvider = max(MPDelayProvider, 1);
				}
			}


			// HP ȸ������ �����Ѵٸ�...
			if (HPAmount != 0 && HPQuantity != 0)
			{
				if (CurrentHP < MaxHP) 
				{
					EffectManager* pEffectManager = pSlayer->getEffectManager();

					double temp     = (double)((double)HPAmount/(double)HPQuantity);
					uint   Period   = (uint)ceil(temp);
					Turn_t Deadline = Period* HPDelayProvider;

					if (pSlayer->isFlag(Effect::EFFECT_CLASS_HP_RECOVERY)) 
					{
						Effect* pEffect = pEffectManager->findEffect(Effect::EFFECT_CLASS_HP_RECOVERY);
						EffectHPRecovery* pEffectHPRecoveryEffect = dynamic_cast<EffectHPRecovery*>(pEffect);

						// ������ ������� Ƚ���� ä��� HP���� ����Ѵ�.
						// �װ��� ���� ȸ���翡 ���Ѵ�.
						int PrevHPAmount = pEffectHPRecoveryEffect->getHPQuantity()* pEffectHPRecoveryEffect->getPeriod();
						HPAmount = min((int)(HPAmount + PrevHPAmount), MaxHP - CurrentHP);

						// �� �߿� ū ����ȸ����� ���� �����̸� ����.
						HPQuantity      = max(HPQuantity,      (int)(pEffectHPRecoveryEffect->getHPQuantity()));
						HPDelayProvider = min(HPDelayProvider, (int)(pEffectHPRecoveryEffect->getDelay()));

						// ���� ȸ������ ������, �󸶾� ����� ȸ���� ���ΰ��� �����Ѵ�.
						temp     = (double)((double)HPAmount/(double)HPQuantity);
						Period   = (uint)ceil(temp);
						Deadline = Period* HPDelayProvider;

						// HP Recovery effect�� �����Ѵ�.
						pEffectHPRecoveryEffect->setDeadline(Deadline);
						pEffectHPRecoveryEffect->setDelay(HPDelayProvider);
						pEffectHPRecoveryEffect->setHPQuantity(HPQuantity);
						pEffectHPRecoveryEffect->setPeriod(Period);

						// ȸ�� �����϶�� ��Ŷ�� �ڽſ��� ������.
						GCHPRecoveryStartToSelf gcHPRecoveryStartToSelf;
						gcHPRecoveryStartToSelf.setPeriod(pEffectHPRecoveryEffect->getPeriod());
						gcHPRecoveryStartToSelf.setDelay(pEffectHPRecoveryEffect->getDelay());
						gcHPRecoveryStartToSelf.setQuantity(pEffectHPRecoveryEffect->getHPQuantity());

						pGamePlayer->sendPacket(&gcHPRecoveryStartToSelf);

						// ȸ�� �����϶�� ��Ŷ�� �ٸ��̵鿡�� ������.
						// ȸ�� ���� ��Ŷ, ���۰� �� ���� ��Ŷ�� ������.
						GCHPRecoveryStartToOthers gcHPRecoveryStartToOthers;
						gcHPRecoveryStartToOthers.setObjectID(pSlayer->getObjectID());
						gcHPRecoveryStartToOthers.setPeriod(pEffectHPRecoveryEffect->getPeriod());
						gcHPRecoveryStartToOthers.setDelay(pEffectHPRecoveryEffect->getDelay());
						gcHPRecoveryStartToOthers.setQuantity(pEffectHPRecoveryEffect->getHPQuantity());

						pZone->broadcastPacket(pSlayer->getX(), pSlayer->getY(), &gcHPRecoveryStartToOthers, pSlayer);
						GCUseOK _GCUseOK;
						pGamePlayer->sendPacket(&_GCUseOK);
					} 
					else 
					{
						EffectHPRecovery* pEffectHPRecovery = new EffectHPRecovery();

						pEffectHPRecovery->setTarget(pSlayer);
						pEffectHPRecovery->setDeadline(Deadline);
						pEffectHPRecovery->setDelay(HPDelayProvider);
						pEffectHPRecovery->setNextTime(0);
						pEffectHPRecovery->setHPQuantity(HPQuantity);
						pEffectHPRecovery->setPeriod(Period);

						pEffectManager->addEffect(pEffectHPRecovery);

						// ȸ�� �����϶�� ��Ŷ�� �ڽſ��� ������.
						GCHPRecoveryStartToSelf gcHPRecoveryStartToSelf;
						gcHPRecoveryStartToSelf.setPeriod(Period);
						gcHPRecoveryStartToSelf.setDelay(HPDelayProvider);
						gcHPRecoveryStartToSelf.setQuantity(HPQuantity);

						pGamePlayer->sendPacket(&gcHPRecoveryStartToSelf);

						// ȸ�� �����϶�� ��Ŷ�� �����̵鿡�� ������.
						GCHPRecoveryStartToOthers gcHPRecoveryStartToOthers;
						gcHPRecoveryStartToOthers.setObjectID(pSlayer->getObjectID());
						gcHPRecoveryStartToOthers.setPeriod(Period);
						gcHPRecoveryStartToOthers.setDelay(HPDelayProvider);
						gcHPRecoveryStartToOthers.setQuantity(HPQuantity);

						pZone->broadcastPacket(pSlayer->getX(), pSlayer->getY(), &gcHPRecoveryStartToOthers, pSlayer);
						GCUseOK _GCUseOK;
						pGamePlayer->sendPacket(&_GCUseOK);
					}
				} 
				else 
				{
					GCCannotUse _GCCannotUse;
					_GCCannotUse.setObjectID(pPacket->getObjectID());
					pGamePlayer->sendPacket(&_GCCannotUse);
					return;
				}
			} 
			else 
			{
				notRecoverHP = true;
			}

			// MP ȸ������ �����Ѵٸ�...
			if (MPAmount != 0 && MPQuantity != 0 )
			{
				// �󸶾� ��� ���ʸ���.
				if (CurrentMP < MaxMP) 
				{
					EffectManager* pEffectManager = pSlayer->getEffectManager();

					double temp     = (double)((double)MPAmount/(double)MPQuantity);
					uint   Period   = (uint)ceil(temp);
					Turn_t Deadline = Period* MPDelayProvider;

					if (pSlayer->isFlag(Effect::EFFECT_CLASS_MP_RECOVERY)) 
					{
						Effect* pEffect = pEffectManager->findEffect(Effect::EFFECT_CLASS_MP_RECOVERY);
						EffectMPRecovery* pEffectMPRecoveryEffect = dynamic_cast<EffectMPRecovery*>(pEffect);

						// ������ ������� Ƚ���� ä��� MP���� ����Ѵ�.
						// �װ��� ���� ȸ���翡 ���Ѵ�.
						int PrevMPAmount = pEffectMPRecoveryEffect->getMPQuantity()* pEffectMPRecoveryEffect->getPeriod();
						MPAmount = min((int)(MPAmount + PrevMPAmount), MaxMP - CurrentMP);

						// �� �߿� ū ����ȸ����� ���� �����̸� ����.
						MPQuantity      = max(MPQuantity,      (int)(pEffectMPRecoveryEffect->getMPQuantity()));
						MPDelayProvider = min(MPDelayProvider, (int)(pEffectMPRecoveryEffect->getDelay()));

						// ���� ȸ������ ������, �󸶾� ����� ȸ���� ���ΰ��� �����Ѵ�.
						temp     = (double)((double)MPAmount/(double)MPQuantity);
						Period   = (uint)ceil(temp);
						Deadline = Period* MPDelayProvider;

						// MP Recovery effect�� �����Ѵ�.
						pEffectMPRecoveryEffect->setDeadline(Deadline);
						pEffectMPRecoveryEffect->setDelay(MPDelayProvider);
						pEffectMPRecoveryEffect->setMPQuantity(MPQuantity);
						pEffectMPRecoveryEffect->setPeriod(Period);

						// ȸ�� �����϶�� ��Ŷ�� �ڽſ��� ������.
						GCMPRecoveryStart gcMPRecoveryStart;
						gcMPRecoveryStart.setPeriod(pEffectMPRecoveryEffect->getPeriod());
						gcMPRecoveryStart.setDelay(pEffectMPRecoveryEffect->getDelay());
						gcMPRecoveryStart.setQuantity(pEffectMPRecoveryEffect->getMPQuantity());

						pGamePlayer->sendPacket(&gcMPRecoveryStart);

						GCUseOK _GCUseOK;
						pGamePlayer->sendPacket(&_GCUseOK);
					} 
					else 
					{
						EffectMPRecovery* pEffectMPRecovery = new EffectMPRecovery();

						pEffectMPRecovery->setTarget(pSlayer);
						pEffectMPRecovery->setDeadline(Deadline);
						pEffectMPRecovery->setDelay(MPDelayProvider);
						pEffectMPRecovery->setNextTime(0);
						pEffectMPRecovery->setMPQuantity(MPQuantity);
						pEffectMPRecovery->setPeriod(Period);

						pEffectManager->addEffect(pEffectMPRecovery);

						// ȸ�� �����϶�� ��Ŷ�� �ڽſ��� ������.
						GCMPRecoveryStart gcMPRecoveryStart;
						gcMPRecoveryStart.setPeriod(Period);
						gcMPRecoveryStart.setDelay(MPDelayProvider);
						gcMPRecoveryStart.setQuantity(MPQuantity);

						pGamePlayer->sendPacket(&gcMPRecoveryStart);

						GCUseOK _GCUseOK;
						pGamePlayer->sendPacket(&_GCUseOK);
					}
				} 
				else 
				{
					GCCannotUse _GCCannotUse;
					_GCCannotUse.setObjectID(pPacket->getObjectID());
					pGamePlayer->sendPacket(&_GCCannotUse);
					return;
				}
			} 
			else 
			{
				notRecoverMP = true;
			}

			if (notRecoverHP && notRecoverMP) 
			{
				GCCannotUse _GCCannotUse;
				_GCCannotUse.setObjectID(pPacket->getObjectID());
				pGamePlayer->sendPacket(&_GCCannotUse);
				return;
			}
			else
			{
				decreaseItemNum(pBeltItem, pBeltInventory, pSlayer->getName(), STORAGE_BELT, pBelt->getItemID(), SlotID, 0);
			}
		}
		else if ( pCreature->isOusters() )
		{
			Ousters* pOusters = dynamic_cast<Ousters*>(pCreature);
			Zone*   pZone   = pOusters->getZone();

			// SlotID�� �޴´�.
			SlotID_t SlotID = pPacket->getSlotID();

			Ousters::WearPart part = ( SlotID > 2 ? Ousters::WEAR_ARMSBAND2 : Ousters::WEAR_ARMSBAND1 );
			if ( SlotID > 2 ) SlotID -= 3;

			Item* pOustersArmsband = pOusters->getWearItem(part);
			
			if (pOustersArmsband == NULL)
			{
				GCCannotUse _GCCannotUse;
				_GCCannotUse.setObjectID(pPacket->getObjectID());
				pGamePlayer->sendPacket(&_GCCannotUse);
				return;
			}

			// ��Ʈ�� �κ��丮�� �޾ƿ´�.
			Inventory* pOustersArmsbandInventory = ((OustersArmsband*)pOustersArmsband)->getInventory();
			
			// ������ ������ �ʰ��ϸ� ����� �� ����.
			if (SlotID >= pOustersArmsbandInventory->getWidth())
			{
				GCCannotUse _GCCannotUse;
				_GCCannotUse.setObjectID(pPacket->getObjectID());
				pGamePlayer->sendPacket(&_GCCannotUse);
				return;
			}
			
			// ��Ʈ�� ���� ������ �������� �޾ƿ´�.
			Item* pOustersArmsbandItem = pOustersArmsbandInventory->getItem(SlotID, 0);
			
			// �������� ������ �翬�� ����� �� ����.
			if (pOustersArmsbandItem == NULL)
			{
				GCCannotUse _GCCannotUse;
				_GCCannotUse.setObjectID(pPacket->getObjectID());
				pGamePlayer->sendPacket(&_GCCannotUse);
				return;
			}
			
			// ������ �ִ� �������� Object�� �޴´�.
			ObjectID_t ItemObjectID = pOustersArmsbandItem->getObjectID();

			// �����Ͱ� ��ġ���� �ʰų�, Ǫ�ĳ� ���������̰� �ƴ϶�� ����� �� ����.
			if (ItemObjectID != pPacket->getObjectID()
				|| ( pOustersArmsbandItem->getItemClass() != Item::ITEM_CLASS_PUPA && pOustersArmsbandItem->getItemClass() != Item::ITEM_CLASS_COMPOS_MEI )
				)
			{
				GCCannotUse _GCCannotUse;
				_GCCannotUse.setObjectID(pPacket->getObjectID());
				pGamePlayer->sendPacket(&_GCCannotUse);
				return;
			}

			if ( pOustersArmsbandItem->getItemClass() == Item::ITEM_CLASS_PUPA )
			{
				HP_t  MaxHP     = pOusters->getHP(ATTR_MAX);
				HP_t  CurrentHP = pOusters->getHP(ATTR_CURRENT);
				Pupa* pPupa     = dynamic_cast<Pupa*>(pOustersArmsbandItem);

				// ���Ͽ� ȸ���Ǵ� ��
				int		HPQuantity = pPupa->getHPQuantity();

				// ������ ���� �ΰ�.
				int		HPDelayProvider = pPupa->getHPDelay();

				HP_t    PupaHPAmount = 0;

				PupaHPAmount = pPupa->getHPAmount();

				int     HPAmount     = min(MaxHP - CurrentHP , (int)PupaHPAmount);
				bool    notRecoverHP = false;


				// HP ȸ������ �����Ѵٸ�...
				if (HPAmount != 0 && HPQuantity != 0)
				{
					if (CurrentHP < MaxHP) 
					{
						EffectManager* pEffectManager = pOusters->getEffectManager();

						double temp     = (double)((double)HPAmount/(double)HPQuantity);
						uint   Period   = (uint)ceil(temp);
						Turn_t Deadline = Period* HPDelayProvider;

						if (pOusters->isFlag(Effect::EFFECT_CLASS_HP_RECOVERY)) 
						{
							Effect* pEffect = pEffectManager->findEffect(Effect::EFFECT_CLASS_HP_RECOVERY);
							EffectHPRecovery* pEffectHPRecoveryEffect = dynamic_cast<EffectHPRecovery*>(pEffect);

							// ������ ������� Ƚ���� ä��� HP���� ����Ѵ�.
							// �װ��� ���� ȸ���翡 ���Ѵ�.
							int PrevHPAmount = pEffectHPRecoveryEffect->getHPQuantity()* pEffectHPRecoveryEffect->getPeriod();
							HPAmount = min((int)(HPAmount + PrevHPAmount), MaxHP - CurrentHP);

							// �� �߿� ū ����ȸ����� ���� �����̸� ����.
							HPQuantity      = max(HPQuantity,      (int)(pEffectHPRecoveryEffect->getHPQuantity()));
							HPDelayProvider = min(HPDelayProvider, (int)(pEffectHPRecoveryEffect->getDelay()));

							// ���� ȸ������ ������, �󸶾� ����� ȸ���� ���ΰ��� �����Ѵ�.
							temp     = (double)((double)HPAmount/(double)HPQuantity);
							Period   = (uint)ceil(temp);
							Deadline = Period* HPDelayProvider;

							// HP Recovery effect�� �����Ѵ�.
							pEffectHPRecoveryEffect->setDeadline(Deadline);
							pEffectHPRecoveryEffect->setDelay(HPDelayProvider);
							pEffectHPRecoveryEffect->setHPQuantity(HPQuantity);
							pEffectHPRecoveryEffect->setPeriod(Period);

							// ȸ�� �����϶�� ��Ŷ�� �ڽſ��� ������.
							GCHPRecoveryStartToSelf gcHPRecoveryStartToSelf;
							gcHPRecoveryStartToSelf.setPeriod(pEffectHPRecoveryEffect->getPeriod());
							gcHPRecoveryStartToSelf.setDelay(pEffectHPRecoveryEffect->getDelay());
							gcHPRecoveryStartToSelf.setQuantity(pEffectHPRecoveryEffect->getHPQuantity());

							pGamePlayer->sendPacket(&gcHPRecoveryStartToSelf);

							// ȸ�� �����϶�� ��Ŷ�� �ٸ��̵鿡�� ������.
							// ȸ�� ���� ��Ŷ, ���۰� �� ���� ��Ŷ�� ������.
							GCHPRecoveryStartToOthers gcHPRecoveryStartToOthers;
							gcHPRecoveryStartToOthers.setObjectID(pOusters->getObjectID());
							gcHPRecoveryStartToOthers.setPeriod(pEffectHPRecoveryEffect->getPeriod());
							gcHPRecoveryStartToOthers.setDelay(pEffectHPRecoveryEffect->getDelay());
							gcHPRecoveryStartToOthers.setQuantity(pEffectHPRecoveryEffect->getHPQuantity());

							pZone->broadcastPacket(pOusters->getX(), pOusters->getY(), &gcHPRecoveryStartToOthers, pOusters);
							GCUseOK _GCUseOK;
							pGamePlayer->sendPacket(&_GCUseOK);
						} 
						else 
						{
							EffectHPRecovery* pEffectHPRecovery = new EffectHPRecovery();

							pEffectHPRecovery->setTarget(pOusters);
							pEffectHPRecovery->setDeadline(Deadline);
							pEffectHPRecovery->setDelay(HPDelayProvider);
							pEffectHPRecovery->setNextTime(0);
							pEffectHPRecovery->setHPQuantity(HPQuantity);
							pEffectHPRecovery->setPeriod(Period);

							pEffectManager->addEffect(pEffectHPRecovery);

							// ȸ�� �����϶�� ��Ŷ�� �ڽſ��� ������.
							GCHPRecoveryStartToSelf gcHPRecoveryStartToSelf;
							gcHPRecoveryStartToSelf.setPeriod(Period);
							gcHPRecoveryStartToSelf.setDelay(HPDelayProvider);
							gcHPRecoveryStartToSelf.setQuantity(HPQuantity);

							pGamePlayer->sendPacket(&gcHPRecoveryStartToSelf);

							// ȸ�� �����϶�� ��Ŷ�� �����̵鿡�� ������.
							GCHPRecoveryStartToOthers gcHPRecoveryStartToOthers;
							gcHPRecoveryStartToOthers.setObjectID(pOusters->getObjectID());
							gcHPRecoveryStartToOthers.setPeriod(Period);
							gcHPRecoveryStartToOthers.setDelay(HPDelayProvider);
							gcHPRecoveryStartToOthers.setQuantity(HPQuantity);

							pZone->broadcastPacket(pOusters->getX(), pOusters->getY(), &gcHPRecoveryStartToOthers, pOusters);
							GCUseOK _GCUseOK;
							pGamePlayer->sendPacket(&_GCUseOK);
						}
					} 
					else 
					{
						GCCannotUse _GCCannotUse;
						_GCCannotUse.setObjectID(pPacket->getObjectID());
						pGamePlayer->sendPacket(&_GCCannotUse);
						return;
					}
				} 
				else 
				{
					notRecoverHP = true;
				}

				if (notRecoverHP) 
				{
					GCCannotUse _GCCannotUse;
					_GCCannotUse.setObjectID(pPacket->getObjectID());
					pGamePlayer->sendPacket(&_GCCannotUse);
					return;
				}
				else
				{
					decreaseItemNum(pOustersArmsbandItem, pOustersArmsbandInventory, pOusters->getName(), STORAGE_BELT, pOustersArmsband->getItemID(), SlotID, 0);
				}
			}
			else if ( pOustersArmsbandItem->getItemClass() == Item::ITEM_CLASS_COMPOS_MEI )
			{
				MP_t    MaxMP        = pOusters->getMP(ATTR_MAX);
				MP_t    CurrentMP    = pOusters->getMP(ATTR_CURRENT);
				ComposMei* pComposMei      = dynamic_cast<ComposMei*>(pOustersArmsbandItem);

				// ���Ͽ� ȸ���Ǵ� ��
				int		MPQuantity = pComposMei->getMPQuantity();

				// ������ ���� �ΰ�.
				int		MPDelayProvider = pComposMei->getMPDelay();

				Attr_t INT = pOusters->getINT();

				MP_t    ComposMeiMPAmount = 0;

				ComposMeiMPAmount = pComposMei->getMPAmount();

				int     MPAmount     = min(MaxMP - CurrentMP , (int)(pComposMei->getMPAmount()* (1 + (double)((double)INT / 300.0))));
				bool    notRecoverMP = false;

				// MP ȸ������ �����Ѵٸ�...
				if (MPAmount != 0 && MPQuantity != 0 )
				{
					// �󸶾� ��� ���ʸ���.
					if (CurrentMP < MaxMP) 
					{
						EffectManager* pEffectManager = pOusters->getEffectManager();

						double temp     = (double)((double)MPAmount/(double)MPQuantity);
						uint   Period   = (uint)ceil(temp);
						Turn_t Deadline = Period* MPDelayProvider;

						if (pOusters->isFlag(Effect::EFFECT_CLASS_MP_RECOVERY)) 
						{
							Effect* pEffect = pEffectManager->findEffect(Effect::EFFECT_CLASS_MP_RECOVERY);
							EffectMPRecovery* pEffectMPRecoveryEffect = dynamic_cast<EffectMPRecovery*>(pEffect);

							// ������ ������� Ƚ���� ä��� MP���� ����Ѵ�.
							// �װ��� ���� ȸ���翡 ���Ѵ�.
							int PrevMPAmount = pEffectMPRecoveryEffect->getMPQuantity()* pEffectMPRecoveryEffect->getPeriod();
							MPAmount = min((int)(MPAmount + PrevMPAmount), MaxMP - CurrentMP);

							// �� �߿� ū ����ȸ����� ���� �����̸� ����.
							MPQuantity      = max(MPQuantity,      (int)(pEffectMPRecoveryEffect->getMPQuantity()));
							MPDelayProvider = min(MPDelayProvider, (int)(pEffectMPRecoveryEffect->getDelay()));

							// ���� ȸ������ ������, �󸶾� ����� ȸ���� ���ΰ��� �����Ѵ�.
							temp     = (double)((double)MPAmount/(double)MPQuantity);
							Period   = (uint)ceil(temp);
							Deadline = Period* MPDelayProvider;

							// MP Recovery effect�� �����Ѵ�.
							pEffectMPRecoveryEffect->setDeadline(Deadline);
							pEffectMPRecoveryEffect->setDelay(MPDelayProvider);
							pEffectMPRecoveryEffect->setMPQuantity(MPQuantity);
							pEffectMPRecoveryEffect->setPeriod(Period);

							// ȸ�� �����϶�� ��Ŷ�� �ڽſ��� ������.
							GCMPRecoveryStart gcMPRecoveryStart;
							gcMPRecoveryStart.setPeriod(pEffectMPRecoveryEffect->getPeriod());
							gcMPRecoveryStart.setDelay(pEffectMPRecoveryEffect->getDelay());
							gcMPRecoveryStart.setQuantity(pEffectMPRecoveryEffect->getMPQuantity());

							pGamePlayer->sendPacket(&gcMPRecoveryStart);

							GCUseOK _GCUseOK;
							pGamePlayer->sendPacket(&_GCUseOK);
						} 
						else 
						{
							EffectMPRecovery* pEffectMPRecovery = new EffectMPRecovery();

							pEffectMPRecovery->setTarget(pOusters);
							pEffectMPRecovery->setDeadline(Deadline);
							pEffectMPRecovery->setDelay(MPDelayProvider);
							pEffectMPRecovery->setNextTime(0);
							pEffectMPRecovery->setMPQuantity(MPQuantity);
							pEffectMPRecovery->setPeriod(Period);

							pEffectManager->addEffect(pEffectMPRecovery);

							// ȸ�� �����϶�� ��Ŷ�� �ڽſ��� ������.
							GCMPRecoveryStart gcMPRecoveryStart;
							gcMPRecoveryStart.setPeriod(Period);
							gcMPRecoveryStart.setDelay(MPDelayProvider);
							gcMPRecoveryStart.setQuantity(MPQuantity);

							pGamePlayer->sendPacket(&gcMPRecoveryStart);

							GCUseOK _GCUseOK;
							pGamePlayer->sendPacket(&_GCUseOK);
						}
					} 
					else 
					{
						GCCannotUse _GCCannotUse;
						_GCCannotUse.setObjectID(pPacket->getObjectID());
						pGamePlayer->sendPacket(&_GCCannotUse);
						return;
					}
				} 
				else 
				{
					notRecoverMP = true;
				}

				if (notRecoverMP) 
				{
					GCCannotUse _GCCannotUse;
					_GCCannotUse.setObjectID(pPacket->getObjectID());
					pGamePlayer->sendPacket(&_GCCannotUse);
					return;
				}
				else
				{
					decreaseItemNum(pOustersArmsbandItem, pOustersArmsbandInventory, pOusters->getName(), STORAGE_BELT, pOustersArmsband->getItemID(), SlotID, 0);
				}
			}
		}
	} 
	catch (Throwable & t) 
	{
			//cout << t.toString();
	}
	
#endif	// __GAME_SERVER__

    __END_DEBUG_EX __END_CATCH
}
