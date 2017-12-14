// FIX IT NODES
// WHEN SOLUTION IS FOUND IN TABLE, THE STARTNODE IS ADDED A SECOND TIME TO THE SOLUTION.
// PROBLEM LIKELY IN BUILD PATH




#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iomanip>

using namespace std;

class TspPath {
public:
    int cost;   // cost ultimately includes the steps from the first and last node, as well as between all interanl nodes
    int *path;  // note: the way I'm using this, only the "internal" vertices of a tour are put in the path array here
    TspPath(int pathLength){
        cost=0;
        path = new int[pathLength];
    }
    // destructor
    ~TspPath(){
        delete path; // doing this properly so we don't leak the memory for all these path arrays
    }

};


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
		result += pow(2, nodes[i]-1);
	}
    result;
    //cout << "******** pathIndex for { ";
    //for (int a = 0; a < nNodes; a++) {
      //  cout << nodes[a] << " ";
    //}
    //cout << " } = " << result << endl;
	return result;
}






//  888               d8b 888      888                        888    888      
//  888               Y8P 888      888                        888    888      
//  888                   888      888                        888    888      
//  88888b.  888  888 888 888  .d88888      88888b.   8888b.  888888 88888b.  
//  888 "88b 888  888 888 888 d88" 888      888 "88b     "88b 888    888 "88b 
//  888  888 888  888 888 888 888  888      888  888 .d888888 888    888  888 
//  888 d88P Y88b 888 888 888 Y88b 888      888 d88P 888  888 Y88b.  888  888 
//  88888P"   "Y88888 888 888  "Y88888      88888P"  "Y888888  "Y888 888  888 
//                                          888                               
//                                          888                               
//                                          888                               


//  --- ITERATIVE VERSION

int* buildPathFromStartNode(int pathIndex, int nNodes, int startNode, struct Subsol** solTable) {
    //cout << "---------------------------------entering build path .. startNode = " << startNode << endl;
    int* subsolPath = (int *) calloc(nNodes - 1, sizeof(int));
    pathIndex -= pow(2, startNode-1);
    subsolPath[0] = solTable[pathIndex][startNode].firstStep;
    startNode = subsolPath[0];
    for (int a = 1; a < nNodes; a++) {
        pathIndex -= pow(2, startNode-1);
        subsolPath[a] = solTable[pathIndex][startNode].firstStep;
        startNode = subsolPath[a];
    }
    return subsolPath;
}

/*   --- RECURSIVE VERSION ---
int* buildPathFromStartNode(int pathIndex, int nNodes, int startNode, struct Subsol** solTable) {
    cout << "---------------------------------entering build path .. startNode = " << startNode << endl;
    if (nNodes == 1) {
        int* solPath = (int *) calloc(1,sizeof(int)); // here, n is the number of rows
        solPath[0] = startNode;
        cout << "build path returning with solPath = { " << solPath[0] << " }    solTable[][].firstStep = " << solTable[pathIndex][startNode].firstStep << endl;
        return solPath;
    }
    else {
        int* subsolPath = (int *) calloc(nNodes - 1, sizeof(int));
        int subsolStartNode = startNode;
        int subSolPathIndex = pathIndex - pow(2, subsolStartNode-1);
        subsolPath = buildPathFromStartNode(subSolPathIndex, nNodes - 1, solTable[pathIndex][startNode].firstStep, solTable);
        return subsolPath;
    }
}
*/


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

void displayMatrix(int** mat, int nRows, int nCols, ostream& outs) {
    int i,j;
    for(i=0;i<nRows;i++){
        for(j=0;j<nCols;j++){
            outs << setw(8) << mat[i][j];
        }
        outs <<"\n";
    }
 }


// build an empty solTable #rows=(2^n)-1  #cols=n
struct Subsol** createSolTable(int n) {

    int rows = pow(2, n) - 1;
    //cout << "Creating empty solution table: " << endl;
    //cout << "Rows: " << rows << endl;
    //cout << "Cols: " << n << endl;
    //cout << endl;

    struct Subsol** solTable = (struct Subsol **) calloc(pow(2, n) - 1,sizeof(struct Subsol*));
    int i;
    for (i = 0; i < rows; i++){
        solTable[i]=(struct Subsol*)calloc(n,sizeof(struct Subsol)); 
    }
    return solTable;
}

