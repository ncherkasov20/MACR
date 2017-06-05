//File: MAS.h
//===================================================================
// Description: Multiagent System conflict resolution
//===================================================================
// May, 2017             Created by                 Nikita Cherkasov
//===================================================================
#pragma once

#include <agents.h>
#include <ppl.h>
#include <fstream>
#include "Agent.h"
#include "AgConfs.h"
#include "Message.h"
#include "Master.h"
#include "Vertex.h"
#include "Tools.h"

using namespace concurrency;
using namespace std;

/**
* @class MAS
* Мультиагентная система
*/
class MAS {
public:
	MAS(int iLexNumber, int iAgentNumber, int iConflictNumber);

	~MAS() {
		delete [] m_bufferList;
		delete [] m_bufferDes;
		delete [] m_bufferAnc;
		delete [] m_bufferForAncWeight;
	}

	/**
	* Инициализация данных
	* @param input - файл, из которого будет производиться чтение данных
	*/
	void Init( ifstream & input );
	/**
	* Запуск алгоритма
	* @param output - файл, в который будет производиться запись результатов
	*/
	void Start( ofstream & output );//Start algoritm
	/**
	* Выбрать айди для связи c преком
	* @param ipossibleAnc - список возможных предков
	* @param idPred - айди агента предка
	* @return id 
	*/
	int ChoiceOfIde( vector<Agent *> ipossibleAnc, int idPred);//Choice id for edge
	/**
	* Создать мультиагентную систему
	*/
	void CreateMASC();
	/**
	* Добавление нового агента в систему, используется при построение системы
	* @param ipossibleAnc - список возможных предшественников для агента
	* @param iIdAgent - id агента, которого нужно добавить в систему
	* @param iRltGroups - уже получившиеся списки родственников
	*/
	void AddNewAg( vector<Agent *> & ipossibleAnc, int iIdAgent, vector<vector<int>> & iRltGroups );
	/**
	* Проверка конфликтности групп агентов для построения системы
	* @param ivect1 - первый группа агентов
	* @param ivect2 - вторая группа агентов
	* @return true, если группы в конфлите, и false  в обратном случае
	*/
	bool ChekConflict( vector<int> ivect1, vector<int> ivect2 );
	/**
	* Проверка конфликтности агентов для построения системы
	* @param iFirstAg - первый агент
	* @param iSecondAg - второй агент
	* @return true, если агенты в конфлите, и false  в обратном случае
	*/
	bool ChekConflict( int iFirstAg, int iSecondAg );
	/**
	* Вывести систему агентов
	*/
	void PrintMAS(ofstream & output);
private:
	vector<Vertex> m_vertexList; //Vertex list
	vector<Agent> m_agentList; //List of agents
	Master m_master; //Master agent
	unbounded_buffer<Message> * m_bufferList, * m_bufferAnc, * m_bufferDes, m_masterBuffer;////Lists of sources agents and master source
	single_assignment<Message> * m_bufferForAncWeight;//Lists of sources for weight of ancestors
	int m_lexAgentNumber, m_agentNumber, m_conflictNumber; //The number of lexical agents, agents and the number of conflict structures
	AgConfsList m_agConfsList;//List of conflict structures
};