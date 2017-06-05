//File: Message.h
//===================================================================
// Description: Messages for communication agents
//===================================================================
// May, 2017             Created by                 Nikita Cherkasov
//===================================================================
#pragma once

#include<vector>

using namespace std;

enum a_act { compWeight = 1, compWeightAnc, compWeightDes, compWeightAncIsReady, ready, sendRlt, takeRlt, findRltStart, findRlt, activity, rem, remMe, stop};

/**
* @class Message
* Сообщения, которыми обмениваются агенты
*/
class Message {
public:
	a_act m_action;//Act of communication
	int m_inf, m_con, m_id, m_type;//Info of communication
	vector<int> m_rlt;
	Message() : m_action ( a_act(0) ), m_inf( 0 ), m_con ( 0 ), m_id ( 0 ) {}
	Message( a_act iaction ) : m_action ( iaction), m_inf ( 0 ), m_con ( 0 ), m_id ( 0 ) {}
	Message( a_act iaction , int iinf ) : m_action (iaction), m_inf ( iinf ), m_con ( 0 ), m_id ( 0 ) {}
	Message( a_act iaction, int iinf, int icon ) : m_action ( iaction ), m_inf ( iinf ), m_con ( icon) , m_id ( 0 ) {}
	Message( a_act iaction, int iinf, int icon, int iid ) : m_action ( iaction ), m_inf( iinf ), m_con( icon ), m_id( iid ) {}
	Message( a_act iaction, int iinf, int icon, int iid, int itype ) : m_action ( iaction) , m_inf ( iinf ), m_con ( icon ), m_id ( iid ), m_type ( itype ) {}
	Message(a_act iaction, vector<int> irlt) : m_action( iaction ), m_rlt( irlt ) {}
	~Message() {}
};