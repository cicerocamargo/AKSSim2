#include <string>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	string enumName;
	cout << "type enum name: ";
	cin >> enumName;



	string enumDeclaration;
	string arrayOfNames = "const char* " + enumName + "Names[] = {";
	enumDeclaration = "typedef enum {";

	cout << "type each value name and hit return key (type \"end\" to finish the sequence):\n";
	while(true) {
		string enumValue;
		cin >> enumValue;
		if (enumValue == "end") {
			break;
		}
		string currName = enumName + enumValue;
		arrayOfNames += "\n\t\"" + currName + "\",";
		enumDeclaration += "\n\t" + currName + ",";
	}
	arrayOfNames = arrayOfNames.substr(0, arrayOfNames.size()-1);
	enumDeclaration = enumDeclaration.substr(0, enumDeclaration.size()-1);
	arrayOfNames += "\n};\n";
	enumDeclaration += "\n} " + enumName + ";\n";

	cout << "\n------File.h------\n";
	cout << endl << enumDeclaration << endl;

	cout << "\n------File.cpp------\n";
	cout << arrayOfNames << endl;
	cout << "\nconst char* nameFromEnumValue(" << enumName << " value) {\n";
	cout << "\treturn " << enumName << "Names[value];\n";
	cout << "}\n\n";

	return 0;
}