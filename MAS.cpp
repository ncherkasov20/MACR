#include "stdafx.h"

#include "MAS.h"

MAS::MAS( int iLexNumber, int iAgentNumber, int iConflictNumber ){
	m_conflictNumber = iConflictNumber;
	m_lexAgentNumber = iLexNumber;
	m_agentNumber = iAgentNumber;
	m_bufferList = new unbounded_buffer<Message>[iLexNumber + iAgentNumber];
	m_bufferAnc = new unbounded_buffer<Message>[iLexNumber + iAgentNumber];
	m_bufferDes = new unbounded_buffer<Message>[iLexNumber + iAgentNumber];
	m_bufferForAncWeight = new single_assignment<Message>[iLexNumber + iAgentNumber];
	m_vertexList.reserve(iLexNumber + iAgentNumber);
	m_agentList.reserve(iLexNumber + iAgentNumber);
	//Init AgentList
	for (int i = 0; i < m_lexAgentNumber + m_agentNumber; i++) {
		m_vertexList.push_back(Vertex(i, m_lexAgentNumber, m_lexAgentNumber));
		m_agentList.push_back(Agent(&m_vertexList[i], m_bufferList, m_bufferDes, m_bufferAnc, m_bufferForAncWeight, &m_masterBuffer));
		m_agentList[i].SetSource(m_bufferList[i]);
		m_master.getAgentList().push_back(&m_agentList[i]);
	}
	//Init Master agent	
	m_master.setMAS(&m_agentList);
	m_master.setBufferList(m_bufferList);
	m_master.setAgentNumber(m_lexAgentNumber + m_agentNumber);
	m_master.setLexAgentNumber(m_lexAgentNumber);
	m_master.setConflictNumber(m_conflictNumber);
	m_master.SetSource(m_masterBuffer);
}

void MAS::Init( ifstream & input ) {
	//Init ConfsList
	int confAg, confPartner, size;
	for (int i = 0; i < m_conflictNumber; i++) {
		input >> size >> confAg;
		m_master.getConf().push_back(confAg);
		vector<Agent*> coList;
		for (int j = 0; j < size; j++) {
			input >> confPartner;
			m_agentList[confAg].getCoList().push_back(confPartner);
			coList.push_back(&m_agentList[confPartner]);
		}
		m_agConfsList.push_back(AgConfs(&m_agentList[confAg], coList));
	}

	CreateMASC();
}

void MAS::Start( ofstream & output ) {
	PrintMAS(output);
	m_master.start();
	agent::wait(&m_master);
	ofstream output1("output1.txt");
	PrintMAS(output1);
	output1.close();
}

void MAS::CreateMASC() {
	vector<vector<int>> RltGroups;
	vector<Agent*> possibleAnc;
	for (int i = 0; i < m_lexAgentNumber; i++) {
		possibleAnc.push_back(&m_agentList[i]);
	}
	srand((unsigned)time(NULL));
	for (int idAgent = m_lexAgentNumber; idAgent < m_agentNumber + m_lexAgentNumber; idAgent++) {
		AddNewAg(possibleAnc, idAgent, RltGroups);
		possibleAnc.push_back(&m_agentList[idAgent]);
	}
}

