#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include<cstdlib>
#include <cmath>
#include <queue>
using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld; //already initialized added bc need to use dataType

class Actor: public GraphObject {
private:
    int m_life;
    string imageID;
    double size;
protected:
    int hitPoints;
    int xCoord;
    int yCoord;
    bool amIAlive;
 
    StudentWorld* studW;
public:
    Actor(int imageID, int startX, int startY, Direction startDirection, StudentWorld* world, double size = 1.0, int depth = 0):
        GraphObject(imageID, startX, startY, startDirection, size, depth){
        amIAlive = true;//when spawned object, it becomes alive
        studW = world;//set up studW to be correct world
    };
    virtual void overlap(StudentWorld* world) {
    }
    StudentWorld* getWorld() { return studW; }
    bool outOfField(int x, int y, Direction d);
    string is4Away(StudentWorld* world);
    string is3Away(StudentWorld* world);
    bool isFacingIceMan(Direction d, StudentWorld* world);
    void setFacingIceMan(Direction d,StudentWorld* world);
    int getHitpoints(){return hitPoints;}
    virtual void setHitpoints(int a){hitPoints += a;}
    bool isAlive() { return amIAlive; };
    void setAlive(bool alive) { amIAlive = alive; };
    virtual void doSomething() = 0;
    virtual ~Actor() {}
  //  …
};

class AnnoyedActor : public Actor {
    public:
        AnnoyedActor(int imageID, int startX, int startY, Direction startDirection, StudentWorld* world, double size = 1.0, int depth = 0) :
            Actor(imageID, startX, startY, startDirection, world, size, depth) {

        }

    protected:
    virtual void isAnnoyed() =0;
};

class Protester: public AnnoyedActor {
protected:
    bool leave_the_oil_field;
    int shoutLast15;
    int moves;
    int tickStun;
    int ticksToWait;
    int numSquaresToMoveInCurrentDirection;
    bool stun;
    string direction = "left";

public:
    Protester(int startX, int startY, int imageID, int tStun, StudentWorld* world) : AnnoyedActor (imageID, startX, startY, left,world, 1.0, 0) {
        numSquaresToMoveInCurrentDirection = 8 + (rand() % 53);
        hitPoints = 5;//set data members numbers specified by packet
        shoutLast15 = 15;
        stun = false;
        tickStun = tStun;
        ticksToWait = 10;
        leave_the_oil_field = false;//doesn't leave field bc is Alive
        setVisible(true);//appear on screen
        
    };
    virtual void isAnnoyed();
    virtual bool setHit(int a) {hitPoints += 2; stun = true; return stun;}
    void virtual tryGold(int x, int y);
    virtual bool yell(int x, int y);
    virtual void det15(){shoutLast15--;}
    virtual int getStun(){return tickStun;}
    virtual void detStun(){tickStun--;}
    virtual void reset15(){shoutLast15 = 15;}
    void getNumSquaresToMoveInCurrentDirection(); //Get the number of squares to move in current direction
    virtual bool iceManisInSight(int x, int y, StudentWorld* world);
    virtual bool isAtFork(int x, int y, StudentWorld* world);
    virtual void doSomething() override;
    bool overlap(Actor object); // Checks if overlap with specific object
    virtual void moveOne(int x, int y, Direction d);
    virtual ~Protester() {};
};
class HardcoreProtester: public Protester {
public:
    //Hardcore Protestor -> Protestor -> Actor -> GraphObject
    HardcoreProtester(int startX, int startY, int ticks_to_stare, StudentWorld* world) : Protester (startX, startY, IID_HARD_CORE_PROTESTER, ticks_to_stare, world) {
        //decide how many numSquaresToMoveInCurrentDirection between 8 and 60
        numSquaresToMoveInCurrentDirection = 8 + (rand() % 60);
        //ticks_to_stare = max(50, 100 – current_level_number * 10);
        goldInv = 0;
        leave_the_oil_field = false;//doesn't leave field bc is Alive
        hitPoints = 20;//set data members numbers specified by packet
        setVisible(true);//appear on screen
    };
    virtual void doSomething() override;
    virtual void tryGold(int x, int y) override;
    void setGoldInv(int a){goldInv += a;}
    int getGold(){return goldInv;}
    int getTickStare() {return ticks_to_stare;}
    virtual ~HardcoreProtester() {};
private:
    int ticksToWaitBetweenMoves;
    int ticks_to_stare;
    int goldInv;
    
};

class Prop : public Actor{
private:
    int tickRange;
    
public:
    Prop(int imageID, int startX, int startY, float size, int depth, Direction startDirection, StudentWorld* world)
            : Actor(imageID, startX, startY, startDirection, world) {
                
            }
    bool canPickUp() {return true;}
    virtual void doSomething() = 0;
    bool pickUp(){return true;}
    bool disappear(int numTicks){return true;}
    int getTicks(){return 0;}
    
virtual ~Prop() {}
};

