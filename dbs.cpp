#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream> 

#include "players.h"
#include "games.h"

using namespace std;

const int QUEUE_SIZE = 1000;



int randomgen() {
    return rand() % 1000 + 1;
}

struct node {
    playerNode player;
    Games* games;
    int gamesCount;
    node* left;
    node* right;

    node(const playerNode& p) 
    : player(p), games(NULL), gamesCount(0), left(NULL), right(NULL) {}
    
    void initGames(int size) {
        games = new Games[size];
        gamesCount = 0;
    }

    void addGame(const Games& game) {
        games[gamesCount] = game;
        gamesCount = gamesCount + 1;
        //cout<<"Added Game"<<game.Game_ID<<"Counter = "<<gamesCount<<endl;
    }

     node(const node& other) 
        : player(other.player), gamesCount(other.gamesCount), left(NULL), right(NULL) {
        if (other.games) {
            games = new Games[gamesCount];
            for (int i = 0; i < gamesCount; ++i) {
                games[i] = other.games[i];
            }
        } else {
            games = nullptr;
        }
    }

    // Assignment operator
    node& operator=(const node& other) {
        if (this != &other) {
            player = other.player;
            gamesCount = other.gamesCount;

            delete[] games;
            if (other.games) {
                games = new Games[gamesCount];
                for (int i = 0; i < gamesCount; ++i) {
                    games[i] = other.games[i];
                }
            } else {
                games = nullptr;
            }

            left = right = NULL;  // Assignment operator shouldn’t copy left and right pointers.
        }
        return *this;
    }

    // Destructor to release allocated memory
    ~node() {
        delete[] games;
    }
};

struct Queue {
    node* data[QUEUE_SIZE];
    int front;
    int rear;

    Queue() : front(0), rear(0) {}

    bool isEmpty() {
        return front == rear;
    }

    void enqueue(node* n) {
        if (rear < QUEUE_SIZE) {
            data[rear++] = n;
        }
    }

    node* dequeue() {
        if (!isEmpty()) {
            return data[front++];
        }
        return nullptr;
    }
};

// DBS class
class DBS {
    node* root;

    node* insertPlayerHelper(node* currentNode, node* newPlayerNode);

    void collectPlayerData(node* currentNode , pair<string , int> arr[] , int& count){
        if(currentNode == NULL){
            return;
        }
        arr[count++] = make_pair(currentNode->player.getPlayerID(), currentNode->player.getPlayerID().length());
        collectPlayerData(currentNode->left , arr , count);
        collectPlayerData(currentNode->right , arr , count);
    }

    void swap(pair<string , int>& a, pair<string , int>& b) {
        pair<string , int> temp = a;
        a = b;
        b = temp;
    }

    void sortArray(pair<string , int> arr[] , int size){
        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                if (arr[i].second < arr[j].second) {
                    swap(arr[i], arr[j]);
                }
            }
        }
    }

public:
    DBS() : root(NULL) {}

    void readfile_players();
    void insertPlayer(node* newPlayerNode);
    void inorderTraversal() const;
    void inorderTraversalHelper(node* currentNode) const;

    // Queury Functions
    node* searchPlayer(const string& playerID);
    node* searchPlayerHelper(node* currectNode , const string& playerID);

    node* deletePlayer(const string& playerID);
    node* deletePlayerHelper(node* currentNode, const string& playerID);

    void saveData(const string& fileName);
    void saveDataHelper(node* currentNode, ofstream& file);

    void showNLayers(int N);

    void showLayerNumber(const string& playerID);

    void showPath(const string& playerID);

    void editEntry(const string& playerID);

    void showTopNPlayers(int N);

    void hasPlayed(const string& playerID ,const string& gameName);
};

