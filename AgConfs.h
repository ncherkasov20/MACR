//File: AgConfs.h
//===================================================================
// Description: Class of conflict structures
//===================================================================
// May, 2017             Created by                 Nikita Cherkasov
//===================================================================
#pragma once

#include "Agent.h"

/**
* @class AgConfs
* Множество конфликтующих агентов
* m_ag конфликтующий агент, m_CoList конфликтный список агента 
*/
class AgConfs { 
public:
	Agent * m_ag; //Pointer to the conflicting agent
	vector<Agent*> m_CoList;  //List of pointers to conflicting partners
	AgConfs() : m_ag(NULL), m_CoList(NULL) {}
	AgConfs(Agent * iAgent, vector<Agent*> iCoList) : m_ag(iAgent), m_CoList(iCoList) {}
	~AgConfs() {}
}; typedef std::vector<AgConfs> AgConfsList;