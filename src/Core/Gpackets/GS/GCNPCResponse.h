//////////////////////////////////////////////////////////////////////////////
// Filename    : GCNPCResponse.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_NPC_RESPONSE_H__
#define __GC_NPC_RESPONSE_H__

#include "Packet.h"
#include "PacketFactory.h"

enum 
{
	// Ŭ���̾�Ʈ�� �Ͽ��� ������ �������̽��� ������ �ϴ� �ڵ�
	NPC_RESPONSE_INTERFACE_REPAIR = 0,
	NPC_RESPONSE_INTERFACE_STASHOPEN,
	NPC_RESPONSE_INTERFACE_SILVER_COATING,
	NPC_RESPONSE_INTERFACE_CREATE_GUILD,
	NPC_RESPONSE_INTERFACE_DESTROY_GUILD,
	NPC_RESPONSE_INTERFACE_NEWBIE_ITEM,
	NPC_RESPONSE_INTERFACE_TUTORIAL_COMPUTER,
	NPC_RESPONSE_INTERFACE_TUTORIAL_BRIEFING,
	NPC_RESPONSE_INTERFACE_TUTORIAL_BOOKCASE,
	NPC_RESPONSE_INTERFACE_HELICOPTER,

	// ...
	NPC_RESPONSE_QUIT_DIALOGUE,
	NPC_RESPONSE_HEAL,

	// Ŭ���̾�Ʈ�� ��Ŷ�� ���� ���� �ڵ�
	NPC_RESPONSE_REPAIR_OK,
	NPC_RESPONSE_REPAIR_FAIL_ITEM_NOT_EXIST,
	NPC_RESPONSE_REPAIR_FAIL_ITEM_TYPE,
	NPC_RESPONSE_REPAIR_FAIL_MONEY,

	NPC_RESPONSE_STASH_SELL_OK,
	NPC_RESPONSE_STASH_SELL_FAIL_MAX,
	NPC_RESPONSE_STASH_SELL_FAIL_MONEY,

	NPC_RESPONSE_SILVER_COATING_OK,
	NPC_RESPONSE_SILVER_COATING_FAIL_ITEM_NOT_EXIST, 
	NPC_RESPONSE_SILVER_COATING_FAIL_ITEM_TYPE,
	NPC_RESPONSE_SILVER_COATING_FAIL_MONEY,

	NPC_RESPONSE_DONATION_OK,
	NPC_RESPONSE_DONATION_FAIL_MONEY,

	// �ӽÿ� ���̵�
	// ������ ��ġ���� �౸���� ���� ����� ���� ����� NPC Response�� ����ؼ� ���δ�.
	NPC_RESPONSE_DECREASE_BALL,

	// ��� ����
	NPC_RESPONSE_TEAM_REGIST_FAIL_ALREADY_JOIN,			// ��� �ѹ� ����? �ڳ״� �̹� <team_name> �� �Ҽ��̶�� ���� �ֱ�
	NPC_RESPONSE_TEAM_REGIST_FAIL_QUIT_TIMEOUT,			// �ڳ״� �ٸ� ���� Ż������ �� ���� �ʾұ�. ���� �� �����ϰ� �����ϰ� �ൿ�ϰ�
	NPC_RESPONSE_TEAM_REGIST_FAIL_CANCEL_TIMEOUT,		// �ڳ״� ���� ��� ���� �� �� ���� �ʾұ�. ��� ���ǿ� �����ϵ��� �ڽ��� �� �� �⸣�� ����. �� �� �����ϰ� ��ȸ�� �������� �ϰ�
	NPC_RESPONSE_TEAM_REGIST_FAIL_LEVEL,				// �Ǹ������� �����ν��� ������ �� ������ �� ����. �Ƿ��� �۰� ���Գ�
	NPC_RESPONSE_TEAM_REGIST_FAIL_MONEY,				// ���� ������� ���� ���� �ʿ��ϴٳ�. �ڳ״� ���� ���� ���̴±�...
	NPC_RESPONSE_TEAM_REGIST_FAIL_FAME,					// <player_name>�̶�.. �� ���� �׷� �̸��� ������ ���ߴٳ�. �װ� �ڳװ� ǲ������ ���� �ǹ�����. �Ƿ��� �۰� �ٽ� ã�ƿ��Գ�
	NPC_RESPONSE_TEAM_REGIST_FAIL_NAME,					// �� �̸��� �̹� ���̰� �ֱ�, �ٸ� �̸��� ������ ����
	NPC_RESPONSE_TEAM_REGIST_FAIL_DENY,					// �źεǾ����ϴ�.

