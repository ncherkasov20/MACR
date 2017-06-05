//File: Vertex.h
//===================================================================
// Description: Class of objects with which the agent works
//===================================================================
// May, 2017             Created by                 Nikita Cherkasov
//===================================================================
#pragma once

#include <vector>

using namespace std;

/**
* @class Vertex
* Данные, с которыми работают агенты
*/
class Vertex {
public:
	/**
	* @class Edge
	* Cвязи между агентами
	*/
	class Edge { //Incident edge
	public:
		Edge() : m_edgeType(0), m_edgeVer(NULL), m_edgeId(0), m_edgeDir(0) {}

		Edge(int itype, Vertex * iver, int iid, int idir) : m_edgeType(itype), m_edgeVer(iver), m_edgeId(iid), m_edgeDir(idir) {}

		~Edge() {}

		/**
		* Получить направление ребра
		*/
		int & getEdgeDir();
		/**
		* Получить имя ребра
		*/
		int & getEdgeId();
		/**
		* Получить тип ребра
		*/
		int & getEdgeType();
		/**
		* Получить указатель на смежную вершину
		*/
		Vertex * getEdgeVer();
	private:
		Vertex * m_edgeVer; //Pointer to the adjacent vertex
		int m_edgeDir, m_edgeType, m_edgeId; //Direction, name, conflict type of the edge 
	}; typedef std::vector<Edge> edgeList;

	Vertex() :m_id ( 0 ), m_conNumber ( 0 ), m_wt ( 0 ), m_wDes ( 0 ), m_wAnc ( 0 ), m_nAnc_c ( NULL ), m_nDes_c ( NULL ), m_wAnc_c ( NULL ), m_wDes_c ( NULL ) {}
	
	Vertex(int iid, int iConNumber, int iLexNumber);
	
	~Vertex() {}

	/**
	* Получить список ребер
	* @return m_edgeList
	*/
	vector<Edge> & getEdgeList();
	/**
	* Получить список родственников
	* @return m_rlt
	*/
	vector<int> &  getRlt();
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
private:
	int * m_nDes_c, * m_nAnc_c, * m_wDes_c, * m_wAnc_c;//Weight and number of ancestors (descendants) by communication 
	int m_id, m_wt, m_conNumber, m_wDes, m_wAnc;//Id, weight, number of connections, ancestors and descendants weight of Vertex
	vector<int> m_rlt, m_confRlt, m_coList;//Lists of relatives, conflicting relatives and conflict list
	edgeList m_edgeList;//Edge list
};