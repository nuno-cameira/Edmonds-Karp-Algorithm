//============================================================================
//				Instituto Superior Tecnico
//			 Analise e Sintese de Algoritmos
//				     2 Projecto
//
// Date:		: Abril, 2014
// Author		: Nuno Cameira, 69769
// Author		: Vasco Loureiro, 70993
// Version		: 1.0.7.0
// Description	:
//============================================================================


#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif


#include <vector>
#include <stdio.h>
#include <iostream>
#include <list>
using namespace std;

class Node {

private:
	int _id;
	int _father;
	bool _visited;
	bool _closed;
	list<int> _connections;
	
public:
	Node() :
	        _id(-1), _visited(false), _closed(false), _connections(0) {}
	
	Node(int id) :
		_id(id), _visited(false), _closed(false), _connections(0) {}
		
		
	//Clone pattern
	virtual Node* clone() const {return(new Node(*this));};

	list<int> getConnections() {
		return _connections;
	}
	
	void addConnection(int id) {
		_connections.push_back(id);
	}
	
	void removeConnection(int id){
	  _connections.remove(id);
	}
	
	int getID() const {
		return _id;
	}
	
	int getFather() {
		return _father;
	}
	
	bool isVisited() {
		return _visited;
	}
	
	bool isClosed() {
		return _closed;
	}

	void setVisited(bool visited) {
		_visited = visited;
	}
	
	void setFather(int father) {
		_father = father;
	}
	
	void setClosed(bool closed) {
		_closed = closed;
	}
	
	void resetState(){
		_father = -1;
		_visited = false;
		_closed = false;
	}
	
	std::list<int>::iterator firstConnection() {
		return _connections.begin();
	}
	
	std::list<int>::iterator lastConnection() {
		return _connections.end();
	}
	
	bool contains(int id) {
		for (std::list<int>::iterator it = this->firstConnection();
				it != this->lastConnection(); ++it) {
			if(*it == id){
				return true;
			}
		}
		return false;
	}
	
	bool operator!=(const Node &other) const {
		return (_id != other._id);
	}
	
};



class Stack {

private:
	list<int> visitedOrder;
	
public:
	Stack() :
			visitedOrder(0) {
	}

	void pushBack(int val) {
		visitedOrder.push_back(val);
	}

	void popFront() {
		visitedOrder.pop_front();
	}
	
	int size(){
	  return visitedOrder.size();
	}
	
	int getFrontElement(){
	  return visitedOrder.front();
	}
	
	list<int> getList(){
		return visitedOrder;
	}
	
	void clear(){
		visitedOrder.clear();
	}
	
	// Prints out Stack info
	friend ostream &operator<<( ostream &output, Stack &s ){
		
		output << "STACK: ";
		list<int> list = s.getList();
		for(int i = 0; i < (int) s.getList().size(); i++){
			output << list.front() << " " ;
			list.pop_front();
		}	
        return output;
    }

};


// Prints out list info
ostream &operator<<( std::ostream& output, list<int> &l){
	
	list<int> listCopy = l;
		for(int i = 0; i < (int) l.size(); i++){
			output << listCopy.front() << " " ;
			listCopy.pop_front();
		}

    return output;
}


// Resets the state of each Node in the graph
void resetStates(vector<Node> *graph, int graphSize){
	
	for(int i = 0; i < graphSize; i++){
		(*graph)[i].resetState();
	}
}


// Does a Breadth-First Search
// returns a list with the selected path, including the source and the end
list<int> BFS(vector<Node> *graph, int source, int end){

	list<int> answer;
	Node *nodeInProcess;
	
	bool asSolution = false; // tells if the BFS ended with a solution 

	// FIFO list
	Stack stack = Stack();
	stack.pushBack(source);
	while(stack.size() != 0){
		DEBUG_MSG(stack);
		nodeInProcess = &(*graph)[stack.getFrontElement()];
		stack.popFront();
		
		Node *nextConnection = 0;
	
		for (std::list<int>::iterator it = nodeInProcess->firstConnection();
				it != nodeInProcess->lastConnection(); ++it) {
		
				nextConnection = &(*graph)[*it];
		
				if(nextConnection->getID() != nodeInProcess->getID()){			
					if(!nextConnection->isVisited() && !nextConnection->isClosed()) {
						nextConnection->setVisited(true);
						nextConnection->setFather(nodeInProcess->getID());
						stack.pushBack(nextConnection->getID());
						if(nextConnection->getID() == end){
							stack.clear();
							asSolution = true;
							DEBUG_MSG("SOLUTION - BREAK");
							break;
						}
					}
				}
		}
		nodeInProcess->setClosed(true);
	}
  
	if(asSolution){
		for (int node = (*graph)[end].getID(); node != source;
			node = (*graph)[node].getFather()){
				answer.push_front(node);
		}
		answer.push_front(source);
	}
	else{
		answer.push_front(-1);
		DEBUG_MSG("NO SOLUTION");
	}
	
	DEBUG_MSG("PATH: " << answer);
	
	return answer;
}



