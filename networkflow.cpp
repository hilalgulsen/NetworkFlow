/*
Course:			Analysis of Algorithms II (BLG336E)
Lecturer:		Zehra Çataltepe
Name-Surname:	Hilal Gülşen
Number:			150120023
PROJECT 3
*/
#include <iostream>
#include <fstream>
#include <stack>
using namespace std;

class Network_Flow{
	public:
	Network_Flow(){};
	void initialize(char* inputFile1, char* inputFile2);
	bool DFS();
	void FordFulkerson();
	void writeOutput();
	int publicationCount;
	int reviewerCount;
	int nodeCount;
	int source;
	int sink;
	int **flowGraph;
	int **residualGraph;
	string *reviewers;
	string *publications;
	bool *visited;
	int *parents;
};



int main(int argc,char** argv){
	Network_Flow flow;
	flow.initialize(argv[1], argv[2]);	//Initialize graphs
	flow.FordFulkerson();	//Solve using Ford Fulkerson algorithm
	flow.writeOutput();		//Write assignings to output file
}

void Network_Flow::initialize(char* inputFile1, char* inputFile2){
	ifstream inputfile1,inputfile2;
	inputfile1.open(inputFile1);
	inputfile2.open(inputFile2);
	inputfile1>>reviewerCount;							//First line in reviewer.txt represents number of reviewers
	inputfile2>>publicationCount;						//First line in publications.txt represents number of publications
	
	nodeCount = reviewerCount + publicationCount + 2;	//There are source and sink nodes.Therefore, 2 is added to nodeCount.
	sink = nodeCount - 1;								//Sink node is last node
	source = 0;	                                        //Source node is first node
	//Memory is allocated										
	flowGraph = new int*[nodeCount];
	residualGraph = new int*[nodeCount];
	reviewers = new string[reviewerCount];
	publications = new string[publicationCount];
	visited = new bool[nodeCount];
	parents = new int[nodeCount];
	for(int i=0; i<nodeCount; i++){
		flowGraph[i] = new int[nodeCount];
		residualGraph[i] = new int[nodeCount];
	}
	//Flow Graph is initialized
	for(int i=0; i<nodeCount; i++){
		for(int j=0; j<nodeCount; j++){
			flowGraph[i][j] = 0;
		}
	}
	//From reviewer.txt reviewer name is taken to reviewers array and his/her capability is taken to flowGraph.
	//These capabilities are inserted between source and reviewers as edge.Reviewers indexes are started from 1.
	for(int i=0; i<reviewerCount; i++){
		inputfile1>>reviewers[i]>>flowGraph[source][i + 1];
	}
	//From publications.txt publication name is taken to publications array
	for(int i=0; i<publicationCount; i++){
		inputfile2>>publications[i];
	}
	//Reviewers cannot make review all of them.Therefore, 1 or 0 is inserted between reviewers and publications according to ability
	for(int i=0; i<publicationCount; i++){
		for(int j=0; j<reviewerCount; j++){
			if(reviewers[j].substr(0,4) == "Prof"){	//If reviewer is Professor, he/she can review all publications
				flowGraph[j + 1][reviewerCount + i + 1] = 1;
			}
			else if(reviewers[j].substr(0,7) == "AstProf"){	//If reviewer is Assistant Professor, he/she can review Conference and Workshop proceedings
				if(publications[i].substr(0,8) == "ConfProc" || publications[i].substr(0,8) == "WorkProc" ){
					flowGraph[j + 1][reviewerCount + i + 1] = 1;
				}
			}
			else if(reviewers[j].substr(0,3) == "Ast"){	//If reviewer is Assistant, he/she can review only workshop
				if(publications[i].substr(0,8) == "WorkProc"){
					flowGraph[j + 1][reviewerCount + i + 1] = 1;
				}
			}
		}
	}
	//Each publication must be reviewed by exactly certain number of reviewers
	//These numbers are inserted between publications and sink
	for(int i=0; i<publicationCount; i++){
		if(publications[i].substr(0,8) == "ConfProc"){	//If publication is conference proceeding, it must be reviewed by 3 reviewers
			flowGraph[i + reviewerCount + 1][nodeCount - 1] = 3;
		}
		else{	//If publication is journal or workshop proceeding, it must be reviewed by 2 reviewers
			flowGraph[i + reviewerCount + 1][nodeCount - 1] = 2;
		}
	}
	//Copy flow graph to residual graph
	for(int i=0; i<nodeCount; i++){
		for(int j=0; j<nodeCount; j++){
			residualGraph[i][j]=flowGraph[i][j];
		}
	}
	//Close files
	inputfile1.close();
	inputfile2.close();
}

bool Network_Flow::DFS(){
	//Set all nodes not visited
	for(int i=0; i<nodeCount; i++){
		visited[i]=false;
	}
	//Create stack and start from source node.Push source to stack, set visited and make parent node -1. Because, there is no parent node of source.
	stack<int>flowStack;
	flowStack.push(source);
	visited[source]=true;
	parents[source]=-1;

	while(!flowStack.empty()){
		int topNode = flowStack.top();	//Take top of stack
		flowStack.pop();				//Pop it from stack				
		for(int i=0; i<nodeCount; i++){	//For every node in graph
			if(!visited[i] && residualGraph[topNode][i] > 0){	//If node is not visited and there is a path from top of stack to node
				flowStack.push(i);							//Push it to stack
				parents[i] = topNode;						//Set node's parent as top of stack
				visited[i] = true;							//Set node visited
			}
		}
	}
	return visited[sink];		//If sink is visited, this means there is a path from source to sink
}
void Network_Flow::FordFulkerson(){
	int path, node, parent;
	while(DFS()){	//If there is a path from source to sink
		node = sink;	//Start from sink
		path = 99999999;	//Path is large number to find minimum path using comparison
		while(node!=source){	//Up to the source, take parent of node and check edge between parent and node
			parent = parents[node];	
			if(residualGraph[parent][node] <path){	//If it is smaller than path value, update path value
				path = residualGraph[parent][node];
			}
			node = parent;	//Set node as its parent and go through like this until source
		}
		//Now, path is minimum path value
		node = sink;	//Start from sink
		while(node!=source){	//Up to the source, take parent of node
			parent = parents[node];
			residualGraph[parent][node] -= path;	//Remove flow value from edge between parent and node
			residualGraph[node][parent] +=path;		//Add flow value opposite direction
			node = parent;	//Set node as its parent and go through like this until source
		}
	}
}

void Network_Flow::writeOutput(){
	ofstream outputFile;
	outputFile.open("output.txt");
	string reviewedBy="";	//To keep reviewers of publication
	for(int i = reviewerCount + 1; i<sink; i++){
		for(int j=1; j< reviewerCount + 1; j++){
			if(residualGraph[i][j]!=0){	//If there is edge between reviewer and publication, it means that this publication is reviewed by this reviewer
				if(reviewedBy==""){	//If reviewer is first reviewer, it is added without comma
					reviewedBy = reviewers[j-1];
				}
				else{	//If it is not, it is added with comma
					reviewedBy = reviewedBy + "," + reviewers[j-1];
				}
			}
		}
		if(reviewedBy==""){	//If there is no reviewer which makes review this publication
			outputFile<<publications[i-reviewerCount-1]<<" is NOT assigned.\n";	//It is assigned to none of the reviewers
		}
		else{	//If publication is assigned
			outputFile<<publications[i-reviewerCount-1]<<" is assigned to "<<reviewedBy<<".\n";
			reviewedBy="";
		}
	}
}
	
