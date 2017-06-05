#include "stdafx.h"

#include "Master.h"	

vector<Agent*> & Master::getAgentList() { return m_agentList; }

vector<int> & Master::getConf() { return m_conf; }

void Master::setAgentNumber( int ivalue ) { m_agentNumber = ivalue; }

void Master::setLexAgentNumber( int ivalue ) { m_lexAgentNumber = ivalue; }

void Master::setConflictNumber( int ivalue ) { m_conflictNumber = ivalue; }

void Master::setMAS(vector<Agent>* ivalue) { m_MAS = ivalue; }

void Master::setBufferList( unbounded_buffer<Message> * ivalue ) { m_bufferList = ivalue; }

void Master::SetSource( ISource<Message> &input ) { source = &input; }

vector<int> Master::IsConf( vector<int> ivect1, vector<int> ivect2 ) {
	int sizeVect1 = size(ivect1);
	int sizeVect2 = size(ivect2);
	vector<int> AgentIsConfList(sizeVect1,0);
	for (int i = 0; i < sizeVect1; i++) {
		int sum = 0;
		int sizeCoList = m_agentList[ivect1[i]]->getCoList().size();
		for (int l = 0; l < sizeCoList; l++) {
			for (int j = 0; j < sizeVect2; j++)
				if (ivect2[j] == m_agentList[ivect1[i]]->getCoList()[l]) {
					sum++;
					break;
				}
		}
		if (sum == sizeCoList) AgentIsConfList[i] = 1;
	}
	return AgentIsConfList;
}

void Master::CreateConfRltList() {
	int size = m_conf.size();
	for (int i = 0; i < size; i++) {
		int sizeRLt = m_agentList[m_conf[i]]->getRlt().size();
		for (int j = 0; j < sizeRLt; j++)
			for (int l = 0; l < size; l++)
				if (m_agentList[m_conf[i]]->getRlt()[j] == m_conf[l]){
					m_agentList[m_conf[i]]->getConfRlt().push_back(m_agentList[m_conf[i]]->getRlt()[j]);
					break;
				}
	}
}

vector<ConfGroup> Master::FindConfGroups() {
	vector<ConfGroup> confGroups;
	vector<int> m_G1, m_A2, m_G2;
	m_G1 = m_agentList[m_conf[0]]->getConfRlt();
	m_A2 = m_agentList[m_conf[0]]->getCoList();
	int sizeA2 = m_A2.size();
	for (int i = 0; i < sizeA2; i++)
		m_G2 = Tools::UnionVectors(m_G2, m_agentList[m_A2[i]]->getConfRlt());
	while (!m_conf.empty()) {
		vector<int> G1AllConf(m_G1.size(), 1), G2AllConf(m_G2.size(), 1);
		vector<int> G1IsConfList(m_G1.size()), G2IsConfList(m_G2.size());
		G1IsConfList = IsConf(m_G1, m_G2);
		G2IsConfList = IsConf(m_G2, m_G1);
		if (G1IsConfList == G1AllConf && G2IsConfList == G2AllConf) {
			m_conf = Tools::DifferenceOfVectors(m_conf, Tools::UnionVectors(m_G1, m_G2));
			confGroups.push_back(ConfGroup(m_G1, m_G2));
			if (!m_conf.empty()) {
				m_G1 = m_agentList[m_conf[0]]->getConfRlt();
				m_A2 = m_agentList[m_conf[0]]->getCoList();
				sizeA2 = m_A2.size();
				vector<int> G2;
				for (int i = 0; i < sizeA2; i++)
					m_G2 = Tools::UnionVectors(G2, m_agentList[m_A2[i]]->getConfRlt());

			}
		}
		else{
			int size;
			if (G1IsConfList != G1AllConf) {
				size = m_G1.size();
				for (int i = 0; i < size; i++)
					if (G1IsConfList[i] == 0){
						m_A2 = m_agentList[m_G1[i]]->getCoList();
						sizeA2 = m_A2.size();
						for (int j = 0; j < sizeA2; j++)
							m_G2 = Tools::UnionVectors(m_G2, m_agentList[m_A2[j]]->getConfRlt());
					}
			}
			if (G2IsConfList != G2AllConf) {
				size = m_G2.size();
				for (int i = 0; i <size; i++)
					if (G2IsConfList[i] == 0){
						m_A2 = m_agentList[m_G2[i]]->getCoList();
						sizeA2 = m_A2.size();
						for (int j = 0; j < sizeA2; j++)
							m_G1 = Tools::UnionVectors(m_G1, m_agentList[m_A2[j]]->getConfRlt());
					}
			}
		}
	}
	return confGroups;
}