	NPC_RESPONSE_TEAM_STARTING_FAIL_ALREADY_JOIN,		// ����� �̹� �ٸ� ���� ���ԵǾ� �ֽ��ϴ�.
	NPC_RESPONSE_TEAM_STARTING_FAIL_QUIT_TIMEOUT,		// �ڳ״� �ٸ� ���� Ż������ �� ���� �ʾұ�. ���� �� �����ϰ� �����ϰ� �ൿ�ϰ�
	NPC_RESPONSE_TEAM_STARTING_FAIL_CANCEL_TIMEOUT,		// �ڳ״� ���� ��� ���� �� �� ���� �ʾұ�. ���ǿ� �����ϵ��� �ڽ��� �� �� �⸣�� ����. �� �� �����ϰ� ��ȸ�� �������� �ϰ�
	NPC_RESPONSE_TEAM_STARTING_FAIL_LEVEL,				// �ڳ״� ���� ������ ���� ���� ���̴±���. ���� �� �Ƿ��� �۰� ã�Գ�.
	NPC_RESPONSE_TEAM_STARTING_FAIL_MONEY,				// <player_name>, �� ����� ���ؼ��� ���� �� �ʿ��ϴٳ�
	NPC_RESPONSE_TEAM_STARTING_FAIL_FAME,				// <player_name>�̶�.. �� ���� �׷� �̸��� ������ ���ߴٳ�. �װ� �ڳװ� ǲ������ ���� �ǹ�����. �Ƿ��� �۰� �ٽ� ã�ƿ��Գ�
	NPC_RESPONSE_TEAM_STARTING_FAIL_DENY,				// �źεǾ����ϴ�.

	NPC_RESPONSE_CLAN_REGIST_FAIL_ALREADY_JOIN,			// ��� �ѹ� ����? �ڳ״� �̹� <clan_name> Ŭ���� ������ ���� ���¶�� �����ֱ�
	NPC_RESPONSE_CLAN_REGIST_FAIL_QUIT_TIMEOUT,			// �ڳ״� �ٸ� Ŭ���� Ż������ �󸶵��� �ʾұ�. ���� �̸����� �Ű� �ٴϴ� ���� ���� ����. ������ ���ϰ�
	NPC_RESPONSE_CLAN_REGIST_FAIL_CANCEL_TIMEOUT,		// �ڳ״� Ŭ���� ��� ���� �� �� ���� �ʾұ�. �� �� �����ϰ� ��ȸ�� �������� �ϰ�
	NPC_RESPONSE_CLAN_REGIST_FAIL_LEVEL,				// �Ǹ������� ��θӸ��ν��� ������ �� ������ �� ����. �Ƿ��� �۰� ���Գ�.
	NPC_RESPONSE_CLAN_REGIST_FAIL_MONEY,				// Ŭ���� ����Ϸ��� ���� ���� �ʿ��ϴٳ�. �ڳ״� ���� ����̴±�...
	NPC_RESPONSE_CLAN_REGIST_FAIL_FAME,					// <player_name>�̶�.. ���� � �����̾��ΰ�����. �� ���� �Ǹ� ���ð� �ٽ� ã�ƿ��Գ�.
	NPC_RESPONSE_CLAN_REGIST_FAIL_NAME,					// 
	NPC_RESPONSE_CLAN_REGIST_FAIL_DENY,					// �źεǾ����ϴ�.

