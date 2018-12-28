#ifndef BTREE_H
#define BTREE_H 

#include <string>

struct User{
	User();
	User(std::string name, int perm, std::string genre1, std::string genre2){
		this->name = name;
		this->perm = perm;
		this->genre1 = genre1;
		this->genre2 = genre2;
		this->index = -1;
	};
	std::string name;
	int perm;
	std::string genre1;
	std::string genre2;
	int index;
};

struct TreeNode{
	TreeNode(bool isLeaf){ this->isLeaf = isLeaf; this->size = 0; this->parent = NULL; this->childNumber = 0; data = new User* [2]; children = new TreeNode* [5]; };
	TreeNode* parent;
	int minPerm;
	int childNumber;
	bool isLeaf;
	int size;
	User **data;
	int keys[4];
	TreeNode **children;
};

class BTree{
public:
	BTree();
	void insert(User *entry);
	User* search(int perm);
	void print();
	bool empty();
private:
	void insertLeaf(TreeNode *leaf, User *entry);
	bool updateKeys(TreeNode *node);
	TreeNode *getSibling(TreeNode *node);
	TreeNode *root;
};

#endif