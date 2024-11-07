#ifndef PLAYER_H
#define PLAYER_H

#include<iostream>
#include "gamesplayed.h"
using namespace std;

class playerNode {
    string Player_ID;
    string Name;
    string phone_num;
    string email;
    string pass;
    Game_play* gamesPlayed;  
    int gameCount;           

public:

playerNode() : Player_ID(""), Name(""), phone_num(""), email(""), pass(""), gamesPlayed(nullptr), gameCount(0) {}

playerNode(string PI, string N, string pho, string em, string pw)
    : Player_ID(PI), Name(N), phone_num(pho), email(em), pass(pw), gamesPlayed(nullptr), gameCount(0) {}

playerNode(const playerNode& other) 
        : Player_ID(other.Player_ID), Name(other.Name), phone_num(other.phone_num),
          email(other.email), pass(other.pass), gameCount(other.gameCount) {
        if (other.gamesPlayed) {
            gamesPlayed = new Game_play[gameCount];
            for (int i = 0; i < gameCount; ++i) {
                gamesPlayed[i] = other.gamesPlayed[i];
            }
        } else {
            gamesPlayed = nullptr;
        }
    }

    playerNode& operator=(const playerNode& other) {
        if (this != &other) {
            Player_ID = other.Player_ID;
            Name = other.Name;
            phone_num = other.phone_num;
            email = other.email;
            pass = other.pass;

            delete[] gamesPlayed;
            gameCount = other.gameCount;
            if (other.gamesPlayed) {
                gamesPlayed = new Game_play[gameCount];
                for (int i = 0; i < gameCount; ++i) {
                    gamesPlayed[i] = other.gamesPlayed[i];
                }
            } else {
                gamesPlayed = nullptr;
            }
        }
        return *this;
    }

~playerNode() {
    delete[] gamesPlayed;
}

void addGamePlay(Game_play* gameArray, int count) {
    gameCount = count;
    gamesPlayed = new Game_play[gameCount];
    for (int i = 0; i < gameCount; ++i) {
        gamesPlayed[i] = gameArray[i];
        //gamesPlayed[i].printGameData();
    }
}

void displayGamesPlayed(){
    for (int i = 0; i < 5; ++i) {
        cout<<"Game "<< i+1 <<" : ";
        gamesPlayed[i].printGameData();
    }
}

void printPlayerData() {
    cout<<"---------------------------------------\n";
    cout << "Player ID: " << Player_ID << endl;
    cout << "Name: " << Name << endl;
    cout << "Phone Number: " << phone_num << endl;
    cout << "Email: " << email << endl;
    cout << "Password: " << pass << endl;
    //displayGamesPlayed();
    cout<<"---------------------------------------\n";
}
    string getPlayerID() const { return Player_ID; }
    string getPlayerName() const {return Name;}
    string getPhoneNum() const { return phone_num; }
    string getEmail() const { return email; }
    string getPass() const { return pass; }

    void setPlayerID(string PI) { Player_ID = PI; }
    void setPlayerName(string N) { Name = N; }
    void setPhoneNum(string pho) { phone_num = pho; }
    void setEmail(string em) { email = em; }
    void setPass(string pw) { pass = pw; }

    int getGameCount() const { return gameCount; }
};

#endif