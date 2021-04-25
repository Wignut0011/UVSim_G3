#ifndef MENUSTRINGS_H
#define MENUSTRINGS_H
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

///Dane Manley (Including the .txt files)

enum :size_t {FILES = 16};

class MenuStrings{
    //width 120, height 30.
    vector<string> pages;
    string pageNames[FILES] = {"MainMenu.txt","Readme1.txt","Readme2.txt","Readme3.txt","Readme4.txt","Readme5.txt",
                               "Readme6.txt","EditMode.txt","ExecuteMode.txt","Save.txt","Load.txt","Copy.txt",
                               "Cut.txt","Paste.txt","Insert.txt","Delete.txt"};
public:
    MenuStrings(){
        setupMenus();
    };
    string getMenu(int pageNum){
        return pages[pageNum];
    }
private:
    void setupMenus(){
        ifstream file;
        for (int i = 0; i < FILES; i++){
            string line;
            stringstream text;
            string test = pageNames[i];
            file.open("Pages/" + string(pageNames[i]), ios::in);
            if (file){
                while( getline(file,line)){
                    text << line << "\n";
                }
                file.close();
            }
            else throw runtime_error("Missing menu .txt file "+pageNames[i]);
            pages.emplace_back(text.str());
        }
    }
};
#endif