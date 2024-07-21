#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <math.h>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

typedef struct player {
    int sofifa_id;
    string name, positions;
} Player;

vector<vector<Player>> create_hashtable(string file, int sizeHash, ofstream &output);
int insertHash(string stats_player, vector<vector<Player>> &hashtable, int sizeHash, int *occupied);
Player searchHash(int fifa_id, vector<vector<Player>> &hashtable, int &tests, int &found);
void searchHashArq(vector<vector<Player>> &hashtable, ofstream &output);