void MAS::AddNewAg( vector<Agent*> & ipossibleAnc, int iIdAgent, vector<vector<int>> & iRltGroups ) {
	int numberOfPred, idFirstPred, idSecondPred, edgeType, firstEdgeId, secondEdgeId;
	numberOfPred = rand() % 2;
	if (numberOfPred == 0) {
		idFirstPred = rand() % size(ipossibleAnc);
		bool avail = 0;
		int numberOfGroup = iRltGroups.size();
		for (int i = 0; i < numberOfGroup; i++) {
			avail = Tools::CheckAvailability(idFirstPred, iRltGroups[i]);
			if (avail == true) {
				iRltGroups[i].push_back(iIdAgent);
				break; }
		}
		if (avail == false) {
			vector<int> Rlt;
			Rlt.push_back(idFirstPred);
			Rlt.push_back(iIdAgent);
			iRltGroups.push_back(Rlt);
		}
		firstEdgeId = ChoiceOfIde(ipossibleAnc, idFirstPred);
		edgeType = rand() % 2;
		ipossibleAnc[idFirstPred]->getEdgeList().push_back(Vertex::Edge(edgeType, m_agentList[iIdAgent].getVertex(), firstEdgeId, 1));
		ipossibleAnc[idFirstPred]->getNumDesCon()[firstEdgeId]++;
		m_agentList[iIdAgent].getEdgeList().push_back(Vertex::Edge(edgeType, ipossibleAnc[idFirstPred]->getVertex(), firstEdgeId, 0));
		m_agentList[iIdAgent].getNumAncCon()[firstEdgeId]++;
	}
	else
	{
		do {
			idFirstPred = rand() % size(ipossibleAnc);
			idSecondPred = rand() % size(ipossibleAnc);
		} while (idSecondPred == idFirstPred || ChekConflict(idFirstPred, idSecondPred) == true);
		bool firstAvail = false, secondAvail = false, conflictBetweenGroups = false;
		int firstGroupNumber, secondGroupNumber;
		int numberOfGroup = iRltGroups.size();
		for (int i = 0; i < numberOfGroup; i++) {
			firstAvail = Tools::CheckAvailability(idFirstPred, iRltGroups[i]);
			if (firstAvail == 1) {
				firstGroupNumber = i;
				break; }
		}
		for (int i = 0; i < numberOfGroup; i++) {
			secondAvail = Tools::CheckAvailability(idSecondPred, iRltGroups[i]);
			if (secondAvail == 1) {
				secondGroupNumber = i;
				break; }
		}
		if (firstAvail == false && secondAvail == false) {
			vector<int> Rlt;
			Rlt.push_back(idFirstPred);
			Rlt.push_back(idSecondPred);
			Rlt.push_back(iIdAgent);
			iRltGroups.push_back(Rlt);
		}
		if (firstAvail == false && secondAvail == true) {
			vector<int> v;
			v.push_back(idFirstPred);
			if (ChekConflict(v, iRltGroups[secondGroupNumber]) == false) {
				iRltGroups[secondGroupNumber].push_back(idFirstPred);
				iRltGroups[secondGroupNumber].push_back(iIdAgent);
			}
			else conflictBetweenGroups = true;
		}
		if (firstAvail == true && secondAvail == false) {
			vector<int> v;
			v.push_back(idSecondPred);
			if (ChekConflict(v, iRltGroups[firstGroupNumber]) == false) {
				iRltGroups[firstGroupNumber].push_back(idSecondPred);
				iRltGroups[firstGroupNumber].push_back(iIdAgent);
			}
			else conflictBetweenGroups = true;
		}
		if (firstAvail == true && secondAvail == true) {
			if (firstGroupNumber == secondGroupNumber) {
				iRltGroups[firstGroupNumber].push_back(iIdAgent);
			}
			else
				if (ChekConflict(iRltGroups[firstGroupNumber], iRltGroups[secondGroupNumber]) == false) {
					iRltGroups[firstGroupNumber] = Tools::UnionVectors(iRltGroups[firstGroupNumber], iRltGroups[secondGroupNumber]);
					iRltGroups[firstGroupNumber].push_back(iIdAgent);
					iRltGroups.erase(iRltGroups.begin() + secondGroupNumber);
				}
				else conflictBetweenGroups = true;
		}
		if (conflictBetweenGroups == true) {
			AddNewAg(ipossibleAnc, iIdAgent, iRltGroups);
		}
		else
		{
			firstEdgeId = ChoiceOfIde(ipossibleAnc, idFirstPred);
			secondEdgeId = ChoiceOfIde(ipossibleAnc, idSecondPred);
			edgeType = rand() % 2;
			ipossibleAnc[idFirstPred]->getEdgeList().push_back(Vertex::Edge(edgeType, m_agentList[iIdAgent].getVertex(), firstEdgeId, 1));
			ipossibleAnc[idFirstPred]->getNumDesCon()[firstEdgeId]++;
			m_agentList[iIdAgent].getEdgeList().push_back(Vertex::Edge(edgeType, ipossibleAnc[idFirstPred]->getVertex(), firstEdgeId, 0));
			m_agentList[iIdAgent].getNumAncCon()[firstEdgeId]++;
			edgeType = rand() % 2;
			ipossibleAnc[idSecondPred]->getEdgeList().push_back(Vertex::Edge(edgeType, m_agentList[iIdAgent].getVertex(), secondEdgeId, 1));
			ipossibleAnc[idSecondPred]->getNumDesCon()[secondEdgeId]++;
			m_agentList[iIdAgent].getEdgeList().push_back(Vertex::Edge(edgeType, ipossibleAnc[idSecondPred]->getVertex(), secondEdgeId, 0));
			m_agentList[iIdAgent].getNumAncCon()[secondEdgeId]++;
		}
	}
}

