//File: Agent.h
//===================================================================
// Description: Class of agents for resolving conflicts
//===================================================================
// May, 2017             Created by                 Nikita Cherkasov
//===================================================================
#pragma once

#include <agents.h>
#include <ppl.h>
#include "Message.h"
#include "Vertex.h"
#include "Tools.h"

using namespace std;
using namespace concurrency;

/**
* @class Agent
* Агенты системы
*/
class Agent : public agent{
public:
	explicit Agent() : m_ver(NULL), m_bufferList ( NULL ), m_bufferForAncWeight ( NULL ), m_bufferDes ( NULL ), m_bufferAnc ( NULL ), m_masterBuffer ( NULL ) {}
	
	explicit Agent( Vertex * iver, unbounded_buffer<Message> * ibufferList, unbounded_buffer<Message> * ibufferDes, unbounded_buffer<Message> * ibufferAnc, single_assignment<Message> * ibufferForAncWeight, unbounded_buffer<Message> * imasterBuffer );

	Agent( const Agent & iother ) {
		m_ver = iother.m_ver;
		m_bufferList = iother.m_bufferList;
		m_bufferDes = iother.m_bufferDes;
		m_bufferAnc = iother.m_bufferAnc;
		m_masterBuffer = iother.m_masterBuffer;
		m_bufferForAncWeight = iother.m_bufferForAncWeight;
		m_power = iother.m_power;
		m_rmvd = iother.m_rmvd;
	}

	Agent & operator = ( const Agent & iother ) {
		m_ver = iother.m_ver;
		m_bufferList = iother.m_bufferList;
		m_bufferDes = iother.m_bufferDes;
		m_bufferAnc = iother.m_bufferAnc;
		m_masterBuffer = iother.m_masterBuffer;
		m_bufferForAncWeight = iother.m_bufferForAncWeight;
		m_power = iother.m_power;
		m_rmvd = iother.m_rmvd;
		return *this;
	}
	
	~Agent() {}
	/**
	* Вычисление веса
	*/
	void CompWeight();//Method of calculating the weight 
	/**
	* Вычисление веса предков по связи
	* @param icon - связь
	* @param iNumDesCon - количетсво предков по связи iCon
	*/
	void CompWeight_Anc_c( int icon, int iNumAncCon );//Method of calculating weight of anc to 1 con
	/**
	* Вычисление веса потомков по связи
	* @param icon - связь
	* @param iNumDesCon - количетсво потомков по связи iCon
	*/
	void CompWeight_Des_c( int icon, int iNumDesCon );//Method of calculating weight of des to 1 con
	/**
	* Вычисление веса предков
	*/
	void CompWeightAnc();
	/**
	* Вычисление веса потомков
	*/
	void CompWeightDes();
	/**
	* Отправка родственников своим предкам
	*/
	void SendRlt();
	/**
	* Запуск второго этапа поиска родственников
	*/
	void FindRltStart();
	/**
	* Поиск родственников
	*/
	void FindRlt( vector<int> iRlt );
	/**
	* Установить основной источник, откуда читать сообщения
	*/
	void SetSource( ISource<Message> &input );
	/**
	* Получить список родственников
	* @return m_rlt
	*/
	vector<int> & getRlt();
	/**
	* Получить список конфиктных родственников
	* @return m_confRlt
	*/
	vector<int> & getConfRlt();
	/**
	* Получить список конфликтных партнеров
	* @return m_coList
	*/
	vector<int> & getCoList();
	/**
	* Получить айди
	* @return m_id
	*/
	int & getId();
	/**
	* Получить вес предков
	* @return m_wAnc
	*/
	int & getwAnc();
	/**
	* Получить вес потоков
	* @return m_wDes
	*/
	int & getwDes();
	/**
	* Получить число связей
	* @return m_conNumber
	*/
	int & getConNumber();
	/**
	* Получить вес
	* @return m_wt
	*/
	int & getWt();
	/**
	* Получить массив с числами потомков по связям
	* @return m_nDes_c
	*/
	int * getNumDesCon();
	/**
	* Получить массив с числами предков по связям
	* @return m_nAnc_c
	*/
	int * getNumAncCon();
	/**
	* Получить массив с весами потомков по связям
	* @return m_wDes_c
	*/
	int * getWeightDesCon();
	/**
	* Получить массив с весами предков по связям
	* @return m_wAnc_c
	*/
	int * getWeightAncCon();
	/**
	* Получить указатель на данные
	* @return m_ver
	*/
	Vertex * getVertex();
	/**
	* Получить список ребер
	* @return m_edgeList
	*/
	vector<Vertex::Edge> & getEdgeList();
protected:
	void run();
private:
	Vertex * m_ver;//Pointer to vertex
	bool m_power, m_rmvd = 0;//Сapacity and status of remove
	unbounded_buffer<Message> * m_bufferList, * m_bufferDes, * m_bufferAnc, * m_masterBuffer;//Lists of sources agents and master source
	single_assignment<Message> * m_bufferForAncWeight;//Source for weight of ancestors
	ISource<Message> * source;//Main source
};