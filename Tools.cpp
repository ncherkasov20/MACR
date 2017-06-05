#include "stdafx.h"

#include "Tools.h"

bool Tools::CheckAvailability(int ielem, vector<int>ivect) {
	int sizeVect = size(ivect);
	for (int i = 0; i < sizeVect; i++)
		if (ielem == ivect[i])
			return true;
	return false;
}

vector<int> Tools::UnionVectors(vector<int> iovect1, vector<int> ivect2) {
	int sizeVect1 = size(iovect1);
	int sizeVect2 = size(ivect2);
	bool avail;
	for (int i = 0; i < sizeVect2; i++) {
		avail = false;
		for (int j = 0; j < sizeVect1; j++)
			if (iovect1[j] == ivect2[i]) {
				avail = true;
				break;
			}
		if (avail == false)
			iovect1.push_back(ivect2[i]);
	}
	return iovect1;
}

vector<int> Tools::DifferenceOfVectors(vector<int> iovect1, vector<int> ivect2) {
	int sizeVect2 = ivect2.size();
	for (int i = 0; i < sizeVect2; i++) {
		int sizeVect1 = iovect1.size();
		for (int j = 0; j < sizeVect1; j++)
			if (iovect1[j] == ivect2[i]) {
				iovect1.erase(iovect1.begin() + j);
				break;
			}
	}
	return iovect1;
}