void DBS::readfile_players() {
    
    int skip = (230046%100) * 10 + 100;
    ifstream file("Players.txt");
    if (!file) {
        cout << "Error: Could not open file." << endl;
        return;
    }

    int line_no = 0;
    string line;
    while (getline(file, line)) {
        int random = randomgen();
        if(random < skip){
            continue;
        }

        stringstream ss(line);
        string playerData[5];
        
        // Reading first 5 fields: player data
        for (int i = 0; i < 5; ++i) {
            getline(ss, playerData[i], ',');
        }

        string playerID = playerData[0];
        string name = playerData[1];
        string phone = playerData[2];
        string email = playerData[3];
        string password = playerData[4];
        

        playerNode player(playerID, name, phone, email, password);

        int gameCounter = 0;
        Game_play* gamesPlayed = new Game_play[1000];

        string gameID;
        float hoursPlayed;
        int achievements;

        while (getline(ss, gameID, ',')) {
            ss >> hoursPlayed;
            ss.ignore();
            ss >> achievements;
            ss.ignore();

            //cout<< "Game Id : " <<gameID<<" | Hours Played : "<<hoursPlayed<<" | Acheivements : "<<achievements<<endl;
            gamesPlayed[gameCounter++] = Game_play(gameID, hoursPlayed, achievements);
        }

        player.addGamePlay(gamesPlayed, gameCounter);
        node* playerNode = new node(player);
        playerNode->initGames(gameCounter);


        ifstream gamesFile("Games.txt");
        if (!gamesFile) {
            cout << "Error: Could not open Games.txt." << endl;
            return;
        }

        string gameLine;
        while (getline(gamesFile, gameLine)) {
            stringstream gss(gameLine);
            string gameID, name, developer, publisher;
            float fileSize;
            int downloads;

            getline(gss, gameID, ',');
            getline(gss, name, ',');
            getline(gss, developer, ',');
            getline(gss, publisher, ',');
            gss >> fileSize;
            gss.ignore();
            gss >> downloads;

            bool gamePlayed = false;
            for (int i = 0; i < gameCounter; ++i) {
                if (gamesPlayed[i].getGameID() == gameID) {
                    gamePlayed = true;
                    break;
                }
            }

            if (gamePlayed) {
                //cout << "Found game: " << gameID << " - " << name << endl;
                Games gameInfo(gameID, name, developer, publisher, fileSize, downloads);
                playerNode->addGame(gameInfo);
            }
        }

        gamesFile.close();
        insertPlayer(playerNode);
        delete[] gamesPlayed;
    }

    file.close();
}
// Insert player into the BST
void DBS::insertPlayer(node* newPlayerNode) {
    root = insertPlayerHelper(root,newPlayerNode);
}

node* DBS::insertPlayerHelper(node* currentNode,node* newPlayerNode) {
    if(currentNode == NULL){
        return newPlayerNode;
    }

    if(newPlayerNode->player.getPlayerID() < currentNode->player.getPlayerID()){
        currentNode->left = insertPlayerHelper(currentNode->left, newPlayerNode);
    }
    else{
        currentNode->right = insertPlayerHelper(currentNode->right, newPlayerNode);
    }

    return currentNode;
}

// Inorder traversal of the BST
void DBS::inorderTraversal() const {
    inorderTraversalHelper(root);
}

void DBS::inorderTraversalHelper(node* currentNode) const {
    if (currentNode != NULL) {
        inorderTraversalHelper(currentNode->left);
        currentNode->player.printPlayerData();

        // for (int i = 0; i < currentNode->gamesCount; ++i) {
        //     cout << "Game ID: " << currentNode->games[i].Game_ID << " | "
        //          << "Name: " << currentNode->games[i].Name << " | "
        //          << "Developer: " << currentNode->games[i].Developer << " | "
        //          << "Publisher: " << currentNode->games[i].Publisher << " | "
        //          << "File Size: " << currentNode->games[i].size_GB << " GB | "
        //          << "Downloads: " << currentNode->games[i].download << endl;
        // }
        inorderTraversalHelper(currentNode->right);
    }
}

// Queury Functions;

node* DBS::searchPlayer(const string& playerID){
    return searchPlayerHelper(root, playerID);
}

node* DBS::searchPlayerHelper(node* currentNode, const string& playerID){
    if(currentNode == NULL){
        return NULL;
    }
    if (currentNode->player.getPlayerID() == playerID){
        return currentNode;
    }
    if(playerID < currentNode->player.getPlayerID()){
        return searchPlayerHelper(currentNode->left, playerID);
    }
    else{
        return searchPlayerHelper(currentNode->right, playerID);
    }
}

node* DBS::deletePlayer(const string& playerID){
    root = deletePlayerHelper(root, playerID);
    return root;
}