	NPC_RESPONSE_CLAN_STARTING_FAIL_ALREADY_JOIN,		// ����� �̹� �ٸ� Ŭ���� ���ԵǾ� �ֽ��ϴ�.
	NPC_RESPONSE_CLAN_STARTING_FAIL_QUIT_TIMEOUT,		// �ڳ״� �ٸ� Ŭ���� Ż������ �󸶵��� �ʾұ�. ���� �̸����� �Ű� �ٴϴ� ���� ���� ����. ������ ���ϰ�
	NPC_RESPONSE_CLAN_STARTING_FAIL_CANCEL_TIMEOUT,		// �ڳ״� Ŭ���� ��� ���� �� �� ���� �ʾұ�. �� �� �����ϰ� ��ȸ�� �������� �ϰ�
	NPC_RESPONSE_CLAN_STARTING_FAIL_LEVEL,				// �Ǹ��� �����ڰ� �Ǳ⿡�� ������ �� ������ �� ����. ���� ���� ������ �ϰ� ���Գ�
	NPC_RESPONSE_CLAN_STARTING_FAIL_MONEY,				// ����� �����ص� Ŭ���� ������ �� �ִ� ���� �����ϸ� �� Ŭ���� �������� ��������
	NPC_RESPONSE_CLAN_STARTING_FAIL_FAME,				// <player_name>�̶�.. �� ���� �׷� �̸��� ������ ���ߴٳ�. �װ� �ڳװ� ǲ������ ���� �ǹ�����. �Ƿ��� �۰� �ٽ� ã�ƿ��Գ�
	NPC_RESPONSE_CLAN_STARTING_FAIL_DENY,				//  �źεǾ����ϴ�.

	NPC_RESPONSE_GUILD_SHOW_REGIST,				// ��� ��� â ����
	NPC_RESPONSE_GUILD_SHOW_STARTING_JOIN,		// ��� ��Ÿ�� ��� ���� â ����
	NPC_RESPONSE_GUILD_SHOW_JOIN,				// ��� ���� â ����
	NPC_RESPONSE_GUILD_SHOW_QUIT,				// ��� Ż�� â ����
	NPC_RESPONSE_GUILD_ERROR,					// ��� ����

	NPC_RESPONSE_TRADE_GIFT_BOX_OK,				// ���� ���� ��ȯ ����
	NPC_RESPONSE_TRADE_GIFT_BOX_NO_ITEM,		// ���� ���� ����
	NPC_RESPONSE_TRADE_GIFT_BOX_ALREADY_TRADE,	// ���� ���ڸ� �̹� �ѹ� ��ȯ�ߴ�.
	NPC_RESPONSE_TRADE_GIFT_BOX_ERROR,			// ���� ���� ��ȯ�� ���� ��Ÿ ����

	NPC_RESPONSE_REWARD_OK,		            	// ������ �޾ҽ��ϴ�.
	NPC_RESPONSE_REWARD_FAIL,		            // ������ ���� �� �����ϴ�.
	NPC_RESPONSE_NO_EMPTY_SLOT,		            // �� �ڸ��� �����ϴ�.

	NPC_RESPONSE_SHOW_TAX_BALANCE,				// ���� �׿��ִ� ������ �ܾ��� �����ش�.
	NPC_RESPONSE_WITHDRAW_TAX_OK,				// ��� �����Ͱ� ������ ã�� ���� �����ߴ�.
	NPC_RESPONSE_WITHDRAW_TAX_FAIL,				// ��� �����Ͱ� ������ ã�� ���� �����ߴ�.

	NPC_RESPONSE_NO_GUILD,						// ��(Ŭ��)�� �ҼӵǾ� ���� �ʽ��ϴ�.
	NPC_RESPONSE_NOT_GUILD_MASTER,				// ��(Ŭ��) �����Ͱ� �ƴմϴ�.
	NPC_RESPONSE_HAS_NO_CASTLE,					// ��(Ŭ��)�� ���� ���� �����ϴ�.
	NPC_RESPONSE_NOT_YOUR_CASTLE,				// ��(Ŭ��)�� ������ ���� �ƴմϴ�.

