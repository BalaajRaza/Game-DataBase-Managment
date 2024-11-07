#ifndef GAMESPAYED_H
#define GAMESPLAYED_H 

#include<iostream>
using namespace std;


class Game_play {
    string Game_ID;
    float ho_play;
    int achive;

public:
    
    Game_play(){}

    Game_play(string GID, float hours, int achievements) {
        Game_ID = GID;
        ho_play = hours;
        achive = achievements;
    }

    Game_play(const Game_play& other) 
        : Game_ID(other.Game_ID), ho_play(other.ho_play), achive(other.achive) {}

    Game_play& operator=(const Game_play& other) {
        if (this != &other) {
            Game_ID = other.Game_ID;
            ho_play = other.ho_play;
            achive = other.achive;
        }
        return *this;
    }
    
    string getGameID() { return Game_ID; }
    float getHoursPlayed() { return ho_play; }
    int getAchievements() { return achive; }
 
// void printGameData() {
//         cout/*<<" - Game ID: "<< Game_ID*/ 
//             <<"Hours Played: " << ho_play 
//             <<", Achievements: " << achive 
//             << endl;
//     }

void printGameData() {
    if (Game_ID.empty()) {
        cout << "Game_ID is empty!" << endl;
    } else {
        cout << "Game ID: " << Game_ID;
    }
    cout << ", Hours Played: " << ho_play << ", Achievements: " << achive << endl;
}
};

#endif