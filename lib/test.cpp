#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

#include "../include/test.h"
using namespace std;

namespace tester {

static bool isFile(const char *fileName) {
	ifstream infile(fileName);
	return infile.good();
}

Test::Test(void (*f)(), const char* n, const char* sol) {
	test = f;
	name = n;
	msg = false;
	pased = false;
	if (isFile(sol)) {
		expected = "";
		ifstream s(sol);
		string temp;
		bool nl = false;
		while (getline(s, temp)) {
			bool aux=false;
			if(temp.find("\r")!=string::npos){
				replace(temp.begin(),temp.end(),'\r','\n');
				aux=true;
			}
			expected += temp;
			if (!aux && !s.eof())
				expected += "\n";
		}
	} else
		expected = sol;
}
void Test::run() {
	if (msg)
		cout << "Test " << name << ":";
	// Redirect cout.
	streambuf* oldCoutStreamBuf = cout.rdbuf();
	stringstream strCout;
	cout.rdbuf(strCout.rdbuf());

	try {
		test();
	} catch (const char*& c) {
		// Restore old cout.
		cout.rdbuf(oldCoutStreamBuf);
		result = strCout.str();
		if (msg) {
			cout << "NOT Passed";
			cerr << c << " at " << name << endl;
		}
		return;
	}
	// Restore old cout.
	cout.rdbuf(oldCoutStreamBuf);
	if (msg)
		cout << "Passed";
	result = strCout.str();
	pased = result.compare(expected) == 0;
}

ostream & operator<<(ostream & os, const Test& t) {

	os << t.name << ":" << (t.pased ? "passed" : "not passed");
	if (!t.pased && t.msg) {
		os << "\nExpected:\n[" << t.expected << "]" << "\nReceived:\n["
				<< t.result << "]";
	}
	return os;
}
}
