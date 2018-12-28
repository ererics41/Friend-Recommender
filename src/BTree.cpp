#include "BTree.h"
#include <queue>
#include <iostream>
#include <sstream>
using namespace std;

BTree::BTree(){
	root = NULL;
}

bool BTree::empty(){
	return root == NULL;
}

void BTree::insert(User *entry){
	//case where tree is empty
	if(root == NULL){
		TreeNode *node = new TreeNode(true);
		insertLeaf(node,entry);
		root = node;
	}
	//case where there is only 1 node in the tree
	else if(root->isLeaf){
		//case where root is a leaf node and it's not full
		if(root->size < 2){
			insertLeaf(root, entry);
		}
		//case where root is leaf and is full, need to split into two nodes and add a new root
		else{
			//new leaf will be a sibling to the left of root
			TreeNode *newLeaf = new TreeNode(true);
			TreeNode *parent = new TreeNode(false);

			//sort the entries into the correct leaves
			if(entry->perm > root->data[1]->perm){
				insertLeaf(newLeaf, entry);
			}else{
				insertLeaf(newLeaf,root->data[1]);
				root->size--;
				insertLeaf(root,entry);
			}
			parent->children[0] = root;
			parent->children[1] = newLeaf;
			parent->size = 1;
			parent->keys[0] = newLeaf->minPerm;
			newLeaf->parent = parent;
			newLeaf->childNumber = 1;
			root->parent = parent;
			root = parent;
			root->minPerm = root->children[0]->data[0]->perm;
		}
	}
	//case where the root is not a leaf and we have at least 1 interior node
	else{
		//search for where in the tree the entry belongs 
		TreeNode *current = root;
		while(!current->isLeaf){
			TreeNode *next = NULL;
			for(int i = 0; i < current->size; i++){
				if(current->keys[i] <= entry->perm){
					next = current->children[i+1];
				}
			}
			if(next == NULL){
				next = current->children[0];
			}
			current = next;
		}
		
		//current is the leaf that we're trying to insert the entry into
		//case where that leaf is not full (has 1 element), insert in sorted order
		if(current->size < 2){
			insertLeaf(current, entry);
		}else{
			//the leaf we are trying to insert into is full, so check the sibling, sibling is the right sibling unless it's the rightmost child
			TreeNode* parent = current->parent;
			TreeNode* sibling = getSibling(current);
			//check if the sibling has room
			if(sibling->size < 2){
				//check if sibling is on the left or right
				if(sibling->childNumber > current->childNumber){
					//check if the the last element of current is less than our entry
					if(current->data[1]->perm < entry->perm){
						insertLeaf(sibling,entry);
					}else{
						insertLeaf(sibling,current->data[1]);
						current->size--;
						insertLeaf(current,entry);
					}
					parent->keys[current->childNumber] = sibling->data[0]->perm;
				}else{
					//check if the the last element of current is greater than our entry
					if(current->data[0]->perm > entry->perm){
						insertLeaf(sibling,entry);
					}else{
						insertLeaf(sibling,current->data[0]);
						current->data[0] = current->data[1];
						current->size--;
						insertLeaf(current,entry);
					}
				}
				current = parent;
				//traverse up the tree to update keys
				bool done = true;
				while(current != NULL){
					done = updateKeys(current);
					current = current->parent;
					if(done)
						return;
				}
			}else{
				//perform a split
				//declare a new leaf node that will be to the right of the current leaf
				TreeNode* leaf = new TreeNode(true);
				//insert the entry into the tree
				if(current->data[1]->perm < entry->perm){
					insertLeaf(leaf,entry);
				}else{
					insertLeaf(leaf,current->data[1]);
					current->size--;
					insertLeaf(current,entry);
				}
				leaf->childNumber = current->childNumber + 1;
				leaf->parent = current->parent;
				for(int i = parent->size; i >= leaf->childNumber; i--){
					parent->children[i+1] = parent->children[i];
					parent->children[i+1]->childNumber = i+1;
				}
				parent->children[leaf->childNumber] = leaf;
				parent->size++;

				//traverse up the tree to update keys and continue splitting if needed
				current = parent;
				while(current != NULL){
					//check on the number of children and split if needed
					if(current->size >= 4){
						//we have too many children children in this node

						//check if sibling can take them nodes
						sibling = getSibling(current);
						if(sibling != NULL && sibling->size < 3){
							//sibling is able to take some nodes
							if(sibling->childNumber > current->childNumber){
								//sibling is right
								for(int i = sibling->size; i >= 0; i--){
									sibling->children[i+1] = sibling->children[i];
									sibling->children[i+1]->childNumber = i+1;
								}
								sibling->size++;
								sibling->children[0] = current->children[current->size];
								current->size--;
								sibling->children[0]->childNumber = 0;
								sibling->children[0]->parent = sibling;
								updateKeys(sibling);
							}else{
								//sibling is left
								sibling->size++;
								current->size--;
								sibling->children[sibling->size] = current->children[0];
								sibling->children[sibling->size]->childNumber = sibling->size;
								sibling->children[sibling->size]->parent = sibling;
								updateKeys(sibling);
								for(int i = 0; i <= current->size; i++){
									current->children[i] = current->children[i+1];
									current->children[i]->childNumber = i;
								}
							}
						}else{
							TreeNode *newSibling = new TreeNode(false);
							newSibling->size = 1;
							newSibling->children[0] = current->children[3];
							newSibling->children[1] = current->children[4];

							//update the parent of the moved nodes
							newSibling->children[0]->parent = newSibling;
							newSibling->children[1]->parent = newSibling;

							//update child number of split nodes
							newSibling->children[0]->childNumber = 0;
							newSibling->children[1]->childNumber = 1;

							//update the size of current, child number and keys of new sibling
							current->size = 2;
							newSibling->childNumber = current->childNumber + 1;
							updateKeys(newSibling);

							//check if current is the root
							parent = current->parent;
							if(parent == NULL){
								TreeNode *newRoot = new TreeNode(false);
								newRoot->children[0] = root;
								newRoot->children[1] = newSibling;
								newRoot->size = 1;
								updateKeys(current);
								updateKeys(newRoot);
								current->parent = newRoot;
								newSibling->parent = newRoot;
								root = newRoot;
							}else{
								parent->size++;
								for(int i = parent->size; i > current->childNumber+1; i--){
									parent->children[i] = parent->children[i-1];
									parent->children[i]->childNumber = i;
								}
								parent->children[current->childNumber+1] = newSibling;
								newSibling->parent = parent;
							}
						}
					}

					//fix keys of the current node
					updateKeys(current);

					//update current
					current = current->parent;
				}
			}
		}
	}
}

