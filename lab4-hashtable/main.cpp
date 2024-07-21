#include "hashtable.h"

int main() {
    vector<vector<Player>> hashtable;
    vector<string> exp = {"experimento997.txt", "experimento1999.txt", "experimento3989.txt", "experimento7993.txt"};
    ifstream input_f;

    int sizeHash[4] = {997, 1999, 3989, 7993};
    for (int i = 0; i < 4; i++) {
        ofstream output_f(exp[i]);
        cout << "Creating HashTable with size " << sizeHash[i] << "..."  << endl;
        hashtable = create_hashtable("input/players.csv", sizeHash[i], output_f);
        cout << "Searching..." << endl;
        searchHashArq(hashtable, output_f);

        output_f.close();
    }

    return 0;
}