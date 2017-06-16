//////////////////////////////////////////////////////////////////////
//
// Filename    : GameServerManager.h
// Written By  : Reiot
// Description :
//
//////////////////////////////////////////////////////////////////////

#ifndef __GAME_SERVER_MANANGER_H__
#define __GAME_SERVER_MANANGER_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Thread.h"
#include "DatagramSocket.h"

class Datagram;
class DatagramPacket;

//////////////////////////////////////////////////////////////////////
//
// class GameServerManager;
//
// ���� �������� ����� �����ϴ� �������̴�.
//
// ���ο� �����ͱ׷� ���������� �ϳ� ������ ����ŷ ������� �����Ѵ�.
//
//////////////////////////////////////////////////////////////////////

class GameServerManager : public Thread {

public :

	// constructor
	GameServerManager () throw ( Error );

	// destructor
	~GameServerManager () throw ( Error );

	//
	void init () throw ( Error ) {}

	// stop thread
	void stop () throw ( Error );

	// main method
	void run () throw ();

	void sendDatagram ( Datagram * pDatagram ) throw ( ConnectException , Error );
	void sendPacket ( string host , uint port , DatagramPacket * pPacket ) throw ( ConnectException , Error );

private :

	// UDP ���� ����
	DatagramSocket * m_pDatagramSocket;

};

// global variable declaration
extern GameServerManager * g_pGameServerManager;

#endif