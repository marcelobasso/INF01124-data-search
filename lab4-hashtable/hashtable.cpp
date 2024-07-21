#include "hashtable.h"

using namespace std;

vector<vector<Player>> create_hashtable(string file, int sizeHash, ofstream &output) {
    vector<vector<Player>> hashtable(sizeHash);
    string temp;
    ifstream input(file);
    auto start = chrono::high_resolution_clock::now();
    int occupied = 0, list_size, max_list = 0, entries = 0;

    // ignores the header of the file
    getline(input, temp);
    while (getline(input, temp)) {
        list_size = insertHash(temp, hashtable, sizeHash, &occupied);
        if (list_size > max_list) max_list = list_size;
        entries++;
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    output << "PARTE 1: ESTATISTICAS DA TABELA HASH" << endl;
    output << "TEMPO DE CONSTRUCAO DA TABELA: " << duration << "ms" << endl;
    output << "TAXA DE OCUPACAO: " << (float) occupied/sizeHash << endl;
    output << "TAMANHO MAXIMO DE LISTA: " << max_list << endl;
    output << "TAMANHO MEDIO DE LISTA: " << entries/occupied << endl;
    
    input.close();
    return hashtable;
}

int insertHash(string stats_player, vector<vector<Player>> &hashtable, int sizeHash, int *occupied) {
    stringstream ss(stats_player);
    string temp;
    Player auxPlayer;
    vector<string> dados;
    int key;

    for (int i=0; i<2;i++) {
        getline(ss, temp, ',');
        dados.push_back(temp);
    }
    getline(ss, temp);
    dados.push_back(temp);
    auxPlayer.sofifa_id = stoi(dados[0]);
    auxPlayer.name = dados[1];
    auxPlayer.positions = dados[2];
    key = auxPlayer.sofifa_id % sizeHash;

    //cout << hashtable[key].size(); 
    if (hashtable[key].empty()) (*occupied)++;
    hashtable[key].push_back(auxPlayer);

    return hashtable[key].size();
}

Player searchHash(int fifa_id, vector<vector<Player>> &hashtable, int &tests, int &found) {
    int i = 0;
    int key = fifa_id % hashtable.size();
    Player auxPlayer;

    auxPlayer.sofifa_id=fifa_id;
    auxPlayer.name="NAO ENCONTRADO";
    auxPlayer.positions="";

    if(!hashtable[key].empty()){
        while ((i < hashtable[key].size()) && hashtable[key][i].sofifa_id != fifa_id) i++;
        if (hashtable[key][i].sofifa_id == fifa_id) {
            tests+=i;
            found++;
            auxPlayer.name=hashtable[key][i].name;
            auxPlayer.positions=hashtable[key][i].positions;
        }
    }

    return auxPlayer;
}

void searchHashArq(vector<vector<Player>> &hashtable, ofstream &output) {
    ifstream input("input/consultas.csv");
    string temp;
    Player auxPlayer;
    vector<Player> consultas;
    int n_test = 0, deltaTest = 0, maxTest = 0, found = 0;

    output << "PARTE 2: ESTATISTICAS DAS CONSULTAS" << endl;
    auto start = chrono::high_resolution_clock::now();

    while (getline(input, temp)) {
        deltaTest = n_test;
        consultas.push_back(searchHash(stoi(temp), hashtable, n_test, found));
        deltaTest = n_test - deltaTest;
        if (deltaTest >= maxTest) maxTest = deltaTest;
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    output << "TEMPO DE REALIZACAO DE TODAS AS CONSULTAS: " << duration << "ms"<< endl;

    for (int i=0; i<consultas.size(); i++) {
        output << consultas[i].sofifa_id << " " << consultas[i].name << endl;
    }

    output << "MAXIMO DE NUMERO DE TESTES POR NOME ENCONTRADO: " << maxTest << endl;
    output << "MEDIA NUMERO DE TESTES POR NOME ENCONTRADO: " << n_test/found << endl;

    input.close();
    output.close();
}