	// �������
	NPC_RESPONSE_NOT_ENOUGH_MONEY,				// ���� �����մϴ�.(�����û�� ���������� ���������� ������ -_-;)
	NPC_RESPONSE_WAR_SCHEDULE_FULL,				// ���� �������� �� á���ϴ�.
	NPC_RESPONSE_WAR_ALREADY_REGISTERED,		// �̹� ������ ��û�߾����ϴ�.
	NPC_RESPONSE_WAR_REGISTRATION_OK,			// ���� �����쿡 ��� �Ǿ����ϴ�.
	NPC_RESPONSE_ALREADY_HAS_CASTLE,			// �̹� ���� ������ �ֽ��ϴ�.
	NPC_RESPONSE_WAR_UNAVAILABLE,				// ���� ��û�� �� �� �����ϴ�.

	// ���� ���� ����
	NPC_RESPONSE_RACE_WAR_JOIN_FAILED,			// <�����> �� �������� ���� ���� ��û�ο��� �� á���ϴ�.
	NPC_RESPONSE_RACE_WAR_JOIN_OK,				// ���� ���� ���� ��û�� �߽��ϴ�.
	NPC_RESPONSE_RACE_WAR_GO_FIRST_SERVER, 		// ���� ������ �� ������ ù��° ���������� ��û �� ������ �����մϴ�.

	// �̺�Ʈ ����������
	NPC_RESPONSE_GIVE_EVENT_ITEM_FAIL_NOW,		// ������ �̺�Ʈ �������� ���� �� �����ϴ�.
	NPC_RESPONSE_GIVE_EVENT_ITEM_FAIL,			// �̺�Ʈ �������� ���� �� �����ϴ�.
	NPC_RESPONSE_GIVE_EVENT_ITEM_OK,			// �̺�Ʈ�� ���� �������� �޾ҽ��ϴ�.
	NPC_RESPONSE_GIVE_PREMIUM_USER_ONLY,		// �����̾����� ����ڸ� ���� �� �ֽ��ϴ�.

	// Ŀ�ý�û ����
	NPC_RESPONSE_WAIT_FOR_MEET_COUPLE,			// Ŀ�� ��û �� ����� �̸��� �Է��ϼ���
	NPC_RESPONSE_COUPLE_MEET_SUCCESS,			// Ŀ���� ����Ǿ����ϴ�.
	NPC_RESPONSE_COUPLE_CANNOT_MEET,			// Ŀ���� ����� �� �����ϴ�.
	NPC_RESPONSE_MEET_WAIT_TIME_EXPIRED,		// �ð��� ������ ��û�� ��ҵǾ����ϴ�.

	NPC_RESPONSE_WAIT_FOR_APART_COUPLE,			// ����� ����� �̸��� �Է��ϼ���
	NPC_RESPONSE_COUPLE_APART_SUCCESS,			// ��������ϴ�.
	NPC_RESPONSE_NOT_COUPLE,					// Ŀ���� �ƴ϶� ����� �� �����ϴ�.
	NPC_RESPONSE_APART_WAIT_TIME_EXPIRED,		// �ð��� ������ ��û�� ��ҵǾ����ϴ�.

	NPC_RESPONSE_APART_COUPLE_FORCE,			// �Ϲ������� ����� ����� �̸��� �Է��ϼ���.

	NPC_RESPONSE_QUEST,							// ����Ʈ ���� �޽��� --;
	NPC_RESPONSE_LOTTERY,						// ���� �����ֱ�
	NPC_RESPONSE_CANNOT_BUY,					// ������ ������ �������� ã�� �� �����ϴ�.

	NPC_RESPONSE_CLEAR_RANK_BONUS_OK,			// �����Ͻ� ����� ��ų�� ���� �Ǿ����ϴ�.
	NPC_RESPONSE_NO_RANK_BONUS,					// �ش� �ڰ��� �����ϴ�.
	NPC_RESPONSE_ALREADY_CLEAR_RANK_BONUS,		// �����Ͻ� ��� ��ų�� ������ ���� �ֽ��ϴ�.

	NPC_RESPONSE_GNOME_CONTRACT_OK,				// ���� ���ɰ��� ����� �ξ����ϴ�.
	NPC_RESPONSE_DOWN_SKILL,					// �ٿ��ų ��ų�� �����ϼ���>.<