int MAS::ChoiceOfIde( vector<Agent*> ipossibleAnc, int idPred ) {
	if (idPred < m_lexAgentNumber) return ipossibleAnc[idPred]->getId();
	int Number, id;
	int l = size(ipossibleAnc[idPred]->getEdgeList());
	vector<int> possibleId;
	for (int i = 0; i < l; i++) {
		int edgeDir = ipossibleAnc[idPred]->getEdgeList()[i].getEdgeDir();
		if (edgeDir == 0) {
			int id = ipossibleAnc[idPred]->getEdgeList()[i].getEdgeId();
			if (Tools::CheckAvailability(id, possibleId) == false)
				possibleId.push_back(id);
		}
	}
	Number = rand() % size(possibleId);
	id = possibleId[Number];
	return id;
}

bool MAS::ChekConflict( vector<int> ivect1, vector<int> ivect2 ) {
	int sizeVect1 = size(ivect1);
	int sizeVect2 = size(ivect2);
	for (int i = 0; i < sizeVect1; i++)
		if (m_agentList[ivect1[i]].getCoList().empty() == false) {
			int sizeCoList = size(m_agentList[ivect1[i]].getCoList());
			for (int j = 0; j < sizeVect2; j++)
				for (int l = 0; l < sizeCoList; l++)
					if (ivect2[j] == m_agentList[ivect1[i]].getCoList()[l])
						return true;
		}
	return false;
}

bool MAS::ChekConflict( int iFirstAg, int iSecondAg ) {
	int sizeCoList = size(m_agentList[iFirstAg].getCoList());
	for (int i = 0; i < sizeCoList; i++)
		if (iSecondAg == m_agentList[iFirstAg].getCoList()[i])
			return true;
	return false;
}

void MAS::PrintMAS( ofstream & output ) {
	int sizeAgentList = size(m_agentList);
	for (int j = 0; j < sizeAgentList; j++) {
		output << m_agentList[j].getId() << " agent" << " - ";
		int sizeEdgeList = size(m_agentList[j].getEdgeList());
		for (int i = 0; i < sizeEdgeList; i++) {
			int edgeDir = m_agentList[j].getEdgeList()[i].getEdgeDir();
			int edgeType = m_agentList[j].getEdgeList()[i].getEdgeType();
			int edgeId = m_agentList[j].getEdgeList()[i].getEdgeId();
			int id = m_agentList[j].getEdgeList()[i].getEdgeVer()->getId();
			output << id << " ag: " << " Id con = " << edgeId << ", Type con = " << edgeType << ", dir = " << edgeDir << "|";
		}
		output << endl;
	}
}
