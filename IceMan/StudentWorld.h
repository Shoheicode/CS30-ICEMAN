#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
        : GameWorld(assetDir)
    {
    }

    /*
    init() method must:

    A. Initialize the data structures used to keep track of your game’s virtual world
    
    B. Construct a new oil field that meets the requirements stated in the section below
        (filled with Ice, Barrels of oil, Boulders, Gold Nuggets, etc.)
    
    C.Allocate and insert a valid Iceman object into the game world at the proper
        location
    */

    virtual int init()
    {
        setGameStatText("HELLO");
        

        for (int i = 0; i <= 64; i++) {
            for (int j = 0; j <= 59; j++) {
                if (!(i <= 33 && i >= 30 && j >= 4)) {
                    characterList.push_back(new Ice(i, j));
                }
            }
        }

        characterList.push_back(new IceMan(30, 60, this));
        characterList.push_back(new Protester(60, 60));


        return GWSTATUS_CONTINUE_GAME;
    }

    /*
    move method() must:

    A: Update the Status Text on the top of the screen

    B: Must ask all the active actors to do something (like move)
        -If an actor does something that makes Iceman give up, move should return GWSTATUS_PLAYER_DIED
        -If Iceman collects all the oil, move should play the sound  SOUND_FINISHED_LEVEL
        and then return a value of GWSTATUS_FINISHED_LEVEL

    C:It must delete any actors that need be removed from the game and the STL container that tracks them
        -A Protester leaving the upper right hand corner
        -A Boulder hitting the ground
        -A Gold Nugget picked up by Iceman/Protester
        -Water Pool that has dried up
        -ETC
    
    */

    virtual int move()
    {
        // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
        // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

        int score = getScore();
        int lives = getLives();
        int level = getLevel();
        //int numHealth = this->IceMan->getHealth();
//        int numWater = IceMan::getSquirt();
//        int numGold = IceMan::getGold();
//        int numOil = IceMan::getOil();
//        int numSonar = IceMan::getSonarCount();
        
        string scoreText = to_string(score);
        string levelText = to_string(level);
        string liveText = to_string(lives);
        
        string healthText = to_string(score);
        string waterText = to_string(score);
        string goldText = to_string(score);
        string oilText = to_string(score);
        string sonarText = to_string(score);
        cout << scoreText << endl;

        string textbox = "Lvl: "+ levelText +" " + " Lives: " +  liveText + " " + "Scr: " + scoreText;
        //string textbox = "Lvl: " + getLvl + " Hlth: " + getHlth() + " Wtr: " + getWater+ + " Gld: " + " Oil: " + " Sonar: "+ " Scr: " + scoreText;
        setGameStatText(textbox);

        for (Actor* a : characterList) {
            a->doSomething();
        }

        if (false) {
            decLives();
        }

        if (getLives() == 0) {
            return GWSTATUS_PLAYER_DIED;
        }

        return GWSTATUS_CONTINUE_GAME;
    }
    /*
    
    This method is called when Iceman:
        -Loses a life
        -Completes the current level

    */
    virtual void cleanUp()
    {
    }
    
    bool blockedByBoulder();

    

private:
    Actor* a;
    list<Actor*> characterList;
    
};

#endif // STUDENTWORLD_H_