node* DBS::deletePlayerHelper(node* currentNode , const string& playerID){
    if(currentNode == NULL){
        return NULL;
    }
    if(playerID < currentNode->player.getPlayerID()){
        currentNode->left = deletePlayerHelper(currentNode->left, playerID);
    }
    else if(playerID > currentNode->player.getPlayerID()){
        currentNode->right = deletePlayerHelper(currentNode->right, playerID);
    }
    else{
        if(currentNode->left == NULL){
            node* temp = currentNode->right;
            delete currentNode;
            return temp;
        }
        else if(currentNode->right == NULL){
            node* temp = currentNode->left;
            delete currentNode;
            return temp;
        }
        else{
            node* temp = currentNode->right;
            while(temp->left != NULL){
                temp = temp->left;
            }
            currentNode->player = temp->player;
            currentNode->right = deletePlayerHelper(currentNode->right, temp->player.getPlayerID());
        }
    }
    return currentNode;
}

void DBS::saveData(const string& fileName){
    ofstream file(fileName);
    if (!file.is_open()) {
        cout << "Error opening file for writing." << endl;
        return;
    }
    saveDataHelper(root, file);
    file.close();
}

void DBS::saveDataHelper(node* currentNode, ofstream& file){
    if(currentNode != NULL){
        file << currentNode->player.getPlayerID() << ","
             << currentNode->player.getPlayerName() << ","
             << currentNode->player.getPhoneNum() << ","
             << currentNode->player.getEmail() << ","
             << currentNode->player.getPass() << ",";

    for(int i = 0; i < currentNode->gamesCount; i++){
            file << currentNode->games[i].Game_ID << ","
                 << currentNode->games[i].Name << ","
                 << currentNode->games[i].Developer << ","
                 << currentNode->games[i].Publisher << ","
                 << currentNode->games[i].size_GB << ","
                 << currentNode->games[i].download << ",";
    }
    file << endl;
    file << endl;
    file << endl;
    file << endl;

    saveDataHelper(currentNode->left , file);
    saveDataHelper(currentNode->right, file);

    }
}

void DBS::showNLayers(int n) {
    if(n <= 0){
        cout<<"Invalid Number of Layers!"<<endl;
        return;
    }
    Queue q;
    q.enqueue(root);

    int currrentLayer = 0;
    while(!q.isEmpty() && currrentLayer < n){

        int nodesInCurrentLayer = q.rear - q.front;
        if(nodesInCurrentLayer > 0){
            cout<<"Layer "<<currrentLayer+1<<": ";
        
            for(int i=0;i<nodesInCurrentLayer;i++){

                node* currentNode = q.dequeue();
                cout<<currentNode->player.getPlayerID()<<" ";

                if(currentNode->left != NULL){
                    q.enqueue(currentNode->left);
                }

                if(currentNode->right != NULL){
                    q.enqueue(currentNode->right);
                }
            }
            cout<<endl;
            currrentLayer++;
        }
        
    }
    if(currrentLayer < n){
        cout<<"Not enough layers to show."<<endl;
    }

}

void DBS::showLayerNumber(const string& playerID){
    node* playerNode = searchPlayer(playerID);
    
    if(playerNode == NULL){
        cout<<"Player not found."<<endl;
        return;
    }

    Queue q;
    q.enqueue(root);
    int currentLayer = 0;
    while(!q.isEmpty()){
        int nodesInCurrentLayer = q.rear - q.front;
        currentLayer++;

        for(int i=0;i<nodesInCurrentLayer;i++){
            node* currentNode = q.dequeue();
            if(currentNode->player.getPlayerID() == playerID){
                cout<<"Player "<<playerID<<" is in Layer "<<currentLayer<<endl;
                return;
            }
            if(currentNode->left != NULL){
                q.enqueue(currentNode->left);
            }
            if(currentNode->right != NULL){
                q.enqueue(currentNode->right);
            }
        }
    }
}

void DBS::showPath(const string& playerID){
    node* currentNode = root;
    string path = "";

    while(currentNode!= NULL){
        path += currentNode->player.getPlayerID();

        if(currentNode->player.getPlayerID() == playerID){
            cout<<"Path: "<<path<<endl;
            return;
        }
        else if(playerID < currentNode->player.getPlayerID()){
            path+="->";
            currentNode = currentNode->left;
        }
        else{
            path+="->";
            currentNode = currentNode->right;
        }
    }

    cout<<"Player "<<playerID <<" not found in data.\n";
}