vector<int> Master::CreateToRemList(vector<ConfGroup> iConfGroups) {
	vector<int> ToRemList;
	while (!iConfGroups.empty()) {
		vector<int> F1, F2;
		F1 = iConfGroups[0].getG1();
		F2 = iConfGroups[0].getG2();
		int sumF1 = 0, sumF2 = 0;
		int sizeF1 = F1.size();
		int sizeF2 = F2.size();
		for (int i = 0; i < sizeF1; i++)
			sumF1 = sumF1 + m_agentList[F1[i]]->getWt();
		for (int i = 0; i < sizeF2; i++)
			sumF2 = sumF2 + m_agentList[F2[i]]->getWt();
		if (sumF1 < sumF2) ToRemList = Tools::UnionVectors(ToRemList, F1);
		if (sumF2 < sumF1) ToRemList = Tools::UnionVectors(ToRemList, F2);
		iConfGroups.erase(iConfGroups.begin());
	}
	return ToRemList;
}

void Master::RemoveAgents(vector<int> iRemoveList) {
	int sizeAgentList = m_agentNumber;
	int sizeRemList = iRemoveList.size();
	for (int l = 0; l < sizeRemList; l++)
		for (int i = 0; i < sizeAgentList; i++)
			if (m_agentList[i]->getId() == iRemoveList[l]) {
				m_MAS->erase(m_MAS->begin() + i);
				sizeAgentList--;
				break;
			}
}

void Master::run() {
	Message response;
	int activityValue;

	//Start agents
	parallel_for(int(0), m_agentNumber, [&]( int i ) {
		m_agentList[i]->start();
	});
	
	//Start calculating weight of agents
	parallel_for(int(0), m_agentNumber, [&]( int i ) {
		asend(m_bufferList[i], Message(compWeight));
	});

	activityValue = m_agentNumber;
	while (activityValue) {
		response = receive(source);
		if (response.m_action == ready)
			activityValue--;
	}

	//Start computing agents relatives step 1
	parallel_for(int(m_lexAgentNumber), m_agentNumber, [&](int i) {
		asend(m_bufferList[i], Message(sendRlt));
	}); 

	activityValue = m_agentNumber - m_lexAgentNumber;
	while (activityValue) {
		response = receive(source);
		if (response.m_action == activity) {
			activityValue = activityValue + response.m_inf;
		}
	}	

	//Start computing agents relatives step 2
	parallel_for(int(0), m_lexAgentNumber, [&](int i) {
		asend(m_bufferList[i], Message(findRltStart));
	});

	activityValue = m_lexAgentNumber;
	while (activityValue) {
		response = receive(source);
		if (response.m_action == activity) {
			activityValue = activityValue + response.m_inf;
		}
	}

	CreateConfRltList();

	vector<ConfGroup> confGroups; 
	confGroups = FindConfGroups();
	//Create Remove list
	vector<int> toRemList;
	toRemList = CreateToRemList(confGroups);

	vector<int> removeList;
	int sizeRemList = toRemList.size();
	parallel_for(int(0), sizeRemList, [&](int i) {
		asend(m_bufferList[toRemList[i]], Message(rem,-1));
	});

	activityValue = sizeRemList;
	while (activityValue) {
		response = receive(source);
		if (response.m_action == activity) 
			activityValue = activityValue + response.m_inf;
		if (response.m_action == remMe) {
			removeList.push_back(response.m_inf);
			activityValue = activityValue - 1;
		}
	}

	//Finish work
	parallel_for(int(0), m_agentNumber, [&](int i) {
		asend(m_bufferList[i], Message(stop));
	});

	activityValue = m_agentNumber;
	while (activityValue) {
		response = receive(source);
		if (response.m_action == ready) {
			activityValue--;
		}
	}

	parallel_for(int(0), m_agentNumber, [&](int i) {
		agent::wait(m_agentList[i]);
	});

	RemoveAgents(removeList);
	done();
}

