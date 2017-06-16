//////////////////////////////////////////////////////////////////////
// 
// Filename    : VampireSkillInfo.h 
// Written By  : elca@ewestsoft.com
// Description :  ��ų�� ������
// 
//////////////////////////////////////////////////////////////////////

#ifndef __VAMPIRE_SKILL_INFO_H__
#define __VAMPIRE_SKILL_INFO_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "PCSkillInfo.h"
#include "SubVampireSkillInfo.h"
#include "Packet.h"

//////////////////////////////////////////////////////////////////////
//
// class VampireSkillInfo;
//
// ���Ӽ������� Ŭ���̾�Ʈ�� �ڽ��� ����� ������ �˷��ֱ� ���� Ŭ����
//
//////////////////////////////////////////////////////////////////////

class VampireSkillInfo : public PCSkillInfo {

public :
	
	// constructor
	VampireSkillInfo () throw ();
	
	// destructor
	~VampireSkillInfo () throw ();

public :

    // �Է½�Ʈ��(����)���κ��� ����Ÿ�� �о ��Ŷ�� �ʱ�ȭ�Ѵ�.
    void read (SocketInputStream & iStream) throw (ProtocolException, Error);
		    
    // ��½�Ʈ��(����)���� ��Ŷ�� ���̳ʸ� �̹����� ������.
    void write (SocketOutputStream & oStream) const throw (ProtocolException, Error);

	// get packet's body size
	// ����ȭ��, �̸� ���� ������ ����Ѵ�.
	PacketSize_t getSize () throw ();

	static uint getMaxSize () throw () { 
		return szBYTE + szBYTE + (SubVampireSkillInfo::getMaxSize()* 120);
	}

	// get packet's debug string
	string toString () const throw ();

	// get / set New Skill
	bool isLearnNewSkill() const throw() { return m_bLearnNewSkill; }
	void setLearnNewSkill(bool NewSkill) throw() { m_bLearnNewSkill = NewSkill; }

	// get / set ListNumber
	BYTE getListNum() const throw() { return m_ListNum; }
	void setListNum(BYTE ListNum) throw() { m_ListNum = ListNum; }

	// add / delete / clear S List
	void addListElement(SubVampireSkillInfo* pSubVampireSkillInfo) throw() { m_SubVampireSkillInfoList.push_back(pSubVampireSkillInfo); }

	// ClearList
	void clearList() throw() { m_SubVampireSkillInfoList.clear(); m_ListNum = 0; }

	// pop front Element in Status List
	SubVampireSkillInfo* popFrontListElement() throw() 
	{ 
		SubVampireSkillInfo* TempSubVampireSkillInfo = m_SubVampireSkillInfoList.front(); m_SubVampireSkillInfoList.pop_front(); return TempSubVampireSkillInfo; 
	}

private :

	// New ��ų�� ��� �� �ִ��� ������ ����
	bool m_bLearnNewSkill;

	// SubVampireSkillInfo List Total Number
	BYTE m_ListNum;

	// SubVampireSkillInfo List
	list<SubVampireSkillInfo*> m_SubVampireSkillInfoList;

};

#endif