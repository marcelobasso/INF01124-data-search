#include "trabFinal.h"

#define PLAYERS "inputs/players.csv"
#define RATING "inputs/rating.csv"
// #define RATING "inputs/minirating.csv"
#define TAGS "inputs/tags.csv"

int main(){
    int sizeHashPos = 97;
    vector<vector<Player>> hashtableP;
    vector<vector<User>> hashtableU;
    vector<vector<int>>hashtablePos(sizeHashPos);
    Trie* names = nullptr;
    Trie* tags = nullptr;
    bool quit = 0;
    string input;
    vector<string> query;

    // PROCESSAMENTO DE DADOS
    auto start = std::chrono::high_resolution_clock::now();

    cout << "processing players..." << endl;
    hashtableP = createHashtablePlayers(PLAYERS, 7993, hashtablePos, sizeHashPos);
    names = createTrie(PLAYERS, 0);
    
    cout << "processing rating..." << endl;
    hashtableU = createsHashtableUsers(RATING, 7993, hashtableP);

    cout << "processing tags..." << endl;
    tags = createTrie(TAGS, 1);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Tempo de leitura: " << elapsed.count() << " segundos" << std::endl;

    // CONSULTAS
    while (!quit) {
        cout << "\n> ";
        getline(cin, input);
        
        quit = (input.substr(0, input.find(" ")) == "q");

        if (input == "help")
            printHelp();
        else
            runQuery(input, hashtableP, hashtableU, hashtablePos, names, tags, 7993, sizeHashPos);
    }

    // testes
    // string name = "OPA";
    // vector<int> playerNames = searchTrie(names, name, 0);
    // for (int i = 0; i < playerNames.size(); i++) {
    //     Player auxPlayer = searchHash(playerNames[i], hashtableP);
    //     cout << auxPlayer.name << "\t" << auxPlayer.rating / auxPlayer.count << endl;
    // }
    
    // string name = "Solid Player";
    // vector<int> players = searchTrie(tags, name, 0);
    // for (int i = 0; i < players.size(); i++) {
    //     Player auxPlayer = searchHash(players[i], hashtableP);
    //     cout << auxPlayer.name << "\t" << auxPlayer.rating / auxPlayer.count << endl;
    // }
    
    
    return 0;
}