#ifndef GAMES_H
#define GAMES_H

#include<iostream>
using namespace std;

struct Games {
    string Game_ID;
    string Name;
    string Developer;
    string Publisher;
    float size_GB;
    int download;

    Games() : Game_ID(""), Name(""), Developer(""), Publisher(""),  size_GB(0.0), download(0) {}

    Games(string G, string N, string D, string Pu, float FG, int DOwn) {
        Game_ID = G;
        Name = N;
        Developer = D;
        Publisher = Pu;
        size_GB = FG;
        download = DOwn;
    }

    Games& operator=(const Games& other) {
        if (this != &other) {
            Game_ID = other.Game_ID;
            Name = other.Name;
            Developer = other.Developer;
            Publisher = other.Publisher;
            size_GB = other.size_GB;
            download = other.download;
        }
        return *this;
    }

    ~Games() {}

    void printGameData() const {
        cout << "Game ID: " << Game_ID << ", Name: " << Name << ", Developer: " << Developer
            << ", Publisher: " << Publisher << ", File Size: " << size_GB << "GB"
            << ", Downloads: " << download << endl;
    }

    string getGameID() const { return Game_ID; }
};

#endif