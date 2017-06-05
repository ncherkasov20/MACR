//File: Tools.h
//===================================================================
// Description: Class of tools for calculations
//===================================================================
// May, 2017             Created by                 Nikita Cherkasov
//===================================================================
#pragma once

#include<vector>

using namespace std;
/**
* @class Tools
* Инструменты для некоторых вычислений
*/
class  Tools {
public:
	Tools() {}

	~Tools() {}

	/**
	* Проверка наличия ielem в множестве ivect
	* @param ielem - элемент, наличие которого проверяется в ivect
	* @param ivect - множество, в котором проверяется наличие ielem
	* @return true, если ielem присутствует в ivect, и false в обратном случае
	*/
	static bool CheckAvailability(int ielem, vector<int> ivect);
	/**
	* Объединение множеств iovect1 и iovect2
	* @param iovect1 - все элементы
	* @param iovect2 - элеменнты которые надо удалить
	* @return ivect1, в котором лежат элементы из ivect1, которые не принадлежат ivect2
	*/
	static vector<int> UnionVectors(vector<int> ivect1, vector<int> ivect2);
	/**
	* Разность множеств iovect1 и iovect2
	* @param iovect1 - 
	* @param iovect2 -
	* @return iovect1
	*/
	static vector<int> DifferenceOfVectors(vector<int> iovect1, vector<int> ivect2);
};