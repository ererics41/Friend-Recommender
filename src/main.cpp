#include "BTree.h"
#include "Graph.h"
#include <iostream>
#include <vector>
#include <limits>
#include <sstream>

using namespace std;

int main(){

	Graph g;

	cout << "Welcome to the friend recommender." << endl
		 << "1) Input a file." << endl
		 << "2) Add user." << endl
		 << "3) Find a user." << endl
		 << "4) Find a user's details." << endl
		 << "5) Recommend friends." << endl
		 << "6) Exit" << endl
		 << "Which operation would you like to make? (1, 2, 3, 4, 5, 6)." << endl;

	string option;
	while(true){
		getline(cin, option);
		if(option == "1"){
			g.inputFile();
			cout << "Please enter 1, 2, 3, 4, 5, 6 to continue." << endl;
		}else if(option == "2"){
			string name;
			int perm;
			string genre1;
			string genre2;
			string list;
			vector <int> friends;
			cout << "Adding a new user." << endl
				 << "Please enter the name." << endl;
			getline(cin, name);
			cout << "Please enter the perm." << endl;
			cin >> perm;
			while(cin.fail()){
				string buffer;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid perm entered. Enter an integer value for perm." << endl;
				cin >> perm;
			}
			getline(cin,option);
			cout << "Please enter genre 1." << endl;
			getline(cin, genre1);
			cout << "Please enter genre 2." << endl;
			getline(cin, genre2);
			cout << "Please enter a list of friends perms delimited by a space." << endl;
			getline(cin, list);
			stringstream stream(list);
			string tok;
    		friends.clear();
    		while(getline(stream, tok, ' ')) {
    			if(tok.size()>0)
    				try{
	    				friends.push_back(stoi(tok));
	    			}catch(invalid_argument i){
	    				cout << "Error invalid arugment passed as a perm" << endl;
	    			}
    		}
    		g.insert(perm, name, genre1, genre2, friends, true, true);
			cout << "Please enter 1, 2, 3, 4, 5, 6 to continue." << endl;
		}else if(option == "3"){
			int perm;
			cout << "Please enter the perm of the user to search for: ";
			cin >> perm;
			while(cin.fail()){
				string buffer;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid perm entered. Enter an integer value for perm." << endl;
				cin >> perm;
			}
			getline(cin, option);
			g.search(perm);
			cout << "Please enter 1, 2, 3, 4, 5, 6 to continue." << endl;
		}else if(option == "4"){
			int perm;
			cout << "Please enter the perm of the user to search for: ";
			cin >> perm;
			while(cin.fail()){
				string buffer;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid perm entered. Enter an integer value for perm." << endl;
				cin >> perm;
			}
			getline(cin, option);
			g.searchDetails(perm);
			cout << "Please enter 1, 2, 3, 4, 5, 6 to continue." << endl;
		}else if(option == "5"){
			int perm;
			cout << "Please enter the perm of the user to recommend for: ";
			cin >> perm;
			while(cin.fail()){
				string buffer;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid perm entered. Enter an integer value for perm." << endl;
				cin >> perm;
			}
			getline(cin, option);
			g.recommend(perm);
			cout << "Please enter 1, 2, 3, 4, 5, 6 to continue." << endl;
		}else if(option == "6"){
			cout << "Exitting application";
			break;
		}else{
			cout << "Invalid input. Please select an option from 1, 2, 3, 4, 5, 6." << endl;
		}
	}
}




/*cout << "Input a file name. ";

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
    	cout << perm << " " << name << " " << genre1 << " " << genre2 << " ";
    	for (vector<int>::iterator it = friends.begin() ; it != friends.end(); ++it)
    		cout  << *it << " ";
    	cout << endl;
	}

	myfile.close();
	/*int n;
	cout << "enter number of entries into tree" <<endl;
	cin >> n;
	BTree tree;
	User **u1 = new User*[n];
	int perms[n];
	for(int i = 0; i < n; i++){
		bool found = true;
		while(found){
			found = false;
			perms[i] = rand()% 500;
			for(int j = 0; j <i; j++){
				if(perms[i] == perms[j]){
					found = true;
				}
			}
		}
		User *u = new User("Eric", perms[i], "fs","fsd");
		u1[i] = u;
	}

	for(int i = 0; i < n; i++){
		cout << perms[i] << " ";
	}
	cout <<endl;

	for(int i = 0; i < n; i++){
		tree.insert(u1[i]);
		//tree.print();
	}
	tree.print();
	User *s = tree.search(perms[2]);
	if(s!= NULL)
		cout <<s->perm << endl;

	cout << "done" <<endl;*/