
#include "stdafx.h"

#include <fstream>

#include "MAS.h"

using namespace std;

void main() {
	ifstream input("input.txt");
	ofstream output("output.txt");
	int lexNumber, agentNumber, conflictNumber;
	input >> lexNumber >> agentNumber >> conflictNumber;////Number agents, lex agents and amount conflict structure
	MAS MACR( lexNumber, agentNumber, conflictNumber );
	MACR.Init(input);//initialization MAS
	MACR.Start(output);//Start conflict resolution in MASAR
	input.close();
	output.close();
}
