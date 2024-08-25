#include "trabFinal.h"

Player searchHash(int fifa_id, vector<vector<Player>> &hashtable)
{
    int i=0;
    int key=fifa_id % hashtable.size();
    Player auxPlayer;
    int tam_coluna=hashtable[key].size();
    auxPlayer.sofifa_id=fifa_id;
    if(!hashtable[key].empty()){
        while((i<tam_coluna)&&hashtable[key][i].sofifa_id!=fifa_id) i++;
        if(hashtable[key][i].sofifa_id==fifa_id){
            auxPlayer=hashtable[key][i];
        }
    }
    return auxPlayer;
}

Trie* createNode(char c, int value ) {
    vector<int> items;

    items.push_back(value);
    return new Trie { c, items, nullptr, nullptr, nullptr };
}

Trie* createTrie(string file, int option) 
{
    ifstream f(file);
    aria::csv::CsvParser parser(f);
    Trie* root = nullptr;
    int sofifa_id;
    string name;
    
    // ignores first line (header)
    parser.begin();

    for (auto& row : parser) {
        name = row[2];
        sofifa_id = stoi(row[option]);
        root = !option ? insertTriePlayers(root, name, sofifa_id, 0) : intertTrieTags(root, name, sofifa_id, 0);
    }

    f.close();
    return root;
}

Trie* insertTriePlayers(Trie* root, string& name, int value, int d) 
{
    if (d == name.length()) return root;

    char c = tolower(name[d]);

    if (root == nullptr) {
        root = createNode(c, value); 
    }
    if(root->c==c && find(root->value.begin(), root->value.end(), value) == root->value.end()) 
        root->value.push_back(value);

    if (c < root->c) {
        root->left = insertTriePlayers(root->left, name, value, d);
    } else if (c > root->c) {
        root->right = insertTriePlayers(root->right, name, value, d);
    } else {
        root->mid = insertTriePlayers(root->mid, name, value, d + 1);
    }

    return root;
}

Trie* intertTrieTags(Trie* root, string& name, int value, int d)
{
    if (d == name.length()) return root;
    char c = tolower(name[d]);
    if (root == nullptr) {
        root = createNode(c, 0);
    }

    if (c < root->c) {
        root->left = intertTrieTags(root->left, name, value, d);
    } else if (c > root->c) {
        root->right = intertTrieTags(root->right, name, value, d);
    } else {
        if (d + 2 == name.length()) {
            if (root->mid == nullptr) root->mid = createNode(0, value);
            else if(find(root->mid->value.begin(), root->mid->value.end(), value) == root->mid->value.end())
                root->mid->value.push_back(value);
        }
        else root->mid = intertTrieTags(root->mid, name, value, d + 1);
    }

    return root;
}

vector<int> searchTrie(Trie* root, const string& name, int d) 
{
    if (root == nullptr || d >= name.length()) 
        return {};

    char c = tolower(name[d]);

    if (c < root->c) 
        return searchTrie(root->left, name, d);
    else if (c > root->c) 
        return searchTrie(root->right, name, d);
    else {
        if (d + 2 == name.length()) {
            if (root->mid != nullptr) 
                return root->mid->value;

            return {};
        }

        return searchTrie(root->mid, name, d + 1);
    }
}

vector<vector<Player>> createHashtablePlayers(string file, int sizeHash) 
{
    vector<vector<Player>> hashtable(sizeHash);
    ifstream f(file);
    aria::csv::CsvParser parser(f);

    // ignores first line (header)    
    parser.begin();
    for (auto& row : parser)
        insertHashPlayer(row, hashtable, sizeHash);

    f.close();
    return hashtable;
}

vector<vector<User>> createsHashtableUsers(string file, int sizeHash, vector<vector<Player>>& hashtablePlayer) 
{
    vector<vector<User>> hashtable(sizeHash);
    ifstream f(file);
    int sizeHashP = hashtablePlayer.size();
    aria::csv::CsvParser parser(f);

     // ignores first line (header)    
    parser.begin();
    for (auto& row : parser)
        insertHashUser(row, hashtablePlayer, hashtable, sizeHash, sizeHashP);
    
    f.close();
    return hashtable;
}

void insertHashPlayer(vector<string> stats_player, vector<vector<Player>> &hashtable, int sizeHash) 
{
    Player auxPlayer;
    auxPlayer.sofifa_id = stoi(stats_player[0]);
    auxPlayer.count = auxPlayer.rating = 0;
    auxPlayer.shortname = stats_player[1];
    auxPlayer.name = stats_player[2];
    auxPlayer.positions = stats_player[3];
    auxPlayer.nation = stats_player[4];
    auxPlayer.club = stats_player[5];
    auxPlayer.liga = stats_player[6];
    hashtable[auxPlayer.sofifa_id % sizeHash].push_back(auxPlayer); 
}

void insertHashUser(vector<string> review, vector<vector<Player>>& hashtablePlayer,vector<vector<User>> &hashtable, int sizeHash, int sizeHashP)
{
    User auxUser;
    int user_id = stoi(review[0]);
    int player_id = stoi(review[1]);
    float rating = stof(review[2]);
    int key = user_id % sizeHash;
    int index = returnIndexUser(hashtable, sizeHash, user_id);

    if (index == -1) {
        auxUser.user_id = user_id;
        auxUser.players_rating.push_back(pair<int, float>(player_id, rating));
        hashtable[key].push_back(auxUser);
    } else {
        hashtable[key][index].players_rating.push_back(pair<int, float>(player_id, rating));
    }

    // adds rating to the player hashtable
    calcRating(hashtablePlayer, sizeHashP, player_id, rating);
}

int returnIndexUser(vector<vector<User>>& hashtable, int sizeHash, int user_id)
{
    int key = user_id % sizeHash;
    const auto& bucket = hashtable[key];
    int bucket_size=bucket.size();
    for (size_t i = 0; i < bucket_size; ++i) {
        if (bucket[i].user_id == user_id) return i;
    }
    return -1;
}

int returnIndexPlayer(vector<vector<Player>>& hashtable, int sizeHash, int player_id)
{
    int i = 0, key = player_id % sizeHash;
    int tam_coluna=hashtable[key].size();
    if (!hashtable[key].empty()) {
        while ((i+1 < tam_coluna) && hashtable[key][i].sofifa_id != player_id)i++;
    }
    return i;
}

void calcRating(vector<vector<Player>>& hashtable, int sizeHash, int player_id, float rating)
{
    int key = player_id % sizeHash;
    int index=returnIndexPlayer(hashtable, sizeHash, player_id);
    hashtable[key][index].count++;
    hashtable[key][index].rating += rating;
}