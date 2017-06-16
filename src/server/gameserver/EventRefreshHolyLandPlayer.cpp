//////////////////////////////////////////////////////////////////////////////
// Filename    : EventRefreshHolyLandPlayer.cpp
// Written by  : bezz
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "EventRefreshHolyLandPlayer.h"
#include "HolyLandManager.h"
//#include "BloodBibleBonusManager.h"
#include "Zone.h"

//#include "Gpackets/GCHolyLandBonusInfo.h"

//////////////////////////////////////////////////////////////////////////////
// class EventRefreshHolyLandPlayer member methods
//////////////////////////////////////////////////////////////////////////////

EventRefreshHolyLandPlayer::EventRefreshHolyLandPlayer( GamePlayer* pGamePlayer )
	throw ()
	:Event(pGamePlayer)
{
}

void EventRefreshHolyLandPlayer::activate () 
	throw (Error)
{
	__BEGIN_TRY

	// �ƴ��� ������ �ִ� �÷��̾���� ������ ���� ����Ѵ�.(���� ���� ���ʽ�)
	g_pHolyLandManager->refreshHolyLandPlayers();

/*	// �ƴ��� ���� ������ ���� ���� ���ʽ� ������ �Ѹ���.
	GCHolyLandBonusInfo gcHolyLandBonusInfo;
	g_pBloodBibleBonusManager->makeHolyLandBonusInfo( gcHolyLandBonusInfo );
	g_pHolyLandManager->broadcast( &gcHolyLandBonusInfo );
*/
	__END_CATCH
}

string EventRefreshHolyLandPlayer::toString () const 
	throw ()
{
	StringStream msg;
	msg << "EventRefreshHolyLandPlayer("
		<< ")";
	return msg.toString();
}