//function returns the user if present, otherwise returns null
User* BTree::search(int perm){
	TreeNode *current = root;
	while(current != NULL){
		if(current->isLeaf){
			for(int i = 0; i < current->size; i++){
				if(current->data[i]->perm == perm){
					return current->data[i];
				}
			}
			current = NULL;
		}else{
			TreeNode *next = NULL;
			for(int i = 0; i < current->size; i++){
				if(current->keys[i] <= perm){
					next = current->children[i+1];
				}
			}
			if(next == NULL){
				next = current->children[0];
			}
			current = next;
		}
	}
	return NULL;
}

//debugging function that does bfs on the tree and prints nodes in that order
void BTree::print(){
	cout << "Printing the B-Tree" << endl;
	queue<TreeNode*> q;
	queue<int> parentNum;
	ostringstream oss;
	q.push(root);
	TreeNode *current;
	int nodeNum = 0;
	while(!q.empty()){
		oss.str("");
		oss.clear();
		current = q.front();
		q.pop();
		//create a string for the current node
		oss << "{ Node numer: " << nodeNum << ", "
			<< "isLeaf: " << current->isLeaf << ", "
			<< "size: " << current->size << ", "
			<< "minPerm: " << current->minPerm <<", ";
		if(!current->isLeaf){
			//create the necessary string for an interior node
			oss << "Keys: [ " ;
			for(int i = 0; i < current->size; i++){
				oss << current->keys[i] << " ";
			}
			oss <<"] parent: ";
			if(parentNum.empty() || current->parent == NULL){
				oss << "None,";
			}else {
				oss << parentNum.front() << ", ";
				parentNum.pop();
			}

			//push children of current interior node into the queue
			for(int i = 0; i < current->size+1; i++){
				q.push(current->children[i]);
				parentNum.push(nodeNum);
			}
		}else{
			//finish the string for a leaf node
			oss << "Data: [ " ;
			for(int i = 0; i < current->size; i++){
				oss << current->data[i]->perm << " ";
			}
			oss <<"] parent: ";
			if(parentNum.empty() || current->parent == NULL)
				oss << "None,";
			else 
				oss << parentNum.front() << ", ";
			parentNum.pop();
		}
		oss << " Child Number: " << current->childNumber <<" }";
		nodeNum++;
		cout << oss.str() << endl;
	}
}

//function that inserts entry to a leaf in sorted order
void BTree::insertLeaf(TreeNode *leaf, User *entry){
	if(leaf->size == 0){
		leaf->data[0] = entry;
	}else if(leaf->size == 1){
		if(entry->perm < leaf->data[0]->perm){
			leaf->data[1] = leaf->data[0];
			leaf->data[0] = entry;
		}else{
			leaf->data[1] = entry;
		}
	}
	leaf->size++;
	leaf->minPerm = leaf->data[0]->perm;
}

//function that fixes the keys of an interior node, returns true if no keys were changed
bool BTree::updateKeys(TreeNode *node){
	bool fixedKeys = true;
	TreeNode *child;
	for(int i = 0; i < node->size; i++){
		child = node->children[i+1];
		if(node->keys[i] != child->minPerm){
			fixedKeys = false;
			node->keys[i] = child->minPerm;
		}
	}
	node->minPerm = node->children[0]->minPerm;
	return fixedKeys;
}

//function that gets the sibling, chooses the right sibling if there's 2
TreeNode *BTree::getSibling(TreeNode *node){
	TreeNode* parent = node->parent;
	if(parent == NULL){
		return NULL;
	}
	TreeNode* sibling;
	if(node->childNumber < parent->size){
		sibling = parent->children[node->childNumber+1];
	}else{
		sibling = parent->children[node->childNumber-1];
	}
	return sibling;
}