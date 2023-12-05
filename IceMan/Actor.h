#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include<cstdlib>
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

class Protester: virtual public Actor {
protected:
    bool leave_the_oil_field;
    bool hasShoutedLast15;
    int moves;
    int ticksToWait;
    int numSquaresToMoveInCurrentDirection;
    string direction = "left";
public:
    Protester(int startX, int startY, StudentWorld* world) : Actor (IID_PROTESTER, startX, startY, left,world, 1.0, 0) {
        numSquaresToMoveInCurrentDirection = 8 + (rand() % 53);
        hitPoints = 5;//set data members numbers specified by packet
        
        //moves = ((getWorld()->getLevel())/4);
        ticksToWait = max(0,moves);
        leave_the_oil_field = false;//doesn't leave field bc is Alive
        hasShoutedLast15 = false;
        setVisible(true);//appear on screen
    };
    //void moveTo(int x, int y) {};
    virtual void isAnnoyed(){};
    virtual void isBribed(); //pickUp gold
    void getNumSquaresToMoveInCurrentDirection(); //Get the number of squares to move in current direction
    virtual bool blockedByIceOrBoulder(int x, int y, StudentWorld* world);
    virtual bool iceManisInSight(int x, int y, StudentWorld* world);
    virtual void doSomething() override;
    bool overlap(Actor object); // Checks if overlap with specific object
    bool checkDistance(int objectX, int objectY);
    virtual ~Protester() {};
};
class HardcoreProtester: virtual public Protester {
public:
    //Hardcore Protestor -> Protestor -> Actor -> GraphObject
    HardcoreProtester(int startX, int startY, StudentWorld* world) : Protester (startX, startY, world), Actor (IID_HARD_CORE_PROTESTER, startX, startY, left, world) {
        //decide how many numSquaresToMoveInCurrentDirection between 8 and 60
        numSquaresToMoveInCurrentDirection = 8 + (rand() % 60);
        leave_the_oil_field = false;//doesn't leave field bc is Alive
        hitPoints = 20;//set data members numbers specified by packet
        setVisible(true);//appear on screen
    };
    virtual void doSomething() override;
    virtual ~HardcoreProtester() {};
private:
    int ticksToWaitBetweenMoves;
};

class Prop : virtual public Actor{
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
    void getAnnoyed(int dAmage);
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
    //virtual void doSomething() override {
    //}; //must be here or else it will be an abstract class
    virtual ~Ice() {}
};

class Squirt : virtual public Prop {
public:
    Squirt(int startX, int startY, double size, int depth, StudentWorld* world)
           : Prop(IID_WATER_SPURT, startX, startY, size, depth, left, world), Actor(IID_WATER_SPURT, startX, startY, left, world, 1.0, 1) {
               setFacingIceMan(getDirection(), studW);
               distance = 4;
               setVisible(true);//appear on screen
       }
    virtual void doSomething() override;
    virtual ~Squirt() {}
private:
    int distance;
};

class Oil : virtual public Prop {
public:
    Oil(int startX, int startY, StudentWorld* world)
           : Prop(IID_BARREL, startX, startY, 1.0, 2, right, world), Actor(IID_BARREL, startX, startY, right, world, 1.0, 2) {
               setVisible(true);//appear on screen
       }
    virtual void doSomething() override;
    virtual ~Oil() {}
};

class Boulder : virtual public Prop {
public:
    //constructor set up stuff in initialization list
    Boulder(int startX, int startY, StudentWorld* world)
           : Prop(IID_BOULDER, startX, startY, 1.0, 1, down, world), Actor(IID_BOULDER, startX, startY, down,world, 1.0, 1) {
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

class Gold : virtual public Prop {
public:
    //constructor set up stuff in initialization list
    Gold(int startX, int startY, bool isDropped, StudentWorld* world)
           : Prop(IID_GOLD, startX, startY, 1.0, 2, right, world), Actor(IID_GOLD, startX, startY, right, world, 1.0, 2) {
               if (isDropped == false){
                   setVisible(true);//hidden in ice CHANGE ONCE FINISHED
                   currentState = icePickUp;
                   //pick-up able by Iceman
                   //wont disappear
               }
               
               if (isDropped == true){//not finished
                   currentState = waiting;
                   cout << wait << endl;
                       while (wait != 0){
                           setVisible(true);//appears on screen
                           currentState = proPickUp;
                           wait --;
                           cout << wait << endl;
                           
                       }
                   if (wait == 0){
                       setAlive(false);
                   }
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

class SonarKit : virtual public Prop {
public:
    //1.0 and 2 and right
    SonarKit(int startX, int startY, bool canAppear, StudentWorld* world)
           : Prop(IID_SONAR, startX, startY, 1.0, 2, right, world), Actor(IID_SONAR, startX, startY, right, world, 1.0, 2) {
               if (canAppear== false){
                   //tick span loop
                   //canAppear = true;
               }
               if (canAppear == true){
                   //tick span loop
                    setVisible(true);//appear on screen
                   //end loop
                   //setVisible(false);//disappear
               }
               //pick-up able Iceman
               //will be in temp state (limited num of ticks b4 disappearing
               // numTicks will exist T = max(100, 300 – 10*current_level_number)
       }
    virtual void doSomething() override;
    virtual ~SonarKit() {}
private:
    int ticksToWait;
};

class WaterPool : virtual public Prop {
public:
    WaterPool(int startX, int startY, double size, int depth, StudentWorld* world)
           : Prop(IID_WATER_POOL, startX, startY, size, depth, right, world), Actor(IID_WATER_POOL, startX, startY, right, world, 1.0, 2) {
               GraphObject::setVisible(true);//appear on screen
               //pick-up able Iceman
               //will be in temp state (limited num of ticks b4 disappearing
               // numTicks will exist T = max(100, 300 – 10*current_level_number)
       }
    virtual void doSomething() override;
    virtual ~WaterPool() {}
};

#endif ////ACTOR_H_
