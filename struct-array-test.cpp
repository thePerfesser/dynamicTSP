#include <stdio.h>

struct Subsol {
	int cost, firstStep;
};


void checkAccess(struct Subsol** mySolTable) 
{
	printf("Checking access to mySolTable:\n");
	printf("mySolTable[0][0]\n");
	printf("cost: %d\n", mySolTable[0][0].cost);
	printf("1st:  %d\n", mySolTable[0][0].firstStep);

};

int main() {
	int testNum = 5;

	struct Subsol **solTable[2][2];

	struct Subsol st1 = {
		.cost = 10,
		.firstStep = 5
	};

	solTable[0][0] = st1;

	printf("cost: %d\n", solTable[0][0].cost);
	printf("1st:  %d\n", solTable[0][0].firstStep);

	checkAccess(solTable);

	return 0;
}

