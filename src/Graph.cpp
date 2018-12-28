#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Graph::Graph(){
	tree = new BTree();
	size = 0;
	traversed = false;
}

void Graph::insert(int perm, string name, string genre1, string genre2, vector<int> friends, bool update, bool print){
	//insert into the BTree
	if(tree->search(perm) == NULL){
		Node *node = new Node();
		node->perm = perm;
		node->traversed = traversed;
		User *u = new User(name, perm, genre1, genre2);
		u->index = size;
		list.push_back(node);
		size++;
		tree->insert(u);
		Node *current = node;
		Node *newNode;
		for (vector<int>::iterator it = friends.begin() ; it != friends.end(); ++it){
			User *f = tree->search(*it);
			if(f != NULL || !update){
				newNode = new Node();
				newNode->perm = *it;
				newNode->traversed = traversed;
				current->next = newNode;
				current = newNode;
				if(update && f != NULL)
					insertEdge(list[f->index], perm);
			}
		}
		if(print)
			cout << "User successfully inserted." <<endl;
	}else{
		if(print)
			cout << "User already exists in the graph and tree." << endl;
	}
}

void Graph::inputFile(){
	cout << "Input a file name. ";

	string input;
	getline(cin, input);

	ifstream myfile(input);
	
	string name;
	int perm;
	string genre1;
	string genre2;
	vector <int> friends;
	string tok;
	int count;
	while (getline(myfile, input)){
		stringstream stream(input);
		count = 0;
    	friends.clear();
    	while(getline(stream, tok, ';')) {
    		switch(count){
    		case 0:
    			perm = stoi(tok);
    			break;
    		case 1:
    			name = tok;
    			break;
    		case 2:
    			genre1 = tok;
    			break;
    		case 3:
    			genre2 = tok;
    			break;
    		default:
    			//cout << tok << endl;
    			friends.push_back(stoi(tok));
    		}
    		count++;
    	}
    	/*cout << perm << " " << name << " " << genre1 << " " << genre2 << " ";
    	for (vector<int>::iterator it = friends.begin() ; it != friends.end(); ++it)
    		cout  << *it << " ";
    	cout << endl;*/

    	insert(perm, name, genre1, genre2, friends, false, false);
	}

	myfile.close();
	cout << "Input file sucessfully inserted." << endl;
	//tree->print();
}

void Graph::search(int perm){
	User* user = tree->search(perm);
	if(user == NULL){
		cout << "User not found." << endl;
	}else{
		cout << "User " << perm << " was successfully found in the B-Tree." << endl;
	}
}

void Graph::searchDetails(int perm){
	User* user = tree->search(perm);
	if(user == NULL){
		cout << "User not found." << endl;
	}else{
		cout << "User found. User details: ";
		printUser(user);
		cout << "Friends: ";
		Node *current = list[user->index]->next;
		while(current != NULL){
			cout << current->perm << " ";
			current = current->next;
		}
		cout << endl;
	}
}

//traverses the graph using dfs
void Graph::recommend(int perm){
	//clear the recommended friends
	recommended.clear();

	//search for the input perm and make sure it's in the tree
	User *u = tree->search(perm);
	if(u == NULL){
		cout << "User " << perm << " doesn't exist." << endl;
		return;
	}

	//call recursive function recommendHelper to do dfs
	recommendHelper(perm, tree->search(perm));

	//print our the recommended friends
	cout << "Here are your recommended friends." <<endl; 
	for(vector<User *>::iterator it = recommended.begin() ; it != recommended.end(); ++it){
		printUser(*it);
	}
	traversed = !traversed;
}

void Graph::recommendHelper(int perm, User *orig){
	//search for user
	User *user = tree->search(perm);

	//mark node as traversed
	list[user->index]->traversed = !traversed;
	Node *current = list[user->index]->next;
	if(user->perm != orig->perm){
		if(sharedGenre(user,orig) && notFriends(user->perm,orig->index)){
			recommended.push_back(user);
		}
	}

	//traverse the linked list of firends and recurse on the unvisited ones
	while(current != NULL){
		if(list[tree->search(current->perm)->index]->traversed == traversed)
			recommendHelper(current->perm, orig);
		current = current->next;
	}
}

bool Graph::notFriends(int u1Perm, int u2Idx){
	Node *current = list[u2Idx]->next;
	while(current != NULL){
		if(current->perm == u1Perm){
			return false;
		}
		current = current->next;
	}
	return true;
}

bool Graph::sharedGenre(User *user, User *orig){
	return (user->genre1 == orig->genre1 || user->genre1 == orig->genre2 || user->genre2 == orig->genre1 || user->genre2 == orig->genre2);
}

void Graph::printUser(User *u){
	cout << "< Name: " << u->name << ", Perm: " << u->perm << ", Genre1: " << u->genre1 << ", Genre2: " << u->genre2 << " >" << endl;
}

void Graph::insertEdge(Node *root, int perm){
	Node *insert = new Node();
	insert->perm = perm;
	insert->traversed = traversed;
	insert->next = root->next;
	root->next  = insert;
}