void deleteSolTable(struct Subsol** s,int n){
    for(int i=0;i<n;i++){
        free(s[i]);
    }
    free(s);
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
    int pathIndex;
    pathIndex = pathToInt(nTourNodes, tourNodes);
    
    /*
    cout << endl;
    cout << "Entering mctDynamic with the following parameters:" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "pathIndex:   " << pathIndex << endl;
    cout << "startNode:   " << startNode << endl;
    cout << "endNode:     " << endNode << endl;
    cout << "nTourNodes:  " << nTourNodes << endl;
    cout << "tourNodes[]: { ";
    for (int a = 0; a < nTourNodes; a++) {
        cout << tourNodes[a] << " ";
    }
    cout << "}" << endl;
    */
    // if solTable contains solution matching startNode and tourNodes

    //cout << "\nCheck: " << startNode << " { " ;
    //for (int a = 0; a < nTourNodes; a++) {
        //cout << tourNodes[a] << " ";
    //}
    //cout << "} ......";

    if (solTable[pathIndex][startNode].cost) {  // if solution exists
        //cout << "found" << endl;
        //cout << "\n    Solution for solTable[ " << pathIndex << " ][ " << startNode << " ] exists in table. \n     - { ";
        solution->cost = solTable[pathIndex][startNode].cost; // get solution cost from table
        //solution->path = buildPathFromStartNode(pathIndex, nTourNodes, startNode, solTable); 
        //cout << "Calling buildPathFromStartNode( " 
                   // << pathIndex << ", " 
                    //<< nTourNodes << ", " 
                    //<< solTable[pathIndex][startNode].firstStep << ", solTable)" << endl;
        solution->path = buildPathFromStartNode(pathIndex, nTourNodes, solTable[pathIndex][startNode].firstStep, solTable);
        //cout << "******* sol path for solTable[pathIndex][startNode]: { ";
        //cout << "\n    Solution for solTable[ " << pathIndex << " ][ " << startNode << " ] exists in table. \n     - { ";
        //for (int a = 0; a < nTourNodes; a++) {
            //cout << solution->path[a] << " ";
        //}
        //cout << " }" << endl;

    }
    else {
        //cout << "not found" << endl;
        //cout << "Solution DOES NOT exist in table. nTourNodes = " << nTourNodes << "." << endl;
    	if (nTourNodes==1) { // base case: only 1 tourNode between the start/end nodes
            //cout << "nTourNodes == 1" << endl;

        	int onlyStep = tourNodes[0];
        	solution->path[0] = onlyStep; // the only available node is the only step
        	solution->cost = edgeCosts[startNode][onlyStep] + edgeCosts[onlyStep][endNode];
    	
            // save solution in table
            solTable[pathIndex][startNode].firstStep = solution->path[0];
            solTable[pathIndex][startNode].cost = solution->cost;
            //cout << "\n    Solution DOES NOT exist in table. nTourNodes = " << nTourNodes << "." << endl;
            //cout << "         - INSERT solTable[" << pathIndex << "][" << startNode << "].firstStep = " << solTable[pathIndex][startNode].firstStep << endl;
        
        } else {
            //cout << "nTourNodes != 1" << endl;
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

            // save solution in table
            solTable[pathIndex][startNode].firstStep = solution->path[0];
            solTable[pathIndex][startNode].cost = solution->cost;
            //cout << "     - FOUND SOLUTION! solTable[" << pathIndex << "][" << startNode << "].firstStep = " << solTable[pathIndex][startNode].firstStep << endl;
            //cout << "\n    Solution DOES NOT exist in table. nTourNodes = " << nTourNodes << "." << endl;
            //cout << "         - INSERT solTable[" << pathIndex << "][" << startNode << "].firstStep = " << solTable[pathIndex][startNode].firstStep << endl;

        	delete bestSubTour;
        	delete remainingTourNodes;
    	}
    
        
    	
	}
    return solution;
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

TspPath *tspDynamic(int nNodes, int homeNode, int** edgeCosts, struct Subsol** solTable) {
    TspPath *solution;
    int *tourNodes = new int[nNodes-1];
    for(int i=0;i<nNodes-1;i++){
       	if(i<homeNode){
            tourNodes[i]=i;
       	} else {
            tourNodes[i]=i+1;
       	}
   	}
   	solution = mctDynamic(homeNode, homeNode, nNodes-1, tourNodes, edgeCosts, solTable);
   	delete tourNodes;
   	return solution;
}







//  888                       888            
//  888                       888            
//  888                       888            
//  88888b.  888d888 888  888 888888 .d88b.  
//  888 "88b 888P"   888  888 888   d8P  Y8b 
//  888  888 888     888  888 888   88888888 
//  888 d88P 888     Y88b 888 Y88b. Y8b.     
//  88888P"  888      "Y88888  "Y888 "Y8888  
//                                           
//                                           
//                                           

// workhorse recursively finds shortest route between start and end that includes visits all tourNodes
TspPath *tspBruteWorker(int startNode, int endNode, int nTourNodes, int tourNodes[], int** edgeCosts) {
    TspPath *solution = new TspPath(nTourNodes); //solution from the worker will not include the start/end nodes
    int bestTourCost, i;

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

            subTour = tspBruteWorker(firstStep,endNode, nTourNodes-1, remainingTourNodes, edgeCosts);
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
}

TspPath *tspBrute(int nNodes, int homeNode, int**edgeCosts) {
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







//       888 d8b                   888                    .d8888b.           888   .d88      88b.   
//       888 Y8P                   888                   d88P  Y88b          888  d88P"      "Y88b  
//       888                       888                   Y88b.               888 d88P          Y88b 
//   .d88888 888 .d8888b  88888b.  888  8888b.  888  888  "Y888b.    .d88b.  888 888            888 
//  d88" 888 888 88K      888 "88b 888     "88b 888  888     "Y88b. d88""88b 888 888            888 
//  888  888 888 "Y8888b. 888  888 888 .d888888 888  888       "888 888  888 888 Y88b          d88P 
//  Y88b 888 888      X88 888 d88P 888 888  888 Y88b 888 Y88b  d88P Y88..88P 888  Y88b.      .d88P  
//   "Y88888 888  88888P' 88888P"  888 "Y888888  "Y88888  "Y8888P"   "Y88P"  888   "Y88      88P"   
//                        888                        888                                            
//                        888                   Y8b d88P                                            
//                        888                    "Y88P"                                             


void displaySol(int nNodes, int homeNode, TspPath* solution, double elapsedTime) {
    cout << "\nHome Node: " << homeNode << "\n";
    cout << "Total Nodes, N = " << nNodes << "\n";

    cout << "Tour: *" << homeNode << "* --> ";
    for(int i=0; i<nNodes-1; i++ ){
        cout << solution->path[i] << " --> ";
    }
    cout << "*" << homeNode << "*\n";
    cout << "Total Cost: " << solution->cost << "\n";
    cout << "Elapsed Time: " << elapsedTime << endl;;
}

void displaySolMinimal(int nNodes, int homeNode, TspPath* solution, double elapsedTime) {
    cout << " N = " << nNodes << ". ";
    cout << "Tour: *" << homeNode << "* --> ";
    for(int i=0; i<nNodes-1; i++ ){
        cout << solution->path[i] << " --> ";
    }
    cout << "*" << homeNode << "*\n";
    //cout << "Total Cost: " << solution->cost << "\n";
    //cout << "Elapsed Time: " << elapsedTime << endl;;
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
    int nNodes = 11, lowCost=1, highCost=100, homeNode=0;

    TspPath *dynamicSolution, *bruteSolution;

    int **edgeCostMatrix;
    struct Subsol **solTable;


    for (int nNodes = 4; nNodes <= 10; nNodes++) {
    // build/allocate//fill edge cost matrix
    edgeCostMatrix = createZeroSquareMatrix(nNodes);
    fillRandomUndirectedEdgeCostMatrix(edgeCostMatrix,nNodes,lowCost,highCost);
    //displayMatrix(edgeCostMatrix,nNodes,nNodes,cout);

    // build / allocate solTable
    solTable = createSolTable(nNodes);
    
    // execute and time dynamic TSP solution
    //start = clock();
    //dynamicSolution = tspDynamic(nNodes,homeNode,edgeCostMatrix, solTable);
    //stop = clock();
    //cout << "Dynamic - ";
    //displaySolMinimal(nNodes, homeNode, dynamicSolution, (stop-start)/(double)CLOCKS_PER_SEC);
   //displaySol(nNodes, homeNode, dynamicSolution, (stop-start)/(double)CLOCKS_PER_SEC);



    // execute and time brute TSP solution
    start = clock();
    bruteSolution = tspBrute(nNodes,homeNode,edgeCostMatrix);
    stop = clock();
    //cout << "Brute   - ";
    //displaySolMinimal(nNodes, homeNode, bruteSolution, (stop-start)/(double)CLOCKS_PER_SEC);
    displaySol(nNodes, homeNode, bruteSolution, (stop-start)/(double)CLOCKS_PER_SEC);
    
 
    // memory management
    //delete dynamicSolution;
    delete bruteSolution;
    deleteMatrix(edgeCostMatrix,nNodes);
    deleteSolTable(solTable, nNodes);
    cout << endl;
    }
    
    /*
    cout << "Enter to quit...";
    cin.get();
    */
    return 0;
}