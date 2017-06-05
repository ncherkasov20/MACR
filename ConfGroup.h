//File: ConfGroup.h
//===================================================================
// Description: Class of conflict groups
//===================================================================
// May, 2017             Created by                 Nikita Cherkasov
//===================================================================
#pragma once

#include<vector>

using namespace std;

/**
* @class ConfGroup
* Конфликтные группы друзей агентов
* Между ними разрешаются конфликты в системе
*/
class ConfGroup {
public:
	ConfGroup() {}

	ConfGroup( vector<int> iG1, vector<int> iG2 ): m_G1(iG1), m_G2(iG2){}

	~ConfGroup() {}
	/**
	* Получить группу G1
	* @ return m_G1
	*/
	vector<int> getG1();
	/**
	* Получить группу G2
	* @ return m_G2
	*/
	vector<int> getG2();
private:
	vector<int> m_G1, m_G2;//Conflicting groups of friends
};