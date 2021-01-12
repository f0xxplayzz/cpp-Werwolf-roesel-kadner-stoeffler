#include <iostream>
#include <memory>

bool intInVector (int inputInt, std::vector<int> inputVector, bool output) {
	//checks whether an int is in the vector
	output = false;

	for (int i : inputVector) {
		if (inputInt == i) {
			output = true;
		}
	}
	return output;
}