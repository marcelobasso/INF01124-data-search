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
int main() {
    int N, M, C;
    cin >> N;
    int key;
    for (int j = 0; j < N; j++) {
        cin >> M >> C;
        vector<vector<int>>hashtable(M);
        for (int i = 0; i < C; i++) {
            cin >> key;
            hashtable[key % M].push_back(key);
        }
        for (int i = 0; i < M; i++) {
            cout << i << " -> ";
            for (int n = 0; n < hashtable[i].size(); n++)
                cout << hashtable[i][n] << " -> ";
            cout << "\\" << endl;
        }
        if(j+1<N) cout << endl;
    }
    return 0;
}