void DBS::editEntry(const string& playerID){
    node* playerNode = searchPlayer(playerID);
    if(playerNode == NULL){
        cout<<"Player not found."<<endl;
        return;
    }
    cout<<"Enter new name: ";
    string newName;
    cin>>newName;
    playerNode->player.setPlayerName(newName);

    cout<<"Enter Player ID: ";
    string newID;
    cin>>newID;
    playerNode->player.setPlayerID(newID);

    cout<<"Enter Player Phone Number: ";
    string newPhoneNum;
    cin>>newPhoneNum;
    playerNode->player.setPhoneNum(newPhoneNum);

    cout<<"Enter Player Email: ";
    string newEmail;
    cin>>newEmail;
    playerNode->player.setEmail(newEmail);

    cout<<"Enter Player Password: ";
    string newPass;
    cin>>newPass;
    playerNode->player.setPass(newPass);

    cout<<"Player "<<playerID<<" updated successfully."<<endl;


    
}

void DBS::showTopNPlayers(int N){
    int max_size = 1000;
    pair<string , int> array[max_size];
    int count = 0;

    collectPlayerData(root , array , count);

    int size = count;
    for(int i=0;i<N && i<size;i++){
        sortArray(array , size);
        cout<<"Player "<<array[i].first<<" has "<<array[i].second<<" games."<<endl;
    }

}

void DBS::hasPlayed(const string& playerID ,const string& gameName){
    node* playerNode = searchPlayer(playerID);
    if(playerNode == NULL){
        cout<<"Player not found."<<endl;
        return;
    }
    for(int i=0;i<playerNode->gamesCount;i++){
        if(playerNode->games[i].Name == gameName){
            cout<<"Player "<<playerID<<" has played "<<gameName<<endl;
            return;
        }
    }
    cout<<"Player "<<playerID<<" has not played "<<gameName<<endl;

}


int main() {

    srand(time(0));
    DBS gameTree;
    gameTree.readfile_players();
    gameTree.inorderTraversal();
    cout<<"Data Loaded.\n";

    int choice;
    do{
        cout<<"\n--QUEURIES--\n";
        cout<<"1. Search A Player\n";
        cout<<"2. Delete Record\n";
        cout<<"3. Save Data\n";
        cout<<"4. Show N Layers\n";
        cout<<"5. Show Layer Number\n";
        cout<<"6. Show Path\n";
        cout<<"7. Edit Entry\n";
        cout<<"8. Top N Players\n";
        cout<<"9. Has Played\n";
        cout<<"0. Exit\n";

        cout<<"Select : ";
        cin>>choice;
        string playerID;

        if(choice == 1){
            cout<<"Enter Player ID: ";
            cin>>playerID;
            node* playerNode = gameTree.searchPlayer(playerID);
            if(playerNode != NULL){
                cout<<"Player Found : \n";
                playerNode->player.printPlayerData();
            }
            else{
                cout<<"Player Not Found\n";
            }
        }

        else if(choice == 2){
            cout<<"Enter Player ID: ";
            cin>>playerID;
            gameTree.deletePlayer(playerID);
            cout<<"Player Deleted\n";
        }

        else if(choice == 3){
            string fileName ;//= "database.csv";
            cout<<"Enter File Name: ";
            cin>>fileName;
            gameTree.saveData(fileName);
            cout<<"Data Saved to "<<fileName<<endl;

        }

        else if(choice == 4){
            int n;
            cout<<"Enter Number of Layers: ";
            cin>>n;
            gameTree.showNLayers(n);
        }

        else if(choice == 5){
            string playerID;
            cout<<"Enter Player ID: ";
            cin>>playerID;
            gameTree.showLayerNumber(playerID);
        }

        else if(choice == 6){
            string playerID;
            cout<<"Enter Player ID: ";
            cin>>playerID;
            gameTree.showPath(playerID);
        }

        else if(choice == 7){
            string playerID;
            cout<<"Enter Player ID: ";
            cin>>playerID;
            gameTree.editEntry(playerID);
        }

        else if(choice == 8){
            int N;
            cout<<"Enter Number of Players: ";
            cin>>N;
            gameTree.showTopNPlayers(N);
        }

        else if(choice == 9){
            string playerID;
            cout<<"Enter Player ID: ";
            cin>>playerID;
            string gameName;
            cout<<"Enter Game Name: ";
            cin>>gameName;
            gameTree.hasPlayed(playerID,gameName);
        }

        else if(choice == 0){
            cout<<"Exiting...\n";
        }

    }while(choice!=0);

    return 0;
}