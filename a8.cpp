// #include "tsp.cpp"
#include <stdio.h>
#include <math.h>

struct Subsol {
	int cost, firstStep;
};





//                    888    888  88888888888      8888888          888      .d88      88b.   
//                    888    888      888            888            888     d88P"      "Y88b  
//                    888    888      888            888            888    d88P          Y88b 
//  88888b.   8888b.  888888 88888b.  888   .d88b.   888   88888b.  888888 888            888 
//  888 "88b     "88b 888    888 "88b 888  d88""88b  888   888 "88b 888    888            888 
//  888  888 .d888888 888    888  888 888  888  888  888   888  888 888    Y88b          d88P 
//  888 d88P 888  888 Y88b.  888  888 888  Y88..88P  888   888  888 Y88b.   Y88b.      .d88P  
//  88888P"  "Y888888  "Y888 888  888 888   "Y88P" 8888888 888  888  "Y888   "Y88      88P"   
//  888                                                                                       
//  888                                                                                       
//  888                                                                                       

int pathToInt (int nNodes, int nodes[]) {
	int i;
	int result = 0;
	for (i = 0; i < nNodes; i++) {
		printf("i: %d\n", i);
		printf("result:     %d\n", result);
		printf("          + %d\n", (int)pow(2, i));
		printf("          ------------\n");
		result += pow(2, i);
		printf("new result: %d\n", result);
		
	}
	return result;
}




//  888b     d888          888            d8b                .d88888b.                    
//  8888b   d8888          888            Y8P               d88P" "Y88b                   
//  88888b.d88888          888                              888     888                   
//  888Y88888P888  8888b.  888888 888d888 888 888  888      888     888 88888b.  .d8888b  
//  888 Y888P 888     "88b 888    888P"   888 `Y8bd8P'      888     888 888 "88b 88K      
//  888  Y8P  888 .d888888 888    888     888   X88K        888     888 888  888 "Y8888b. 
//  888   "   888 888  888 Y88b.  888     888 .d8""8b.      Y88b. .d88P 888 d88P      X88 
//  888       888 "Y888888  "Y888 888     888 888  888       "Y88888P"  88888P"   88888P' 
//                                                                      888               
//                                                                      888               
//                                                                      888               

int** createZeroSquareMatrix(int n) {
    // we need a 1D array of pointers, one pointer for each row
    int** ecMatrix = (int **) calloc(n,sizeof(int*)); // here, n is the number of rows
    int i;
    // now this loop will allocate the memory for each row
    for (i=0;i<n;i++){
        // ecMatrix[i] will be the pointer to row i
        ecMatrix[i]=(int*)calloc(n,sizeof(int)); // here n is the number of columns
    }
    // ecMatrix is the base pointer to the whole structure representing the 2D matrix
    //   it points to the 1D array of "pointers to rows"
    // ecMatrix[x] picks out a row, it is a pointer row x
    // ecMatrix[x][y] picks out a specific integer from the table: from column y of row x
    return ecMatrix;
}

void deleteMatrix(int** m,int n){
    for(int i=0;i<n;i++){
        free(m[i]);
    }
    free(m);
}

void fillRandomUndirectedEdgeCostMatrix(int** ec, int n, int lo, int hi) {
    int i,j;
    for (i=0;i<n;i++) {
        for (j=i+1;j<n;j++){
            ec[i][j] = rand() % (1+hi-lo) + lo;
            ec[j][i] = ec[i][j]; // fill symmetric location with same value
        }
    }
}

int** createSolTable(int n) {
    // we need a 1D array of pointers, one pointer for each row
    struct Subsol** solTable = (struct Subsol **) calloc(n,sizeof(struct Subsol*)); // here, n is the number of rows
    int i;
    // now this loop will allocate the memory for each row
    for (i=0;i<n;i++){
        // solTable[i] will be the pointer to row i
        solTable[i]=(int*)calloc(n,sizeof(int)); // here n is the number of columns
    }
    // ecMatrix is the base pointer to the whole structure representing the 2D matrix
    //   it points to the 1D array of "pointers to rows"
    // ecMatrix[x] picks out a row, it is a pointer row x
    // ecMatrix[x][y] picks out a specific integer from the table: from column y of row x
    return ecMatrix;
}





//                         888    8888888b.                                           d8b            .d88      88b.   
//                         888    888  "Y88b                                          Y8P           d88P"      "Y88b  
//                         888    888    888                                                       d88P          Y88b 
//  88888b.d88b.   .d8888b 888888 888    888 888  888 88888b.   8888b.  88888b.d88b.  888  .d8888b 888            888 
//  888 "888 "88b d88P"    888    888    888 888  888 888 "88b     "88b 888 "888 "88b 888 d88P"    888            888 
//  888  888  888 888      888    888    888 888  888 888  888 .d888888 888  888  888 888 888      Y88b          d88P 
//  888  888  888 Y88b.    Y88b.  888  .d88P Y88b 888 888  888 888  888 888  888  888 888 Y88b.     Y88b.      .d88P  
//  888  888  888  "Y8888P  "Y888 8888888P"   "Y88888 888  888 "Y888888 888  888  888 888  "Y8888P   "Y88      88P"   
//                                                888                                                                 
//                                           Y8b d88P                                                                 
//                                            "Y88P"                                                                  


