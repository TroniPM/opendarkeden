//////////////////////////////////////////////////////////////////////////////
// Filename    : EffectManager.cpp
// Written by  : Reiot
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "EffectManager.h"
#include "EffectInfo.h"
#include "EffectEnemyErase.h"
#include "Assert.h"
#include "Creature.h"
#include "Zone.h"
#include "ZoneUtil.h"
#include "Gpackets/GCAddEffect.h"

#include "Profile.h"

//#define __PROFILE_EM__

#ifdef __PROFILE_EM__
	#define __BEGIN_PROFILE_EM(name)	beginProfileEx(name);
	#define __END_PROFILE_EM(name)		endProfileEx(name);
#else
	#define __BEGIN_PROFILE_EM(name)	((void)0);
	#define __END_PROFILE_EM(name)		((void)0);
#endif



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
class isSameEffectClass 
{
public:
	isSameEffectClass(Effect::EffectClass EClass) : m_EClass(EClass) {}
	bool operator () (Effect* pEffect) throw ()
	{
		return pEffect->getEffectClass() == m_EClass;
	}
	
private:
	Effect::EffectClass m_EClass;
};


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
EffectManager::EffectManager () 
    throw (Error)
{
	__BEGIN_TRY

	Assert(m_Effects.empty());

	__END_CATCH
}
	
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
EffectManager::~EffectManager () 
    throw ()
{
	__BEGIN_TRY
		
	// �������� deleteEffects() �� ȣ����� �ʾҴٸ�, 
	// �� effect�� ���ؼ� unaffect()�� ȣ���� �ʿ䰡 ����.
	// ���� ��ü�� �������ָ� �ȴ�.
	while (!m_Effects.empty()) 
	{
		// effect ��ü�� �����Ѵ�.
		SAFE_DELETE(m_Effects.front());
		m_Effects.pop_front();
	}

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void EffectManager::save(const string & ownerID)
	throw(Error)
{
	__BEGIN_TRY

	list<Effect*>::const_iterator itr = m_Effects.begin();
	for(; itr != m_Effects.end(); itr++) 
		(*itr)->save(ownerID);

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool EffectManager::isEffect(Effect::EffectClass EClass)
	throw()
{
	__BEGIN_TRY

	list<Effect*>::const_iterator itr = find_if (m_Effects.begin(), m_Effects.end(), isSameEffectClass(EClass)); 
	if (itr != m_Effects.end()) return true;
	return false;

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void EffectManager::deleteEffect(ObjectID_t effectID)
	throw()
{
	__BEGIN_TRY
	__BEGIN_DEBUG

	list<Effect*>::iterator current = m_Effects.begin();

	int i = 0;
	for(current = m_Effects.begin(); current != m_Effects.end(); current++)
	{
		Effect* pEffect = *current;

		if (pEffect->getObjectID() == effectID)
		{
			m_Effects.erase(current);
			pEffect->unaffect();
			SAFE_DELETE(pEffect);
			return;
		}

		i++;
	}	

	//cerr << "EffectManager::deleteEffect���� effect�� ã�� ���߽��ϴ�."<<endl;
	Assert(false);

	__END_DEBUG
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void EffectManager::deleteEffect(Effect::EffectClass EClass)
	throw()
{
	__BEGIN_TRY
	__BEGIN_DEBUG

	list<Effect*>::iterator itr = find_if (m_Effects.begin(), m_Effects.end(), isSameEffectClass(EClass)); 
	if (itr != m_Effects.end()) 
	{
		Effect* pEffect = *itr;
		m_Effects.erase(itr);
		//pEffect->unaffect(); // �̰� �ϸ� �� �ȵɱ�?
		SAFE_DELETE(pEffect);
	}
	else
	{
		//cerr << "EffectManager::deleteEffect���� effect�� ã�� ���߽��ϴ�."<<endl;
	}

	__END_DEBUG
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void EffectManager::deleteEffect(Creature* pCreature, Effect::EffectClass EClass)
	throw()
{
	__BEGIN_TRY
	__BEGIN_DEBUG

	list<Effect*>::iterator itr = find_if (m_Effects.begin(), m_Effects.end(), isSameEffectClass(EClass)); 
	if (itr != m_Effects.end()) 
	{
		Effect* pEffect = *itr;

		m_Effects.erase(itr);

		if (pCreature)
		{
			if (pCreature->isFlag(EClass)) 
				pCreature->removeFlag(EClass);

			pEffect->destroy(pCreature->getName());
		}
	
		//pEffect->unaffect(); // �̰��ϸ� �ȵǳ�
		SAFE_DELETE(pEffect);
	}
	else
	{
		//cerr << "EffectManager::deleteEffect���� effect�� ã�� ���߽��ϴ�."<<endl;
	}

	__END_DEBUG
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Effect* EffectManager::findEffect(Effect::EffectClass EClass)
	throw()
{
	__BEGIN_TRY

	list<Effect*>::iterator itr = find_if (m_Effects.begin(), m_Effects.end(), isSameEffectClass(EClass)); 
	if (itr == m_Effects.end()) 
	{
		return NULL;
	}
	return *itr;
	
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Effect* EffectManager::findEffect(ObjectID_t ObjectID)
	throw()
{
	__BEGIN_TRY

	list<Effect*>::iterator itr = m_Effects.begin();
	for (; itr != m_Effects.end(); itr++)
	{
		Effect* pEffect = *itr;
		if (pEffect->getObjectID() == ObjectID) return pEffect;
	}

	return NULL;

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Effect* EffectManager::findEffect(Effect::EffectClass EClass, string EnemyName)
	throw()
{
	__BEGIN_TRY

	list<Effect*>::const_iterator itr = m_Effects.begin();

	for(; itr != m_Effects.end(); itr++) 
	{
		if ((*itr)->getEffectClass() == EClass && EClass == Effect::EFFECT_CLASS_ENEMY_ERASE) {
			if (((EffectEnemyErase *)(*itr))->getEnemyName() == EnemyName) return *itr;
		}
	}

	return NULL;
	
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
EffectInfo* EffectManager::getEffectInfo()
	throw()
{
	__BEGIN_TRY
	__BEGIN_DEBUG

	EffectInfo* pEffectInfo;

	__BEGIN_PROFILE_EM( "EM_GETEFFECTINFO" );
	
	__BEGIN_PROFILE_EM( "EM_NEW_EFFECTINFO" );
	pEffectInfo = new EffectInfo();
	__END_PROFILE_EM( "EM_NEW_EFFECTINFO" );

	BYTE ListNum = 0;

	Timeval currentTime;

	__BEGIN_PROFILE_EM( "EM_GETTIME" );
	getCurrentTime(currentTime);
	__END_PROFILE_EM( "EM_GETTIME" );

	// ����Ʈ ���� ����.
	for(list<Effect*>::const_iterator itr = m_Effects.begin(); itr != m_Effects.end(); itr++) 
	{
		Effect* pEffect = *itr;
		EffectID_t EffectID = pEffect->getSendEffectClass();

		/*
		bool bSend = true;
		
		switch (EffectID)
		{
			// �� ����Ʈ���� ���������� ���� ����Ʈ�̱� ������
			// Ŭ���̾�Ʈ���� �������� �ʴ´�.
			case Effect::EFFECT_CLASS_AFTERMATH:
			case Effect::EFFECT_CLASS_ALIGNMENT_RECOVERY:
			case Effect::EFFECT_CLASS_ENEMY_ERASE:
			case Effect::EFFECT_CLASS_RESTORE:
				bSend = false;
				break;
			default:
				break;
		}
		*/
		
		// �� Effect�� constructor���� client�� ���� effect���� �ƴ����� �����Ѵ�.
		// by sigi. 2002.11.14
		bool bSend = pEffect->isBroadcastingEffect();

		if (bSend)
		{
			Timeval DeadLine;
			Turn_t Duration;

			__BEGIN_PROFILE_EM( "EM_COMPUTE_TIME" )
			// ������ΰ� ���� �ð��� ���� ���� �ð��� ������� �Ѵ�.
			DeadLine = (*itr)->getDeadline();
			Duration = DeadLine.tv_sec - currentTime.tv_sec;
			__END_PROFILE_EM( "EM_COMPUTE_TIME" )

			if (EffectID == Effect::EFFECT_CLASS_BLOOD_DRAIN) 
			{
				// Duration ���ư��� ���� WORD ������ ���ư��µ�,
				// 2����Ʈ�� ���� �ð� ���� ��쿡�� �ʹ� ���
				// �����÷ο찡 �Ͼ�� �ȴ�.
				// �׷��� �ٸ� ����Ʈ ���� ��쿡�� 10�� ���ؼ� ��������, 
				// ���� ����Ʈ�� 10�� ���ؼ� ������ �ʴ´�.
				pEffectInfo->addListElement(EffectID, Duration);
			} 
			else 
			{
				pEffectInfo->addListElement(EffectID, Duration * 10);
			}

			ListNum++;
		}
	}

	pEffectInfo->setListNum(ListNum);

	__END_PROFILE_EM( "EM_GETEFFECTINFO" );

	return pEffectInfo;

	__END_DEBUG
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//
// sendEffectInfo
//
//////////////////////////////////////////////////////////////////////////////
// Effect ������ broadcasting�Ѵ�.
// by sigi. 2002.11.14
//////////////////////////////////////////////////////////////////////////////
void EffectManager::sendEffectInfo(Creature* pCreature, Zone* pZone, ZoneCoord_t x, ZoneCoord_t y)
	throw(Error)
{
	__BEGIN_TRY
	__BEGIN_DEBUG

	__BEGIN_PROFILE_EM( "SEND_EFFECT_INFO" );

	Assert(pCreature!=NULL);
	Assert(isValidZoneCoord(pZone, x, y));
	
	Timeval currentTime;

	getCurrentTime(currentTime);

	// ������ �ѷ��ش�.
	GCAddEffect gcAddEffect;
	gcAddEffect.setObjectID(pCreature->getObjectID());

	for(list<Effect*>::const_iterator itr = m_Effects.begin(); itr != m_Effects.end(); itr++) 
	{
		Effect* pEffect = *itr;

		if (pEffect->isBroadcastingEffect())
		{
			EffectID_t EffectID = pEffect->getSendEffectClass();
			gcAddEffect.setEffectID(EffectID);

			// ������ΰ� ���� �ð��� ���� ���� �ð��� ������� �Ѵ�.
			Timeval DeadLine = (*itr)->getDeadline();
			Turn_t Duration = DeadLine.tv_sec - currentTime.tv_sec;

			if (EffectID == Effect::EFFECT_CLASS_BLOOD_DRAIN) 
			{
				// Duration ���ư��� ���� WORD ������ ���ư��µ�,
				// 2����Ʈ�� ���� �ð� ���� ��쿡�� �ʹ� ���
				// �����÷ο찡 �Ͼ�� �ȴ�.
				// �׷��� �ٸ� ����Ʈ ���� ��쿡�� 10�� ���ؼ� ��������, 
				// ���� ����Ʈ�� 10�� ���ؼ� ������ �ʴ´�.
				gcAddEffect.setDuration(Duration);
			} 
			else 
			{
				gcAddEffect.setDuration(Duration*10);
			}

			pZone->broadcastPacket(x, y, &gcAddEffect);
		}
	}

	__END_PROFILE_EM( "SEND_EFFECT_INFO" );

	__END_DEBUG
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void EffectManager::addEffect (Effect* pEffect) 
     throw ()
{
	__BEGIN_TRY

	Assert(pEffect != NULL);
	m_Effects.push_back(pEffect);

	__END_CATCH
}

	
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*int EffectManager::heartbeat () 
     throw (Error)
{
	__BEGIN_TRY
	__END_DEBUG

	int rvalue = 0;

	Timeval currentTime;
	getCurrentTime(currentTime);

	list<Effect*>::iterator before  = m_Effects.end();
	list<Effect*>::iterator current = m_Effects.begin();

	while (current != m_Effects.end()) 
	{
		Assert(*current != NULL);
		Effect* pEffect = *current;
		
		// ����Ʈ�� expire �Ǿ����� �����Ѵ�.
		if (currentTime > pEffect->getDeadline()) 
		{
			if (before == m_Effects.end()) 
			{
				// first effect
				m_Effects.erase(current);
				current = m_Effects.begin();
			} 
			else 
			{
				// !first effect
				m_Effects.erase(current);
				current = before;
				current ++;
			}

			// *CAUTION
			// EffectBlood Drain�� ��쿡 ���� ���� ���� �����ش�.
			// ��? EffectBlood Drain�ȿ��� Slayer�� �����Ͱ� �����Ǳ� ������.
			// ���� Effect�� ó�� �ϸ鼭 ���� �� ���� �ִ�.
			// ������ �����̾ �Ǿ����� ������ ����Ʈ���� �ε��ϵ��� �Ѵ�.
			pEffect->unaffect();
			SAFE_DELETE(pEffect);

			rvalue++;
		} 
		else 
		{
			// expire ���� ���� ����Ʈ �߿�,
			// ���� affect �ð��� �Ǿ����� affect�Ѵ�.
			if (currentTime > pEffect->getNextTime()) 
			{
				pEffect->affect();
			}

			before = current ++;
		}

	}

	return rvalue;

	__BEGIN_DEBUG
	__END_CATCH
}
*/
//////////////////////////////////////////////////////////////////////////////
// ���� heartbeat�� �Ȱ�����
// �ð��� �޾ƿ��°Ÿ� �ٸ���. -_-;
// getCurrentTime() �� �ҷ���.. ������.. by sigi. 2002.5.8
//////////////////////////////////////////////////////////////////////////////
int EffectManager::heartbeat (const Timeval& currentTime)
     throw (Error)
{
	__BEGIN_TRY
	__END_DEBUG

	int rvalue = 0;

	__BEGIN_PROFILE_EM( "EM_HEARTBEAT" );

	list<Effect*>::iterator before  = m_Effects.end();
	list<Effect*>::iterator current = m_Effects.begin();

	while (current != m_Effects.end()) 
	{
		Assert(*current != NULL);
		Effect* pEffect = *current;

		// by sigi. for debugging. 2002.12.23
		m_LastEffectClass = (int)pEffect->getEffectClass();
		
		// ����Ʈ�� expire �Ǿ����� �����Ѵ�.
		if (currentTime > pEffect->getDeadline()) 
		{
			if (before == m_Effects.end()) 
			{
				// first effect
				m_Effects.erase(current);
				current = m_Effects.begin();
			} 
			else 
			{
				// !first effect
				m_Effects.erase(current);
				current = before;
				current ++;
			}

			// *CAUTION
			// EffectBlood Drain�� ��쿡 ���� ���� ���� �����ش�.
			// ��? EffectBlood Drain�ȿ��� Slayer�� �����Ͱ� �����Ǳ� ������.
			// ���� Effect�� ó�� �ϸ鼭 ���� �� ���� �ִ�.
			// ������ �����̾ �Ǿ����� ������ ����Ʈ���� �ε��ϵ��� �Ѵ�.
			// �̺�Ʈ ������� �ٲ㼭 ���� ���°� �� ���絵 �ǰ� �����ߴ�. -_-
			pEffect->unaffect();
			SAFE_DELETE(pEffect);

			rvalue++;
		} 
		else 
		{
			// expire ���� ���� ����Ʈ �߿�,
			// ���� affect �ð��� �Ǿ����� affect�Ѵ�.
			if (currentTime > pEffect->getNextTime()) 
			{
				pEffect->affect();
			}

			before = current ++;
		}

	}

	__END_PROFILE_EM( "EM_HEARTBEAT" );

	return rvalue;

	__BEGIN_DEBUG
	__END_CATCH
}