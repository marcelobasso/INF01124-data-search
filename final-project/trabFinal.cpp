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

void printHelp() {
    std::cout << "\nUsage\n  [option] <arguments>\n\n";
    std::cout << "Options:\n  player <prefix>\n    Search player by prefix\n\n";
    std::cout << "  user <user_id>\n    Search user by ID\n\n";
    std::cout << "  top <N> <position>\n    N top players\n\n";
    std::cout << "  tags <list of tags>\n    Search players that have this tags\n\n";
    std::cout << "  q - Quit program" << endl;
}

// for string delimiter
std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

bool runQuery(const string query, vector<vector<Player>>& hashtableP, vector<vector<User>>& hashtableU, Trie* names, Trie* tags, int sizeHash) {
    string search = "", input;
    vector<int> playerNames;
    Player auxPlayer;
    vector<pair<Player, float>> players_list;
    int user_id, user_key, user_index, player_id, player_key, player_index;
    vector<string> queryS;

    queryS = split(query, " ");

    if (queryS[0] == "player") {
        // player prefix search
        playerNames = searchTrie(names, queryS[1], 0);

        for (int i = 0; i < playerNames.size(); i++) {
            auxPlayer = searchHash(playerNames[i], hashtableP);
            cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << auxPlayer.name;
            cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << auxPlayer.rating / auxPlayer.count << endl;
        }
    } else if (queryS[0] == "user") {
        // userId search
        user_id = stoi(queryS[1]);
        user_key = user_id % sizeHash;
        user_index = returnIndexUser(hashtableU, sizeHash, user_id);

        if (user_index == -1) {
            cout << "User does not exist!\n\n";
            return 1;
        } else {
            // find players
            for (pair<int, float>& rating : hashtableU[user_key][user_index].players_rating) {
                player_id = rating.first;
                player_index = returnIndexPlayer(hashtableP, sizeHash, player_id);
                // players_list.push_back(make_pair(hashtableP[player_key][player_index], rating.second));
            }

            // order players

            // prints players
            // for (auto& el : players_list) {
            //     cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << el.first.sofifa_id;
            //     cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << el.first.shortname;
            //     cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << el.first.name;
            //     cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << el.first.rating / el.first.count;
            //     cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << el.first.count;
            //     cout << left << setw(NUM_WIDTH) << setfill(SEPARATOR) << el.second;
            // }
        }
    } else if (queryS[0] == "top") {

    } else if (queryS[0] == "tags") {
        
    } else if (queryS[0] != "q") {
        cout << "Invalid query!\n" << endl;
    }

    return 1;
}