// Sends the flow and updates the graph
// the argument augmentedPath as the source and the end Node within
void sendFlow(vector<Node> *graph, list<int> *augmentedPath){
	
	int currentNode;
	int nextNode;
	
	int augmentedPathSize = augmentedPath->size() - 1;
	
	//the size is -1 so that the last node isn't processed
	//because the last node was updated on the previous iteration
	for (int i = 0; i < augmentedPathSize; i++) {
		currentNode = augmentedPath->front();
		augmentedPath->pop_front();
		nextNode = augmentedPath->front();
		(*graph)[currentNode].removeConnection((*graph)[nextNode].getID());
		
		DEBUG_MSG("REMOVE");
		DEBUG_MSG("source: " << (*graph)[currentNode].getID() 
			 << " end: " << (*graph)[nextNode].getID());
		
		
		if(!(*graph)[nextNode].contains((*graph)[currentNode].getID())){
			(*graph)[nextNode].addConnection((*graph)[currentNode].getID());
			
			DEBUG_MSG("ADICIONA");
			DEBUG_MSG("source: " << (*graph)[nextNode].getID() 
				 << " end: " << (*graph)[currentNode].getID());
		}
	}
}



// Applies an Edmonds–Karp based algorithm
// returns the total flow of the end node
int karpVisit(vector<Node> *graph, int source, int end, int graphSize){	
	
	vector<Node> graphCopy;
	
	// Makes a copy of the origial graph
	for(int j = 0; j < graphSize; j++){
		graphCopy.push_back(*(*graph)[j].clone());
	}
		
	int totalFlow = 0;
	list<int> augmentedPath;
	while(true){
	  //finds augmented paths
	  augmentedPath = BFS(&graphCopy, source, end);
		if(augmentedPath.front() != -1){
			sendFlow(&graphCopy, &augmentedPath);
			resetStates(&graphCopy, graphSize);
			totalFlow++;
		}
		else {
		  break;
		}
	}

	DEBUG_MSG("TOTAL FLOW: " << totalFlow);
	
	return totalFlow;
}



int main() {
 
	int numberOfPoints = 0;
	int numberOfConnections = 0;
	
	//builds the graph
	scanf("%i %i", &numberOfPoints, &numberOfConnections);
	vector<Node> graph;
	for (int i = 0; i < numberOfPoints; i++) {
		graph.push_back(Node(i));
	}	
	
	int pointA = 0;
	int pointB = 0;
	//sets the connections on the graph
	for (int i = 0; i < numberOfConnections; i++) {
		scanf("%i %i", &pointA, &pointB);
		graph[pointA].addConnection(pointB);
		graph[pointB].addConnection(pointA);
	}
	
	int numberOfProblems = 0;
	//reads number of problems to solve
	scanf("%i", &numberOfProblems);
	int numberOfCriticalPoints = 0;
	int criticPoint = 0;
	
	//solves each problem	
	for (int problem = 0; problem < numberOfProblems; problem++) {
	DEBUG_MSG("Solving problem #" << (problem + 1));

	//reads number of critical points
	scanf("%i", &numberOfCriticalPoints);
	vector<int> criticalPoints;
	//stores the critical points
	for (int in = 0; in < numberOfCriticalPoints; in++) {
		scanf("%i", &criticPoint);
		criticalPoints.push_back(criticPoint);
	}

	//the answer starts with an unreal value
	//so that it gets replaced in the first comparison
	int answer = 9999;
	int karpAnswer;
	
	//applies algorithm with a source and an end point, both critical point
	for (int currentCriticalPoint = 0;
		 currentCriticalPoint < (int) criticalPoints.size()-1;
		 currentCriticalPoint++) {
		
		for (int nextCriticalPoint = currentCriticalPoint+1;
			 nextCriticalPoint < (int) criticalPoints.size();
			 nextCriticalPoint++) {
				
				int source = criticalPoints[currentCriticalPoint];
				int end = criticalPoints[nextCriticalPoint];
				DEBUG_MSG("KARP  source: " << source << " end: " << end);
				karpAnswer = karpVisit(&graph, source, end, numberOfPoints);
				// compares the previous answer with the new answer
				answer = min(answer, karpAnswer);
		}
	}				

	//gives the answer of the problem
	cout << answer << endl;
	}

	return 0;
}

