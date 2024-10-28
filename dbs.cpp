#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
using namespace std;

int randomgen() {
    return rand() % 1000 + 1;
}

// Gamenode Class Definition
class Gamenode {
    string Game_ID;
    string Name;
    string Developer;
    string Publisher;
    float F_size_GB;
    int download;

public:
    Gamenode();
    Gamenode(string, string, string, string, float, int);
    void set_val(string, string, string, string, float, int);
    string getGameID() const { return Game_ID; }
    void printGameData() const;


    Gamenode() : Game_ID(""), Name(""), Developer(""), Publisher(""), F_size_GB(0.0), download(0) {}

    Gamenode(string G, string N, string D, string Pu, float FG, int DOwn) {
        Game_ID = G;
        Name = N;
        Developer = D;
        Publisher = Pu;
        F_size_GB = FG;
        download = DOwn;
    }

    void printGameData() const {
        cout << "Game ID: " << Game_ID << ", Name: " << Name << ", Developer: " << Developer
            << ", Publisher: " << Publisher << ", File Size: " << F_size_GB << "GB"
            << ", Downloads: " << download << endl;
    }
};


// Game_play Class Definition
class Game_play {
    string Game_ID;
    float ho_play;
    int achive;

public:
    Game_play();
    Game_play(string, float, int);
    void set_val(string, float, int);
    string getGameID() { return Game_ID; }
    float getHoursPlayed() { return ho_play; }
    int getAchievements() { return achive; }
    
    Game_play() : Game_ID(""), ho_play(0.0), achive(0) {}

    Game_play(string GID, float hours, int achievements) {
        Game_ID = GID;
        ho_play = hours;
        achive = achievements;
    }

    void set_val(string GID, float hours, int achievements) {
        Game_ID = GID;
        ho_play = hours;
        achive = achievements;
    }


};

// playerNode Class Definition
class playerNode {
    string Player_ID;
    string Name;
    string phone_num;
    string email;
    string pass;
    Game_play* gamesPlayed;  
    int gameCount;           

public:
    playerNode();
    playerNode(string, string, string, string, string);
    ~playerNode();  
    void addGamePlay(Game_play*, int); 
    void printPlayerData(); 
    string getPlayerID() const { return Player_ID; }
};

// Constructor implementations for playerNode
playerNode::playerNode() : Player_ID(""), Name(""), phone_num(""), email(""), pass(""), gamesPlayed(nullptr), gameCount(0) {}

playerNode::playerNode(string PI, string N, string pho, string em, string pw)
    : Player_ID(PI), Name(N), phone_num(pho), email(em), pass(pw), gamesPlayed(nullptr), gameCount(0) {}


playerNode::~playerNode() {
    delete[] gamesPlayed;
}

void playerNode::addGamePlay(Game_play* gameArray, int count) {
    gameCount = count;
    gamesPlayed = new Game_play[gameCount];
    for (int i = 0; i < gameCount; ++i) {
        gamesPlayed[i] = gameArray[i]; 
    }
}

void playerNode::printPlayerData() {
    cout << "Player ID: " << Player_ID << endl;
    cout << "Name: " << Name << endl;
    cout << "Phone Number: " << phone_num << endl;
    cout << "Email: " << email << endl;
    cout << "Password: " << pass << endl;
    cout << "Games Played:" << endl;
    for (int i = 0; i < gameCount; ++i) {
        cout << " - Game ID: " << gamesPlayed[i].getGameID()
             << ", Hours Played: " << gamesPlayed[i].getHoursPlayed()
             << ", Achievements: " << gamesPlayed[i].getAchievements() << endl;
    }
}

// tree Class Definition
class tree {
    struct node {
        playerNode play;
        Gamenode game; 
        node *left, *right;

        node(const playerNode &p, const Gamenode &g) : play(p), game(g), left(nullptr), right(nullptr) {}
    };

    node* root;

    node* insertPlayerHelper(node* currentNode, const playerNode& newPlayer);
    node* insertGameHelper(node* currentNode, const Gamenode& newGame);
    Gamenode* searchGameByIDHelper(node* currentNode, const string& gameID); // Change return type here

public:
    tree() : root(nullptr) {}

    void readfile_players();
    void read_game();

    void insertPlayer(const playerNode& newPlayer);
    void game_insert(const Gamenode& newGame);
    
