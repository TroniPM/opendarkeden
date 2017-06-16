//////////////////////////////////////////////////////////////////////
// 
// Filename    : GTOAcknowledgement.cpp 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "GTOAcknowledgement.h"


//////////////////////////////////////////////////////////////////////
// Datagram ��ü�κ��� ����Ÿ�� �о ��Ŷ�� �ʱ�ȭ�Ѵ�.
//////////////////////////////////////////////////////////////////////
void GTOAcknowledgement::read ( Datagram & iDatagram ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	//--------------------------------------------------
	// read player id 
	//--------------------------------------------------
	iDatagram.read( m_ServerType );

	BYTE szMessage;
	iDatagram.read( szMessage );
	iDatagram.read( m_Message , szMessage );

	if ( m_Message != Message )
	{
		filelog("TheOneServer.log", "�߸��� �޽����� �����߽��ϴ�. [%s:%d] : %s", getHost().c_str(), getPort(), m_Message.c_str() );
	}

	BYTE szIP;
	iDatagram.read( szIP );
	iDatagram.read( m_ServerIP, szIP );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// Datagram ��ü�� ��Ŷ�� ���̳ʸ� �̹����� ������.
//////////////////////////////////////////////////////////////////////
void GTOAcknowledgement::write ( Datagram & oDatagram ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oDatagram.write( m_ServerType );
	
	BYTE sz;

	sz = m_Message.size();
	oDatagram.write( sz );
	oDatagram.write( m_Message );

	sz = m_ServerIP.size();
	oDatagram.write( sz );
	oDatagram.write( m_ServerIP );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void GTOAcknowledgement::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	GTOAcknowledgementHandler::execute( this );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
string GTOAcknowledgement::toString () const
    throw ()
{
	StringStream msg;

	msg << "GTOAcknowledgement("
		<< "ServerType : " << (int)m_ServerType
		<< " ,Message : " << m_Message
		<< " ,ServerIP : " << m_ServerIP
		<< ")";

	return msg.toString();
}