	// �ƿ콺���� ��� ����
	NPC_RESPONSE_GUILD_REGIST_FAIL_ALREADY_JOIN,		// ��� �ѹ� ����? �ڳ״� �̹� <guild_name> ��� �Ҽ��̶�� ���� �ֱ�
	NPC_RESPONSE_GUILD_REGIST_FAIL_QUIT_TIMEOUT,		// �ڳ״� �ٸ� ��带 Ż������ �� ���� �ʾұ�. ���� �� �����ϰ� �����ϰ� �ൿ�ϰ�
	NPC_RESPONSE_GUILD_REGIST_FAIL_CANCEL_TIMEOUT,		// �ڳ״� ��带 ��� ���� �� �� ���� �ʾұ�. ��� ���ǿ� �����ϵ��� �ڽ��� �� �� �⸣�� ����. �� �� �����ϰ� ��ȸ�� �������� �ϰ�
	NPC_RESPONSE_GUILD_REGIST_FAIL_LEVEL,				// �Ǹ������� �����ν��� ������ �� ������ �� ����. �Ƿ��� �۰� ���Գ�
	NPC_RESPONSE_GUILD_REGIST_FAIL_MONEY,				// ��带 ������� ���� ���� �ʿ��ϴٳ�. �ڳ״� ���� ���� ���̴±�...
	NPC_RESPONSE_GUILD_REGIST_FAIL_FAME,				// <player_name>�̶�.. �� ���� �׷� �̸��� ������ ���ߴٳ�. �װ� �ڳװ� ǲ������ ���� �ǹ�����. �Ƿ��� �۰� �ٽ� ã�ƿ��Գ�
	NPC_RESPONSE_GUILD_REGIST_FAIL_NAME,				// ��� �̸��� �̹� ���̰� �ֱ�, �ٸ� �̸��� ������ ����
	NPC_RESPONSE_GUILD_REGIST_FAIL_DENY,				// �źεǾ����ϴ�.

	NPC_RESPONSE_GUILD_STARTING_FAIL_ALREADY_JOIN,		// ����� �̹� �ٸ� ��忡 ���ԵǾ� �ֽ��ϴ�.
	NPC_RESPONSE_GUILD_STARTING_FAIL_QUIT_TIMEOUT,		// �ڳ״� �ٸ� ��带 Ż������ �� ���� �ʾұ�. ���� �� �����ϰ� �����ϰ� �ൿ�ϰ�
	NPC_RESPONSE_GUILD_STARTING_FAIL_CANCEL_TIMEOUT,	// �ڳ״� ��带 ��� ���� �� �� ���� �ʾұ�. ���ǿ� �����ϵ��� �ڽ��� �� �� �⸣�� ����. �� �� �����ϰ� ��ȸ�� �������� �ϰ�
	NPC_RESPONSE_GUILD_STARTING_FAIL_LEVEL,				// �ڳ״� ���� ������ ���� ���� ���̴±���. ���� �� �Ƿ��� �۰� ã�Գ�.
	NPC_RESPONSE_GUILD_STARTING_FAIL_MONEY,				// <player_name>, ��� ����� ���ؼ��� ���� �� �ʿ��ϴٳ�
	NPC_RESPONSE_GUILD_STARTING_FAIL_FAME,				// <player_name>�̶�.. �� ���� �׷� �̸��� ������ ���ߴٳ�. �װ� �ڳװ� ǲ������ ���� �ǹ�����. �Ƿ��� �۰� �ٽ� ã�ƿ��Գ�
	NPC_RESPONSE_GUILD_STARTING_FAIL_DENY,				// �źεǾ����ϴ�.

	NPC_RESPONSE_MAX
};

