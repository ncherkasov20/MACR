//File: Master.h
//===================================================================
// Description: Agent-master
//===================================================================
// May, 2017             Created by                 Nikita Cherkasov
//===================================================================
#pragma once

/**
* @class Master
* Агент-мастер
*/
#include <agents.h>
#include <Vector>
#include <ppl.h>
#include "Tools.h"
#include "Message.h"
#include "Agent.h"
#include "ConfGroup.h"

using namespace concurrency;
using namespace std;

class Master : public agent {
public:
	explicit Master() : m_agentNumber ( 0 ), m_lexAgentNumber ( 0 ), m_bufferList( NULL ), m_agentList( NULL ) {}

	~Master() {}

	/**
	* Проверка 2 групп агентов на отношение конфликтности
	* @param ivect1 - 1 группа агентов, @param ivect1 - 2 группа агентов
	* @return возвращает массив cо статусами агентов ( 1 - у агента есть конфликтных партнер, 0 - агент "одинокий", т.е. нет партнера
	*/
	vector<int> IsConf( vector<int> ivect1, vector<int> ivect2 );
	/**
	* Создать списки конфликтных родственников у агентов
	*/
	void CreateConfRltList();
	/**
	* Определить конфликтные группы друзей агентов
	* @return ConfGropus
	*/
	vector<ConfGroup> FindConfGroups();
	/**
	* Составить список агентов на удаление
	* @param iConfGroups - конфликтные группы агентов
	* @return ToRemoveList
	*/
	vector<int> CreateToRemList(vector<ConfGroup> iConfGroups);
	/**
	* Удалить агентов из системы
	* @param iRemoveList - агенты, которых нужно удалить
	*/
	void RemoveAgents(vector<int> iRemoveList);
	/**
	* Получить список указателей на агентов
	*/
	vector<Agent*> & getAgentList();
	/**
	* Получить множество конфликтющих агентов
	*/
	vector<int> & getConf();
	/**
	* Установить общее число агентов
	*/
	void setAgentNumber(int ivalue);
	/**
	* Установить число лексических агентов
	*/
	void setLexAgentNumber(int ivalue);
	/**
	* Установить число конфликтующих агентов
	*/
	void setConflictNumber(int ivalue);
	/**
	* Установить указатель на множество агентов
	*/
	void setMAS(vector<Agent>* ivalue);
	/**
	* Установить указатель на массив блоков сообщений агентов(почтовых ящиков)
	*/
	void setBufferList(unbounded_buffer<Message> * ivalue);
	/**
	* Установить основной источник, откуда читать сообщения
	*/
	void SetSource(ISource<Message> &input);
protected:
	void run();
private:
	int m_agentNumber, m_lexAgentNumber, m_conflictNumber;//The number of lexical agents, agents and the number of conflict structures
	vector<int> m_conf;//Conflicting agents list
	unbounded_buffer<Message> * m_bufferList;//Lists of sources agent
	vector<Agent> * m_MAS;//Pointer to the system
	vector<Agent *> m_agentList;//List of pointers to agents
	ISource<Message> * source;//Source
};
