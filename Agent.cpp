#include "stdafx.h"

#include "Agent.h"

Agent::Agent(Vertex * iver, unbounded_buffer<Message> * ibufferList, unbounded_buffer<Message> * ibufferDes, unbounded_buffer<Message> * ibufferAnc, single_assignment<Message> * ibufferForAncWeight, unbounded_buffer<Message> * imasterBuffer) {
	m_ver = iver;
	m_bufferList = ibufferList;
	m_bufferDes = ibufferDes;
	m_bufferAnc = ibufferAnc;
	m_bufferForAncWeight = ibufferForAncWeight;
	m_masterBuffer = imasterBuffer;
}

void Agent::SetSource(ISource<Message> &input) { source = &input; }

vector<int> & Agent::getRlt() { return m_ver->getRlt(); }

vector<int> & Agent::getConfRlt() { return m_ver->getConfRlt(); }

vector<int> & Agent::getCoList() { return m_ver->getCoList(); }

int & Agent::getId() { return m_ver->getId(); }

int & Agent::getwAnc() { return m_ver->getwAnc(); }

int & Agent::getwDes() { return m_ver->getwDes(); }

int & Agent::getConNumber() { return m_ver->getConNumber(); }

int & Agent::getWt() { return m_ver->getWt(); }

int * Agent::getNumDesCon() { return m_ver->getNumDesCon(); }

int * Agent::getNumAncCon() { return m_ver->getNumAncCon(); }

int * Agent::getWeightDesCon() { return m_ver->getWeightDesCon(); }

int * Agent::getWeightAncCon() { return m_ver->getWeightAncCon(); }

Vertex * Agent::getVertex() { return m_ver; }

vector<Vertex::Edge> & Agent::getEdgeList() { return  m_ver->getEdgeList(); }

void Agent::SendRlt() {
	vector<int> genAncList, updAncList;
	int sizeList = size(getEdgeList());
	for ( int i = 0; i < sizeList; i++ ) {
		int edgeDir = getEdgeList()[i].getEdgeDir();
		int edgeType = getEdgeList()[i].getEdgeType();
		int edgeId = getEdgeList()[i].getEdgeId();
		if ( edgeDir == 0 && edgeType == 0 ) {
			if ( Tools::CheckAvailability(edgeId, updAncList) == false )
			updAncList.push_back(edgeId);
		}
		else
		if (edgeDir == 0 && edgeType == 1)
			if ( Tools::CheckAvailability(edgeId, genAncList) == false )
			genAncList.push_back(edgeId);
	}
	sizeList = size(genAncList);
	for ( int i = 0; i < sizeList; i++ ) {
			asend(m_bufferList[genAncList[i]], Message(takeRlt, Tools::UnionVectors(genAncList, updAncList)));
			asend(m_masterBuffer, Message(activity, 1));
	}
	sizeList = size(updAncList);
	for ( int i = 0; i < sizeList; i++ ) {
		asend(m_bufferList[updAncList[i]], Message(takeRlt, genAncList));
		asend(m_masterBuffer, Message(activity, 1));
	}
	asend(m_masterBuffer, Message(activity, -1));
}

void Agent::FindRltStart() {
	int j = size(getRlt());
	parallel_for(int(0), j, [&](int i) {
		if (getRlt()[i] != getId()) {
			asend(m_bufferList[getRlt()[i]], Message( findRlt, getRlt() ));
			asend(m_masterBuffer, Message( activity, 1 ));
		}
	});
	asend(m_masterBuffer, Message(activity, -1));
}

void Agent::FindRlt( vector<int> iRlt ) {
	int NumOld = size(getRlt());
	getRlt() = Tools::UnionVectors(getRlt(), iRlt);
	int NumNew = size(getRlt());
	if (NumOld < NumNew) {
		parallel_for(int(0), NumNew, [&](int i) {
			if (getRlt()[i] != getId()) {
				asend(m_bufferList[getRlt()[i]], Message(findRlt, getRlt()));
				asend(m_masterBuffer, Message(activity, 1));
			}
		});
	}
	asend(m_masterBuffer, Message(activity, -1));
}

void Agent::CompWeight() {
	parallel_invoke(
		[&] { CompWeightAnc(); },
		[&] { CompWeightDes(); }
	);
	getWt() = 1 + getwDes() + getwAnc();
	asend(m_masterBuffer, Message(ready));
};

void Agent::CompWeight_Anc_c( int icon, int iNumAncCon ) {
	Message response;
	while (iNumAncCon) {
		response = receive(m_bufferAnc[getId()]);
		if (response.m_action == compWeightAnc) {
			getWeightAncCon()[response.m_con] = getWeightAncCon()[response.m_con] + response.m_inf;
			iNumAncCon--;
		}
	}
	int j = size(getEdgeList());
	parallel_for(int(0), j, [&](int i) {
		int edgeDir = getEdgeList()[i].getEdgeDir();
		int edgeId = getEdgeList()[i].getEdgeId();
		int id = getEdgeList()[i].getEdgeVer()->getId();
		if (edgeId == icon && edgeDir == 1)
			asend(m_bufferAnc[id], Message(compWeightAnc, getWeightAncCon()[icon] + 1, icon));
	});
}

