#include "myVector.h"

using namespace mvec;
int main() {
	myVector v{7, 8, 9, 10};
	myVectorIterator it, end;
	end = v.end();
	for (it = v.begin(); it != end; ++it)
		std::cout << *it << ' ';

	return 0;
}