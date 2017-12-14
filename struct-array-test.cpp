#include <stdio.h>
#include <stdlib.h>

struct Subsol {
	int cost, firstStep;
};

void checkAccess(struct Subsol** mySolTable) 
{
	printf("Checking access to mySolTable:\n");
	printf("mySolTable[0][0]\n");
	printf("cost: %d\n", mySolTable[0][0].cost);
	printf("1st:  %d\n", mySolTable[0][0].firstStep);

	if (mySolTable[0][1].cost) {
		printf("exists\n");
	}
	else {
		printf("null\n");
	}

};


struct Subsol** createSolTable(int n) {
    // we need a 1D array of pointers, one pointer for each row
    struct Subsol** solTable = (struct Subsol **) calloc(n,sizeof(struct Subsol*)); // here, n is the number of rows
    int i;
    // now this loop will allocate the memory for each row
    for (i=0;i<n;i++){
        // solTable[i] will be the pointer to row i
        solTable[i]=(struct Subsol*)calloc(n,sizeof(int)); // here n is the number of columns
    }
    // ecMatrix is the base pointer to the whole structure representing the 2D matrix
    //   it points to the 1D array of "pointers to rows"
    // ecMatrix[x] picks out a row, it is a pointer row x
    // ecMatrix[x][y] picks out a specific integer from the table: from column y of row x
    return solTable;
}


int main() {
	int testNum = 5;
	int nNodes = 5;

	struct Subsol **solTable;

	solTable = createSolTable(nNodes);

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