enum CoupleMessage
{
	COUPLE_MESSAGE_NOT_EVENT_TERM = 1,			// Ŀ�� �̺�Ʈ �Ⱓ�� �ƴմϴ�.
	COUPLE_MESSAGE_ALREADY_WAITING,				// �̹� ��븦 ��ٸ��� �ֽ��ϴ�.
	COUPLE_MESSAGE_LOGOFF,						// ������ �α��� ���� �ʾҽ��ϴ�.
	COUPLE_MESSAGE_DIFFERENT_RACE,				// ���� �ٸ� �����Դϴ�.
	COUPLE_MESSAGE_SAME_SEX,					// ���� ���̿��� Ŀ���� �����˴ϴ�.
	COUPLE_MESSAGE_NOT_PAY_PLAYER,				// ���� ����ڰ� �ƴմϴ�.
	COUPLE_MESSAGE_ALREADY_COUPLE,				// �̹� Ŀ���Դϴ�.
	COUPLE_MESSAGE_WAS_COUPLE,					// Ŀ���� ���� ���� �ֽ��ϴ�.
	COUPLE_MESSAGE_NOT_ENOUGH_GOLD,				// �������� �����մϴ�.
	COUPLE_MESSAGE_NOT_ENOUGH_ATTR,				// �ɷ�ġ�� �����մϴ�.
	COUPLE_MESSAGE_NOT_ENOUGH_LEVEL,			// ������ �����մϴ�.
	COUPLE_MESSAGE_INVENTORY_FULL,				// Ŀ�ø��� ���� ������ �����մϴ�.
	COUPLE_MESSAGE_NO_WAITING,					// ����� ��ٸ��� ��Ʈ�ʰ� �����ϴ�.
	COUPLE_MESSAGE_NOT_COUPLE,					// Ŀ���� �ƴմϴ�.

	COUPLE_MESSAGE_MAX
};

enum QuestMessage
{
	START_SUCCESS = 0,
	START_FAIL_QUEST_NUM_EXCEEDED,	// 1
	START_FAIL_DUPLICATED_QUEST_ID,	// 2
	START_FAIL_PC,					// 3
	COMPLETE_SUCCESS,				// 4
	COMPLETE_FAIL_NOT_COMPLETE,		// 5
	COMPLETE_FAIL_NO_INVENTORY_SPACE,// 6
	COMPLETE_FAIL_NOT_IN_QUEST,		// 7
	COMPLETE_FAIL_TIME_EXPIRED,		// 8
	START_FAIL_CANNOT_APPLY_QUEST,	// 9
	CANCEL_SUCCESS,					// 10
	CANCEL_NOT_IN_QUEST,			// 11
	COMPLETE_FAIL_INVALID_NPC,		// 12
	FAIL_BUG						// 13
};

//////////////////////////////////////////////////////////////////////////////
// class GCNPCResponse
//////////////////////////////////////////////////////////////////////////////

class GCNPCResponse : public Packet 
{

public:
	GCNPCResponse() throw() { m_Code = NPC_RESPONSE_MAX; m_Parameter = 0;}
	virtual ~GCNPCResponse() throw() {}

public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);

	PacketID_t getPacketID() const throw() { return PACKET_GC_NPC_RESPONSE; }
	PacketSize_t getPacketSize() const throw();
	string getPacketName() const throw() { return "GCNPCResponse"; }
	string toString() const throw();
	
public:
	BYTE getCode(void) const throw() { return m_Code;}
	void setCode(WORD code) throw() { m_Code = code;}

	uint getParameter(void) const throw() { return m_Parameter; }
	void setParameter(uint parameter) throw() { m_Parameter = parameter; }

private: 
	WORD m_Code;
	uint m_Parameter;

};


//////////////////////////////////////////////////////////////////////////////
// class GCNPCResponseFactory;
//////////////////////////////////////////////////////////////////////////////

class GCNPCResponseFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new GCNPCResponse(); }
	string getPacketName() const throw() { return "GCNPCResponse"; }
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_NPC_RESPONSE; }
	PacketSize_t getPacketMaxSize() const throw() { return szBYTE + szuint; }
};


//////////////////////////////////////////////////////////////////////////////
// class GCNPCResponseHandler
//////////////////////////////////////////////////////////////////////////////

class GCNPCResponseHandler 
{
public:
	static void execute( GCNPCResponse* pPacket, Player* pPlayer) throw(ProtocolException, Error);
};

#endif