void Agent::CompWeight_Des_c(int icon, int iNumDesCon ) {
	Message response;
	while (iNumDesCon) {
		response = receive(m_bufferDes[getId()]);
		if (response.m_action == compWeightDes) {
			getWeightDesCon()[response.m_con] = getWeightDesCon()[response.m_con] + response.m_inf;
			iNumDesCon--;
		} 
	}
	int j = size(getEdgeList());
	parallel_for(int(0), j, [&](int i) {
		int edgeDir = getEdgeList()[i].getEdgeDir();
		int edgeType = getEdgeList()[i].getEdgeType();
		int edgeId = getEdgeList()[i].getEdgeId();
		int id = getEdgeList()[i].getEdgeVer()->getId();
		if (edgeId == icon && edgeDir == 0 && edgeType == 0)
			asend(m_bufferDes[id], Message(compWeightDes, getWeightDesCon()[icon] + 1, icon));
		if (edgeId == icon && edgeDir == 0 && edgeType == 1) {
			int weight;
				response = receive(m_bufferForAncWeight[getId()]);
				if (response.m_action == compWeightAncIsReady) {
					weight = response.m_inf;
				}
				for (int i = 0; i < getConNumber(); i++) {
					weight = weight + getWeightDesCon()[i];
				}
				asend(m_bufferDes[id], Message(compWeightDes, weight + 1, icon));
			}
		});
}

void Agent::CompWeightAnc() {
	int * NumAncCon = new int[getConNumber()];
	for (int i = 0; i < getConNumber(); i++) {
		NumAncCon[i] = getNumAncCon()[i];
	}
	//Comp Anc con weight
	parallel_for(int(0), getConNumber(), [&](int i) {
		CompWeight_Anc_c(i, NumAncCon[i]);
	});
	//Comp Anc weight
	for (int i = 0; i < getConNumber(); i++) {
		getwAnc() = getwAnc() + getWeightAncCon()[i];
	}
	asend(m_bufferForAncWeight[getId()], Message(compWeightAncIsReady, getwAnc()));
}

void Agent::CompWeightDes() {
	int * NumDesCon = new int[getConNumber()];
	for (int i = 0; i < getConNumber(); i++) {
		NumDesCon[i] = getNumDesCon()[i];
	}
	//Comp Anc con weight
	parallel_for(int(0), getConNumber(), [&](int i) {
		CompWeight_Des_c(i, NumDesCon[i]);
	});
	//Comp Anc weight
	for (int i = 0; i < getConNumber(); i++) {
		getwDes() = getwDes() + getWeightDesCon()[i];
	}
}

void Agent::run() {
	m_power = true;
	Message response;
	while (m_power) {
		response = receive(source);
		switch (response.m_action) {
		case compWeight: {
			CompWeight();
			break;
		}
		case sendRlt: {
			SendRlt();
			break;
		}
		case takeRlt: {
			getRlt() = Tools::UnionVectors(getRlt(), response.m_rlt);
			asend(m_masterBuffer, Message(activity, -1));
			break;
		}
		case findRltStart: {
			FindRltStart();
			break;
		}
		case findRlt: {
			FindRlt(response.m_rlt);
			break;
		}
		case rem: {
			if (m_rmvd) {
				asend(m_masterBuffer, Message(activity, -1));
				break;
			}
			if (!response.m_inf) {
				int j = size(getEdgeList());
				for (int i = 0; i < j; i++) {
					int edgeDir = getEdgeList()[i].getEdgeDir();
					int edgeId = getEdgeList()[i].getEdgeId();
					int id = getEdgeList()[i].getEdgeVer()->getId();
					if (edgeId == response.m_con && edgeDir == 1 && id == response.m_id) {
						getEdgeList().erase(getEdgeList().begin() + i);
						break;
					}
				}
				asend(m_masterBuffer, Message(activity, -1));
				break;
			}
			Vertex::edgeList RemEdge;
			if (response.m_type == 1 || response.m_inf == -1) {
				RemEdge = getEdgeList();
				m_rmvd = 1;
			}
			if (response.m_type == 0) {
				int sizeEdgeList = getEdgeList().size();
				for (int i = 0; i < sizeEdgeList; i++) {
					int edgeId = getEdgeList()[i].getEdgeId();
					if (edgeId == response.m_con) {
						RemEdge.push_back(m_ver->getEdgeList()[i]);
						getEdgeList().erase(getEdgeList().begin() + i);
						i--; sizeEdgeList--;
					}
				}
			}
			int sizeRemEdge = size(RemEdge);
			if (m_rmvd == 1) {
				parallel_for(int(0), sizeRemEdge, [&](int i) {
					int edgeDir = RemEdge[i].getEdgeDir();
					if (edgeDir == 0) {
						int edgeId = RemEdge[i].getEdgeId();
						int edgeType = RemEdge[i].getEdgeType();
						asend(m_bufferList[RemEdge[i].getEdgeVer()->getId()], Message(rem, edgeDir, edgeId, getId(), edgeType));
						asend(m_masterBuffer, Message(activity, 1));
					}
				});
			}
			parallel_for(int(0), sizeRemEdge, [&](int i) {
				int edgeDir = RemEdge[i].getEdgeDir();
				if (edgeDir == 1) {
					int edgeId = RemEdge[i].getEdgeId();
					int edgeType = RemEdge[i].getEdgeType();
					asend(m_bufferList[RemEdge[i].getEdgeVer()->getId()], Message(rem, edgeDir, edgeId, getId(), edgeType));
					asend(m_masterBuffer, Message(activity, 1));
				}
			});
			if (m_rmvd == 1) asend(m_masterBuffer, Message(remMe, getId()));
			else  asend(m_masterBuffer, Message(activity, -1));
			break;
		}
		case stop: {
			asend(m_masterBuffer, Message(ready));
			m_power = false;
			break;
		}
		}
	}					 
	done();
}