class IceMan : public Actor{
private:
    int damage;
    int waterSq;
    int sonarC;
    int oil;
    int gold;
public:
    IceMan(int startX, int startY, StudentWorld* world) : Actor(IID_PLAYER, startX, startY, left, world, 1.0, 0) {
        hitPoints = 10;
        oil = 0;
        studW = world;
        waterSq = 5;
        sonarC =1;
        gold = 0;
        damage = 100; //set data members numbers specified by packet
        setVisible(true);//appear on screen
        //reset score if lost life!
    };
    void isInRange(StudentWorld* world);
    void dropGold(StudentWorld* world);
    void checkAnnoyed();
    int getGold() { return gold;}
    int getOil() {return oil;}
    int getSonarCount() { return sonarC;}
    double getHealth() {return hitPoints;}
    int getSquirt() { return waterSq;}
    void setWater(int a){waterSq += a;}
    void setOil(int a){oil += a;}
    void setGold(int a){gold += a;}
    void setSonar(int a){sonarC += a;}
    virtual void overlap(StudentWorld* world) override;
    virtual void doSomething() override;
    virtual ~IceMan() {}
};

class Ice : public GraphObject{
public:
    Ice(int startX, int startY) : GraphObject(IID_ICE, startX, startY, left, 0.25, 3) {
        setVisible(true);//appear on screen
    };
    virtual bool overlap(StudentWorld* world);
    //virtual void doSomething() override {
    //}; //must be here or else it will be an abstract class
    virtual ~Ice() {}
};

class Squirt : public Prop {
public:
    Squirt(int startX, int startY, Direction d, StudentWorld* world)
           : Prop(IID_WATER_SPURT, startX, startY, 1.0, 1, d, world) {
               if(!isFacingIceMan(getDirection(), world)){
                   setFacingIceMan(getDirection(), studW);
               }
               distance = 4;
               setVisible(true);//appear on screen
       }
    virtual void doSomething() override;
    void deT(){ticksToWait--;}
    void detD(){distance--;}
    virtual ~Squirt() {}
private:
    int distance;
    int ticksToWait;
};

class Oil : public Prop {
public:
    Oil(int startX, int startY, StudentWorld* world)
           : Prop(IID_BARREL, startX, startY, 1.0, 2, right, world) {
               setVisible(true);//appear on screen
       }
    virtual void doSomething() override;
    virtual ~Oil() {}
};

class Boulder : public Prop {
public:
    //constructor set up stuff in initialization list
    Boulder(int startX, int startY, StudentWorld* world)
           : Prop(IID_BOULDER, startX, startY, 1.0, 1, down, world) {
               setVisible(true);//appear on screen
               currentState = stable; //set to beginning state
               amIAlive = true; //boulder is alive
       }
    virtual void doSomething() override;
    virtual void overlap(StudentWorld* world) override;
    virtual ~Boulder() {}
    
private:
    //state 0 1 2 falling
    enum state {
        stable,
        waiting,
        falling
    };

    int wait = 30;

    state getState() { return currentState; }

    state currentState;
};

class Gold : public Prop {
public:
    //constructor set up stuff in initialization list
    Gold(int startX, int startY, bool isDropped, StudentWorld* world)
        : Prop(IID_GOLD, startX, startY, 1.0, 2, right, world) {
        if (isDropped == false) {
            setVisible(true);//hidden in ice CHANGE ONCE FINISHED
            currentState = icePickUp;
            //pick-up able by Iceman
            //wont disappear
        }

        if (isDropped == true) {//not finished
            setState(proPickUp);
            setVisible(true);
        }
    }
               //will remain in temp state (will disappear if Protestor picks up or disappear if they don't pick up}
    bool isPickUpAble(Actor IceMan,StudentWorld* world);
    
    //void iceManDropped(){currentState = isDropped; }; //check in doSomething
    virtual void doSomething() override;
    virtual ~Gold() {}
private:
    enum state {
        waiting,
        icePickUp,
        proPickUp
    };

    state getState() { return currentState; }
    void setState(enum state s) { currentState = s; }
    int wait = 50;
    state currentState;
};

class SonarKit : public Prop {
public:
    //1.0 and 2 and right
    SonarKit(int startX, int startY, int ticks, StudentWorld* world)
           : Prop(IID_SONAR, startX, startY, 1.0, 2, right, world) {
               ticksToWait = ticks;
               setVisible(true);
       }
    virtual void doSomething() override;
    void deT(){ticksToWait--;}
    virtual ~SonarKit() {}
private:
    int ticksToWait;
};

class WaterPool : public Prop {
public:
    WaterPool(int startX, int startY, int tixWait, StudentWorld* world)
           : Prop(IID_WATER_POOL, startX, startY, 1.0, 2, right, world) {
//               for (Actor* a : world->getCharacterList()){
//                   if (a->getID()== IID_ICE && a->getX() == startX && a->getY() == startY){
//                       setAlive(false);
//                       return;
//                   }
//               }
               ticksToWait = tixWait;
               setVisible(true);
       }
    virtual void doSomething() override;
    void deT(){ticksToWait--;}
    bool tickEllapsed(int tixWait);
    virtual ~WaterPool() {}
private:
    int ticksToWait;
    int saveTix;
};

#endif ////ACTOR_H_