// workhorse recursively finds shortest route between start and end that includes visits all tourNodes
TspPath *mctDynamic(int startNode, int endNode, int nTourNodes, int tourNodes[], int** edgeCosts, Subsol** solTable) {
    TspPath *solution = new TspPath(nTourNodes); //solution from the worker will not include the start/end nodes
    int bestTourCost, i;



    // if solTable contains solution matching startNode and tourNodes
    // {
    	// return solution from table
    // }
    // } else {
    	if (nTourNodes==1) { // base case: only 1 tourNode between the start/end nodes
        	int onlyStep = tourNodes[0];
        	solution->path[0] = onlyStep; // the only available node is the only step
        	solution->cost = edgeCosts[startNode][onlyStep] + edgeCosts[onlyStep][endNode];
    	} else {
	        int *remainingTourNodes = new int[nTourNodes-1]; // allocate array to store remaining tour nodes for recursive call
        	// use tourNode[0] as initial first step, and fill remainingTour with 2nd through last nodes:
        	int firstStep, bestTourFirstStep, firstStepCost;
        	int nextFirstStep = tourNodes[0];
        	int tourCost;
        	TspPath* subTour;
        	TspPath* bestSubTour = nullptr;
        	for(i=0;i<nTourNodes-1;i++){ // initially the remaining tour nodes will be 2nd through last of the tour nodes
	            remainingTourNodes[i]=tourNodes[i+1];
        	}
        	// cycle through tourNodes as possible first steps
        	for (i=0 ; i < nTourNodes ; i++) {
	            firstStep = nextFirstStep;
            	firstStepCost = edgeCosts[startNode][firstStep];

            	subTour = mctDynamic(firstStep,endNode, nTourNodes-1, remainingTourNodes, edgeCosts, solTable);
            	tourCost = firstStepCost + subTour->cost;

            	if (tourCost < bestTourCost || i==0) { // if we have better solution, replace current best
	                bestTourCost = tourCost;
                	bestTourFirstStep = firstStep;
                	delete bestSubTour;
                	bestSubTour = subTour;
            	} else {
	                delete subTour;
            	}
            	// swap the firstStep node with next node from remainingNodes for next goaround
            	if ( i < nTourNodes-1 ) {
	                nextFirstStep = remainingTourNodes[i];
                	remainingTourNodes[i]=firstStep;
            	}
        	}
        	// construction solution.path from best first step and best subtour path...
        	solution->cost = bestTourCost;
        	solution->path[0] = bestTourFirstStep;
        	for(i=1; i<nTourNodes; i++) {
	            solution->path[i]=bestSubTour->path[i-1];
        	}
        	delete bestSubTour;
        	delete remainingTourNodes;
    	}
    	return solution;
	//}
}






//  888                      8888888b.                                           d8b            .d88      88b.   
//  888                      888  "Y88b                                          Y8P           d88P"      "Y88b  
//  888                      888    888                                                       d88P          Y88b 
//  888888 .d8888b  88888b.  888    888 888  888 88888b.   8888b.  88888b.d88b.  888  .d8888b 888            888 
//  888    88K      888 "88b 888    888 888  888 888 "88b     "88b 888 "888 "88b 888 d88P"    888            888 
//  888    "Y8888b. 888  888 888    888 888  888 888  888 .d888888 888  888  888 888 888      Y88b          d88P 
//  Y88b.       X88 888 d88P 888  .d88P Y88b 888 888  888 888  888 888  888  888 888 Y88b.     Y88b.      .d88P  
//   "Y888  88888P' 88888P"  8888888P"   "Y88888 888  888 "Y888888 888  888  888 888  "Y8888P   "Y88      88P"   
//                  888                      888                                                                 
//                  888                 Y8b d88P                                                                 
//                  888                  "Y88P"                                                                  

TspPath *tspDynamic(int nNodes, int homeNode, int**edgeCosts) {
    TspPath *solution;
    int *tourNodes = new int[nNodes-1];
    for(int i=0;i<nNodes-1;i++){
       	if(i<homeNode){
            tourNodes[i]=i;
       	} else {
            tourNodes[i]=i+1;
       	}
   	}
   	solution = tspBruteWorker(homeNode, homeNode, nNodes-1, tourNodes, edgeCosts);
   	delete tourNodes;
   	return solution;
}







//                         d8b            .d88      88b.   
//                         Y8P           d88P"      "Y88b  
//                                      d88P          Y88b 
//  88888b.d88b.   8888b.  888 88888b.  888            888 
//  888 "888 "88b     "88b 888 888 "88b 888            888 
//  888  888  888 .d888888 888 888  888 Y88b          d88P 
//  888  888  888 888  888 888 888  888  Y88b.      .d88P  
//  888  888  888 "Y888888 888 888  888   "Y88      88P"   
//                                                         
//                                                         
//                                                         


int main()
{
    clock_t start, stop;
    srand(time(0));
    int nNodes = 10, lowCost=1, highCost=100, homeNode=0;

    TspPath *bruteSolution;

    int **edgeCostMatrix, **solTable;

    //while(1){ // infinite loop to check for memory leaks
        edgeCostMatrix = createZeroSquareMatrix(nNodes);
        fillRandomUndirectedEdgeCostMatrix(edgeCostMatrix,nNodes,lowCost,highCost);
        displayMatix(edgeCostMatrix,nNodes,nNodes,cout);

        // build solTable
        solTable = createZeroSquareMatrix(nNodes); 

        start = clock();
        bruteSolution = tspBrute(nNodes,homeNode,edgeCostMatrix);
        stop = clock();
        displaySolution(nNodes, homeNode, bruteSolution, (stop-start)/(double)CLOCKS_PER_SEC);

        delete bruteSolution;
        deleteMatrix(edgeCostMatrix,nNodes);
    //}
    cin.get();
    return 0;
}