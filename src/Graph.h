#ifndef GRAPH_H
#define GRAPH_H 

#include "BTree.h"
#include <vector>

struct Node{
	int perm;
	Node *next; 
	bool traversed;
};

class Graph{
public:
	Graph();
	void insert(int perm, std::string name, std::string genre1, std::string genre2, std::vector<int> friends, bool update, bool print);
	void inputFile();
	void search(int perm);
	void searchDetails(int perm);
	void recommend(int perm);
private:
	void insertEdge(Node *root, int perm);
	void recommendHelper(int perm, User *orig);
	bool notFriends(int u1Perm, int u2Idx);
	bool sharedGenre(User *user, User *orig);
	void printUser(User *u);
	std::vector<Node*> list;
	BTree *tree;
	bool traversed;
	int size;
	std::vector<User*> recommended;
};

#endif