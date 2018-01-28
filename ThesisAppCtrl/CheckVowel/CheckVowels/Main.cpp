#include <iostream>
#include <string>

#include "CheckVowel.h"

using namespace std;


int main(int argc, char **argv) {
	if (argc > 1) {
		string rootPath = string(argv[1]);
		CheckVowel(rootPath).Run();
	}
	else {
		cout << "ERROR: Must specify the rootPath.\n";
	}
	system("pause");
	return 0;
}