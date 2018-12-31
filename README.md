# Friend-Recommender
This console application uses an adjacency list to represent a network of people connected by friendships. A B-Tree is used to keep track of what where each user is in the adjacency list. The application has the following functions:
1. Read from an input file
   - Reads in a list of users to add to the network with different users starting on a new line and individual parameters of each user 
   being delimited by a comma. See given test documents for examples of the format of given users.
2. Add a user
   - Adds a user with a given name, perm, favorite genre 1, favorite genre 2 and a list of friends. If a user with that perm already exists
   nothing happens.
3. Find a user
   - Searches the B-Tree for a given user and returns if that user is found (meaning the user exists in the network).
4. Find a user's details
   - Searches the B-Tree for a given user and if found returns the user's details (name, and favorite genres)
5. Recommend Friends
   - Takes in a given user and recommends friends based on favorite genres. If a user has at least 1 matching favorite genre and is not 
   already a friend of the given user then that user is recommended. A depth first search is used to search the graph for such users. 
