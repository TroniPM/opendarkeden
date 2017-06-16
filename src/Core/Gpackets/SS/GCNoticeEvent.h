//////////////////////////////////////////////////////////////////////////////
// Filename    : GCNoticeEvent.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_NOTICE_EVENT_H__
#define __GC_NOTICE_EVENT_H__

#include "Packet.h"
#include "PacketFactory.h"

#ifndef __GAME_CLIENT__
	#include "Utility.h"
#endif

enum 
{
	NOTICE_EVENT_KICK_OUT_FROM_ZONE,			// ������ �߹��ϴ� �ð�(��)
	NOTICE_EVENT_CONTINUAL_GROUND_ATTACK,		// ������ �������� ������ �Ͼ�� ���(��)
 	NOTICE_EVENT_CONTINUAL_GROUND_ATTACK_END,	// ������ �������� ������ ������.
	NOTICE_EVENT_MASTER_COMBAT_TIME,			// �����Ϳ� �ο� �� �ִ� ���� �ð�(��)
	NOTICE_EVENT_MASTER_COMBAT_END,				// �����Ϳ� �ο�°� ������.
	NOTICE_EVENT_METEOR_STRIKE,					// ���׿� ����(��)
	NOTICE_EVENT_METEOR_STRIKE_END,				// ���׿� ��
	NOTICE_EVENT_PREMIUM_HALF_START,			// �����̾� ����ڴ� ����/��û �ݰ�
	NOTICE_EVENT_PREMIUM_HALF_END,				// �̺�Ʈ ��
	NOTICE_EVENT_SHOP_TAX_CHANGE,				// ���� ���� �ٲ���.(����)

	// �������
	NOTICE_EVENT_WAR_OVER,						// ���ﳡ(CastleZoneID)
	NOTICE_EVENT_RACE_WAR_OVER,					// ���� ���ﳡ

	// ó�� ĳ���� �������� ��
	NOTICE_EVENT_WELCOME_MESSAGE,

	// ����Ʈ �ҸӴ� ������ -_-;
	NOTICE_EVENT_MEET_GRANDMA,
	NOTICE_EVENT_MEET_FISHSHOP_MASTER,

	NOTICE_EVENT_START_QUEST_ENDING,
	NOTICE_EVENT_RESULT_LOTTERY,

	NOTICE_EVENT_RUN_HORN,

	NOTICE_EVENT_MASTER_LAIR_OPEN,				// ������ ����(���丮����)�� ���Ƚ��ϴ�.
	NOTICE_EVENT_MASTER_LAIR_CLOSED,			// ������ ����(���丮����)�� �������ϴ�.
	NOTICE_EVENT_MASTER_LAIR_COUNT,				// ������ ����(���丮����) ���� ���� �ð��� 5�� ���ҽ��ϴ�.

	NOTICE_EVENT_CONTRACT_GNOMES_HORN,			// �ÿ������� ���� ����� �����ž� ����Ͻ� �� �ֽ��ϴ�.
	NOTICE_EVENT_NOT_ENOUGH_MONEY,				// ���� ���ڶ��ϴ�.

	NOTICE_EVENT_MINI_GAME,					// ����ã��~

	NOTICE_EVENT_GET_RIFINIUM,					// ���ǴϿ��� ȹ���Ͽ����ϴ�.
	NOTICE_EVENT_INVENTORY_FULL,				// �κ��丮 ���� ���� ������ �����մϴ�.

	NOTICE_EVENT_FLAG_WAR_READY,				// Capture the Flag! �̺�Ʈ�� �ǽ��մϴ�.
	NOTICE_EVENT_FLAG_WAR_START,				// �̺�Ʈ�� �����մϴ�.
	NOTICE_EVENT_FLAG_WAR_FINISH,				// �̺�Ʈ�� �������ϴ�. 3�� �� �������� �����ϴ�.
	NOTICE_EVENT_FLAG_POURED_ITEM,				// Capture the Flag �̺�Ʈ �������� ���Խ��ϴ�.

	NOTICE_EVENT_ENTER_BEGINNER_ZONE,				// �ʺ��������� ���ðڽ��ϱ�?
	NOTICE_EVENT_LOGIN_JUST_NOW,				// ��� �α����߽��ϴ�.

	NOTICE_EVENT_LEVEL_WAR_ARRANGED,			// �� ������ ������ ���۵˴ϴ�.
	NOTICE_EVENT_LEVEL_WAR_STARTED,				// ������ ������ ���۵Ǿ����ϴ�.

	NOTICE_EVENT_RACE_WAR_SOON,					// 3�Ͼȿ� �������� �մϴ�.

	NOTICE_EVENT_LEVEL_WAR_OVER,				// ������ ������ ������.
	NOTICE_EVENT_NETMARBLE_CARD_FULL,           // 99���� �ݸ��� ī�带 �� ��Ҵ�.

	NOTICE_EVENT_HOLYDAY,						// ������ �������̴�.
	NOTICE_EVENT_CAN_PET_QUEST,					// �� ����Ʈ�� �� �� �ִ�.

	NOTICE_EVENT_MAX
};

//////////////////////////////////////////////////////////////////////////////
// class GCNoticeEvent
//////////////////////////////////////////////////////////////////////////////

class GCNoticeEvent : public Packet 
{

public:
	GCNoticeEvent() throw() { m_Code = NOTICE_EVENT_MAX; m_Parameter = 0;}
	virtual ~GCNoticeEvent() throw() {}

public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);

	PacketID_t getPacketID() const throw() { return PACKET_GC_NOTICE_EVENT; }
	PacketSize_t getPacketSize() const throw();
	string getPacketName() const throw() { return "GCNoticeEvent"; }
	string toString() const throw();
	
public:
	BYTE getCode(void) const throw() { return m_Code;}
	void setCode(WORD code) throw() { m_Code = code;}

	uint getParameter(void) const throw() { return m_Parameter; }
	void setParameter(uint parameter) throw() { m_Parameter = parameter; }

#ifndef __GAME_CLIENT__
	void setParameter(WORD hiWord, WORD loWord) throw() { m_Code = makeDWORD(hiWord, loWord); }
#endif

private: 
	WORD m_Code;
	uint m_Parameter;

};


//////////////////////////////////////////////////////////////////////////////
// class GCNoticeEventFactory;
//////////////////////////////////////////////////////////////////////////////

class GCNoticeEventFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new GCNoticeEvent(); }
	string getPacketName() const throw() { return "GCNoticeEvent"; }
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_NOTICE_EVENT; }
	PacketSize_t getPacketMaxSize() const throw() { return szWORD + szuint; }
};


//////////////////////////////////////////////////////////////////////////////
// class GCNoticeEventHandler
//////////////////////////////////////////////////////////////////////////////

class GCNoticeEventHandler 
{
public:
	static void execute( GCNoticeEvent* pPacket, Player* pPlayer) throw(ProtocolException, Error);
};

#endif
