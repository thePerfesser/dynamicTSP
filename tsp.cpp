#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iomanip>

using namespace std;
// define a matrix type:

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
 void displayMatix(int** mat, int nRows, int nCols, ostream& outs) {
    int i,j;
    for(i=0;i<nRows;i++){
        for(j=0;j<nCols;j++){
            outs << setw(8) << mat[i][j];
        }
        outs <<"\n";
    }
 }

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

void displaySolution(int nNodes, int homeNode, TspPath* solution, double elapsedTime) {
    cout << "\n\nHome Node: " << homeNode << "\n";
    cout << "Total Nodes, N = " << nNodes << "\n";

    cout << "Tour: *" << homeNode << "* --> ";
    for(int i=0; i<nNodes-1; i++ ){
        cout << solution->path[i] << " --> ";
    }
    cout << "*" << homeNode << "*\n";
    cout << "Total Cost: " << solution->cost << "\n";
    cout << "Elapsed Time: " << elapsedTime;
}

TspPath *tspGreedy(int nNodes, int homeNode, int**edgeCosts);

int main()
{
    clock_t start, stop;
    srand(time(0));
    int nNodes = 10, lowCost=1, highCost=100, homeNode=0;

    TspPath *bruteSolution, *greedySolution;

    int **edgeCostMatrix;

    //while(1){ // infinite loop to check for memory leaks
        edgeCostMatrix = createZeroSquareMatrix(nNodes);
        fillRandomUndirectedEdgeCostMatrix(edgeCostMatrix,nNodes,lowCost,highCost);
        displayMatix(edgeCostMatrix,nNodes,nNodes,cout);

        start = clock();
        bruteSolution = tspBrute(nNodes,homeNode,edgeCostMatrix);
        stop = clock();
        displaySolution(nNodes, homeNode, bruteSolution, (stop-start)/(double)CLOCKS_PER_SEC);

        start = clock();
        greedySolution = tspGreedy(nNodes,homeNode,edgeCostMatrix);
        stop = clock();
        displaySolution(nNodes, homeNode, greedySolution, (stop-start)/(double)CLOCKS_PER_SEC);

        delete bruteSolution;
        delete greedySolution;
        deleteMatrix(edgeCostMatrix,nNodes);
    //}
    cin.get();
    return 0;
}

TspPath *tspGreedy(int nNodes, int homeNode, int**edgeCosts) {
    TspPath *solution = new TspPath(nNodes-1); // solution only contains the intermediate nodes
    int *nodesUsed = new int[nNodes]();
    nodesUsed[homeNode]=1; // mark the homeNode as already used
    int currentNode = homeNode;
    solution->cost=0;  int tmp;
    for(int i=0;i<nNodes-1;i++){
        // find smallest step to an unused node
        int leastEdgeCost, bestNextNode;
        int firstUnused=1;
        for(int j=0;j<nNodes;j++){
            tmp=nodesUsed[j];
            if ( !nodesUsed[j]  &&  ( edgeCosts[currentNode][j]<leastEdgeCost || firstUnused ) ) {
                firstUnused=0;
                leastEdgeCost = edgeCosts[currentNode][j];
                bestNextNode = j;
            }
        }
        nodesUsed[bestNextNode]=1;
        solution->path[i]=bestNextNode;
        solution->cost += leastEdgeCost;
        currentNode = bestNextNode;
    }
    solution->cost += edgeCosts[currentNode][homeNode]; // add in cost of hop back to home node
    delete nodesUsed;
    return solution;
}
