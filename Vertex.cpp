#include "stdafx.h"

#include "Vertex.h"

Vertex::Vertex(int iid, int iConNumber, int iLexNumber){
	m_id = iid;
	m_conNumber=iConNumber;
	m_wt = 0;
	m_wDes = 0;
	m_wAnc = 0;
    m_rlt.push_back(iid);
	m_wDes_c = new int[iLexNumber];
	m_wAnc_c = new int[iLexNumber];
	m_nDes_c = new int[iLexNumber];
	m_nAnc_c = new int[iLexNumber];
	for (int j = 0; j <iLexNumber; j++) {
		m_nDes_c[j] = 0;
		m_nAnc_c[j] = 0;
		m_wDes_c[j] = 0;
		m_wAnc_c[j] = 0;
	}
}

vector<Vertex::Edge> & Vertex::getEdgeList() { return m_edgeList; }

vector<int> & Vertex::getRlt() { return m_rlt; }

vector<int> & Vertex::getConfRlt() { return m_confRlt; }

vector<int> & Vertex::getCoList() { return m_coList; }

int & Vertex::getId() { return m_id; }

int & Vertex::getwAnc() { return m_wAnc; }

int & Vertex::getwDes() { return m_wDes; }

int & Vertex::getConNumber() { return m_conNumber; }

int & Vertex::getWt() { return m_wt; }

int * Vertex::getNumDesCon() { return m_nDes_c; }

int * Vertex::getNumAncCon() { return m_nAnc_c; }

int * Vertex::getWeightDesCon() { return m_wDes_c; }

int * Vertex::getWeightAncCon() { return m_wAnc_c; }