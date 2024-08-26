#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <math.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include "parser.hpp"

#define SEPARATOR ' '
#define NAME_WIDTH 6
#define NUM_WIDTH 8

using namespace std;

typedef struct player {
    int sofifa_id, count = 0;
    float rating = 0;
    string name, shortname, positions, club, liga, nation;
} Player;

typedef struct user {
    int user_id;
    vector<pair<int,float>> players_rating;
} User;

typedef struct trie {
    char c;
    vector<int> value;
    struct trie* left;
    struct trie* mid;
    struct trie* right;
} Trie;

// @brief Builds Trie Tree and returns a pointer to its root.
// @param option 0-player, 1-tags
Trie* createTrie(string file, int option);

// @brief Adds item to the players Trie Tree.
Trie* insertTriePlayers(Trie* root, string& name, int value, int d);

// @brief Adds item to the tags Trie Tree.
Trie* intertTrieTags(Trie* root, string& name, int value, int d);

// @brief Creates Trie node.
Trie* createNode(char c, int value);

// @brief Searches for an element on a Trie tree.
vector<int> searchTrie(Trie* root, const std::string& name, int d);

// @brief Creates player hashtable.
vector<vector<Player>> createHashtablePlayers(string file, int sizeHash);

// @brief Creates users hashtable.
vector<vector<User>> createsHashtableUsers(string file, int sizeHash, vector<vector<Player>> &hashtable);

// @brief function to inser a new element on the players hashtable.
void insertHashPlayer (vector<string> stats_player, vector<vector<Player>>& hashtable, int sizeHash);

// @brief function to inser a new element on the users hashtable.
void insertHashUser(vector<string> review, vector<vector<Player>>& hashtablePlayer, vector<vector<User>>& hashtable, int sizeHash, int sizeHashP);

// @brief finds user index inside its position on the hashtable.
int returnIndexUser(vector<vector<User>>& hashtable, int sizeHash, int user_id);

// @brief fins player index inside its position on the hashtable.
int returnIndexPlayer(vector<vector<Player>>& hashtable, int sizeHash, int player_id);

// @brief Adds evaluation to the player.
void calcRating(vector<vector<Player>>& hashtable, int sizeHash, int player_id, float rating);

// @brief Searches on the hashtable.
Player searchHash(int fifa_id, vector<vector<Player>> &hashtable);
 
// @brief Prints menu of queries
void printHelp();

bool runQuery(const string query, vector<vector<Player>>& hashtableP, vector<vector<User>>& hashtableU, Trie* names, Trie* tags, int sizeHash);