    Gamenode* searchGameByID(const string& gameID);

    void inorderTraversal() const;
    void inorderTraversalHelper(node* currentNode) const;
};

void tree::readfile_players() {
    ifstream file("Players.txt");
    int seed = 230046;
    int skip = (seed % 230046) * 10 + 100;

    if (!file) {
        cout << "Error: Could not open file." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (skip < randomgen()) {
            continue;
        }
        size_t pos = 0;
        string token;

        string playerData[5];
        int index = 0;
        while ((pos = line.find(',')) != string::npos && index < 5) {
            playerData[index++] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }

        string Player_ID = playerData[0];
        string Name = playerData[1];
        string phone_num = playerData[2];
        string email = playerData[3];
        string pass = playerData[4];

        playerNode player(Player_ID, Name, phone_num, email, pass);

        int gameCounter = 0;
        Game_play* tempGames = new Game_play[200];

        while (!line.empty()) {
            if ((pos = line.find(',')) == string::npos) break;
            string Game_ID = line.substr(0, pos);
            line.erase(0, pos + 1);

            if ((pos = line.find(',')) == string::npos) break;
            float hours_played = stof(line.substr(0, pos));
            line.erase(0, pos + 1);

            if ((pos = line.find(',')) == string::npos) break;
            int achievements = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            tempGames[gameCounter++] = Game_play(Game_ID, hours_played, achievements);
        }

        player.addGamePlay(tempGames, gameCounter);

        insertPlayer(player);

        delete[] tempGames;
    }

    file.close();
}

void tree::insertPlayer(const playerNode& newPlayer) {
    root = insertPlayerHelper(root, newPlayer);
}

tree::node* tree::insertPlayerHelper(node* currentNode, const playerNode& newPlayer) {
    if (currentNode == nullptr) {
        Gamenode defaultGame;
        return new node(newPlayer, defaultGame);
    }

    if (newPlayer.getPlayerID() < currentNode->play.getPlayerID()) {
        currentNode->left = insertPlayerHelper(currentNode->left, newPlayer);
    } else {
        currentNode->right = insertPlayerHelper(currentNode->right, newPlayer);
    }

    return currentNode;
}

void tree::game_insert(const Gamenode& newGame) {
    root = insertGameHelper(root, newGame);
}

tree::node* tree::insertGameHelper(node* currentNode, const Gamenode& newGame) {
    if (currentNode == nullptr) {
        playerNode defaultPlayer; 
        return new node(defaultPlayer, newGame);
    }

    if (newGame.getGameID() < currentNode->game.getGameID()) {
        currentNode->left = insertGameHelper(currentNode->left, newGame);
    } else {
        currentNode->right = insertGameHelper(currentNode->right, newGame);
    }

    return currentNode;
}

Gamenode* tree::searchGameByID(const string& gameID) {
    return searchGameByIDHelper(root, gameID);
}

Gamenode* tree::searchGameByIDHelper(node* currentNode, const string& gameID) {
    if (currentNode == nullptr) {
        return nullptr;
    }
    if (currentNode->game.getGameID() == gameID) {
        return &currentNode->game; 
    }
    if (gameID < currentNode->game.getGameID()) {
        return searchGameByIDHelper(currentNode->left, gameID); 
    } else {
        return searchGameByIDHelper(currentNode->right, gameID); 
    }
}

void tree::inorderTraversal() const {
    inorderTraversalHelper(root);
}

void tree::inorderTraversalHelper(node* currentNode) const {
    if (currentNode != nullptr) {
        inorderTraversalHelper(currentNode->left);
        cout << "Player ID: " << currentNode->play.getPlayerID() << ", Game ID: " << currentNode->game.getGameID() << endl;
        inorderTraversalHelper(currentNode->right);
    }
}

int main() {

    tree gameTree;
    gameTree.readfile_players();

    gameTree.game_insert(Gamenode("G01", "Game 1", "Dev A", "Pub A", 20.0, 100));
    gameTree.game_insert(Gamenode("G02", "Game 2", "Dev B", "Pub B", 15.0, 200));


    Gamenode* foundGame = gameTree.searchGameByID("G01");
    if (foundGame) {
        foundGame->printGameData();
    } else {
        cout << "Game not found!" << endl;
    }

    gameTree.inorderTraversal();

    return 0;
}