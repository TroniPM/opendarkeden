//////////////////////////////////////////////////////////////////////////////
// Filename    : GCWarScheduleList.cpp 
// Written By  :
// Description :
//////////////////////////////////////////////////////////////////////////////

#include "GCWarScheduleList.h"
#include "Assert.h"

//////////////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////////////
GCWarScheduleList::GCWarScheduleList() 
	throw ()
{
	__BEGIN_TRY 

	__END_CATCH;
}

//////////////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////////////
GCWarScheduleList::~GCWarScheduleList() 
	throw ()
{
	__BEGIN_TRY 

	WarScheduleInfoList::iterator itr = m_WarScheduleList.begin();

	for( ; itr != m_WarScheduleList.end() ; itr++ )
	{
		if( *itr != NULL ) SAFE_DELETE( *itr );
	}

	m_WarScheduleList.clear();
	
	__END_CATCH;
}

//////////////////////////////////////////////////////////////////////////////
// �Է½�Ʈ��(����)���κ��� ����Ÿ�� �о ��Ŷ�� �ʱ�ȭ�Ѵ�.
//////////////////////////////////////////////////////////////////////////////
void GCWarScheduleList::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

	BYTE count = 0;

	iStream.read( count );

	for( int i=0; i<count; i++ )
	{
		WarScheduleInfo* newWarScheduleInfo;
		newWarScheduleInfo = new WarScheduleInfo;

		iStream.read( newWarScheduleInfo->warType );
		iStream.read( newWarScheduleInfo->year );
		iStream.read( newWarScheduleInfo->month );
		iStream.read( newWarScheduleInfo->day );
		iStream.read( newWarScheduleInfo->hour );
		if ( newWarScheduleInfo->warType == 0 )
		{
			BYTE szGuildName; 
			iStream.read( newWarScheduleInfo->challengerGuildID );
			iStream.read( szGuildName );
			if ( szGuildName != 0 ) iStream.read( newWarScheduleInfo->challengerGuildName ,szGuildName );
		}

		addWarScheduleInfo( newWarScheduleInfo );
	}

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////////////
// ��½�Ʈ��(����)���� ��Ŷ�� ���̳ʸ� �̹����� ������.
//////////////////////////////////////////////////////////////////////////////
void GCWarScheduleList::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	BYTE ListNum = m_WarScheduleList.size();
		
	oStream.write( ListNum );

	WarScheduleInfoListItor itr = m_WarScheduleList.begin();

	for( ; itr != m_WarScheduleList.end(); itr++ )
	{
		oStream.write( (*itr)->warType );
		oStream.write( (*itr)->year );
		oStream.write( (*itr)->month );
		oStream.write( (*itr)->day );
		oStream.write( (*itr)->hour );
		if( (*itr)->warType == 0 )
		{
			oStream.write( (*itr)->challengerGuildID );
			BYTE szGuildName = (*itr)->challengerGuildName.size();
			oStream.write( szGuildName );
			oStream.write( (*itr)->challengerGuildName );
		}
	}

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////////////
void GCWarScheduleList::execute (Player * pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	GCWarScheduleListHandler::execute(this , pPlayer);

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

PacketSize_t GCWarScheduleList::getPacketSize () const 
	throw ()
{ 
	__BEGIN_TRY

	PacketSize_t size = szBYTE;

	WarScheduleInfoListItor itr = m_WarScheduleList.begin();
	
	for( ; itr != m_WarScheduleList.end(); itr++ )
	{
		size += szBYTE + szWORD + szBYTE + szBYTE + szBYTE;

		if( (*itr)->warType == 0 )
		{
			size += szGuildID;
			size += szBYTE;
			size += (*itr)->challengerGuildName.size();
		}
	}
		
	return size;

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////////////
// get packet's debug string
//////////////////////////////////////////////////////////////////////////////
string GCWarScheduleList::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	
	msg << "GCWarScheduleList(" 
		<< "WarNum : " << m_WarScheduleList.size();

	WarScheduleInfoListItor itr = m_WarScheduleList.begin();

	for( ; itr != m_WarScheduleList.end(); itr++ )
	{
		msg << ", (WarScheduleInfo : "
			<< ( ((*itr)->warType)?"WAR_TYPE_DIFFERENT_RACE, ":"WAR_TYPE_SAME_RACE, " )
			<< (*itr)->year << "�� "
			<< (*itr)->month << "�� "
			<< (*itr)->day << "��"
			<< (*itr)->hour << "��";

		if( (*itr)->warType == 0 )
		{
			msg << ", Challenger GuildID : " << (*itr)->challengerGuildID;
			msg << ", Challenger GuildName : " << (*itr)->challengerGuildName;
		}

		msg << ")";
	}

	return msg.toString();
		
	__END_CATCH
}

WarScheduleInfo* GCWarScheduleList::popWarScheduleInfo() 
	throw()
{
	__BEGIN_TRY

	if( m_WarScheduleList.empty() ) return NULL;

	WarScheduleInfo* ret = m_WarScheduleList.front();
	m_WarScheduleList.pop_front();

	return ret;

	__END_CATCH
}
