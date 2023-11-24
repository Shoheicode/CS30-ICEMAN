#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include<cstdlib>
using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld; //already initialized added bc need to use dataType

class Actor: virtual public GraphObject {
private:
    int m_life;
string imageID;
double size;
protected:
    int hitPoints;
    int xCoord;
    int yCoord;
    StudentWorld* studW;
public:
    Actor(int imageID, int startX, int startY, Direction startDirection): GraphObject(imageID, startX, startY, startDirection)
         {};
    virtual void overlap(StudentWorld* world) {

    }

    //virtual void setVisible(bool shouldIDisplay);
//unsigned int getScore() const;
StudentWorld* getWorld() { return studW; }
    bool outOfField(int x, int y, Direction d);
//void increaseScore(unsigned int howMuch);
//void setGameStatText(string text);
bool getKey(int& value);
//virtual void playSound(int soundID);
    //virtual void moveTo(int x, int y);
    //int getX() const{ return 0;}
    //int getY() const{return 0;}

    virtual void doSomething() = 0;
        virtual ~Actor() {}
  //  …
};
class Protester: virtual public Actor {
protected:
    bool leave_the_oil_field;
    int numSquaresToMoveInCurrentDirection;
    string direction = "left";
public:
    Protester(int startX, int startY) : Actor (IID_PROTESTER, startX, startY, left), GraphObject(IID_PROTESTER, startX, startY, left, 1.0, 0) {//, GraphObject(IID_PROTESTER, startX, startY, left, 1.0, 0)
        numSquaresToMoveInCurrentDirection = 8 + (rand() % 60);
        hitPoints = 5;
        leave_the_oil_field = false;
        setVisible(true);
    };
    void moveTo(int x, int y) {};
    virtual void isAnnoyed(){};
    virtual void isProPickUp(){}; //pickUp gold
    void getNumSquaresToMoveInCurrentDirection(); //Get the number of squares to move in current direction
    virtual void doSomething() override;
    bool overlap(Actor object); // Checks if overlap with specific object
    bool checkDistance(int objectX, int objectY);
    virtual ~Protester() {};
};
class HardcoreProtester: virtual public Protester {
public:
    HardcoreProtester(int startX, int startY) : Protester (startX, startY), Actor (IID_HARD_CORE_PROTESTER, startX, startY, left), GraphObject(IID_HARD_CORE_PROTESTER, startX, startY, left, 1.0, 0) {
        //decide how many numSquaresToMoveInCurrentDirection between 8 and 60
        numSquaresToMoveInCurrentDirection = 8 + (rand() % 60);
        leave_the_oil_field = false;
        hitPoints = 20;
        setVisible(true);
    };
    virtual void doSomething() override;
// virtual void doSomething()
// {
// If I am facing the Iceman and he is next to me, then
// Shout at the Iceman (to annoy him)
// 11
// Else if the Iceman is visible via direct line of sight, then
// Switch direction to face the Iceman
// Move one square in this direction
// Else if I’m about to run into an obstacle, then
// Pick a new direction
// Move one square in this direction
// Else
// Move one square in my current direction
// }
// ...
    virtual ~HardcoreProtester() {};
};

class Prop : virtual public Actor{
private:
    int tickRange;
    
public:
    Prop(int imageID, int startX, int startY, float size, int depth, Direction startDirection)
            : Actor(imageID, startX, startY, startDirection), GraphObject(imageID, startX, startY, startDirection, size, depth) {
                
            }
    bool setVisible(){return true;}
    bool canPickUp() {return true;}
    virtual void doSomething() override;
    bool pickUp(){return true;}
    bool disappear(int numTicks){return true;}
    bool updateStock(Prop* a) {return true;} //update Iceman's prop stock put in iceman?
    int getTicks(){return 0;}
    int decrementTick(){return 0;}
    int setTicks(){return 0;} //prop appears for x number of ticks
    
virtual ~Prop() {}
};

class IceMan : virtual public Actor{
private:
    int damage;
    int waterSq;
    int sC;
    string state;
    int gold;
    double health;
public:
    IceMan(int startX, int startY, StudentWorld* world) : Actor(IID_PLAYER, startX, startY, left), GraphObject(IID_PLAYER, startX, startY, left, 1.0, 0) {
        hitPoints = 10;
        studW = world;
        waterSq = 5;
        sC =1;
        gold = 0;
        damage = 100;
        setVisible(true);
    };
    bool isAlive();
    bool isDead();
    void getAnnoyed(int dAmage);
    int getGold() { return gold;}
    int getSonarCount() { return sC;}
    double getHealth() {return health;}
    int getSquirt() { return waterSq;}
    string getState() { return state; }
    void setState(string a) {a = state;}
    virtual void overlap(StudentWorld* world) override;
    virtual void doSomething() override;
virtual ~IceMan() {}
    
};

class Ice : public GraphObject{
public:
    Ice(int startX, int startY) : GraphObject(IID_ICE, startX, startY, left, 0.25, 3) {
        setVisible(true);
    };
    //virtual void doSomething() override {

    //}; //must be here or else it will be an abstract class
    virtual ~Ice() {}
};

class Squirt : virtual public Prop {
public:
    Squirt(int startX, int startY, double size, int depth)
           : Prop(IID_WATER_SPURT, startX, startY, size, depth, left), Actor(IID_WATER_SPURT, startX, startY, left), GraphObject(IID_WATER_SPURT, startX, startY,left, 1.0 , 1) {
               //set direction to be facing the Iceman
               distance = 4;
               GraphObject::setVisible(true);
       }
    virtual void doSomething() override;
    virtual ~Squirt() {}
private:
   // Direction d = IceMan.Direction;
    int distance;
};

class Oil : virtual public Prop {
public:
    Oil(int startX, int startY, double size, int depth)
           : Prop(IID_BARREL, startX, startY, size, depth, down), Actor(IID_BARREL, startX, startY, down), GraphObject(IID_BARREL, startX, startY,down, 1.0 , 2) {
               GraphObject::setVisible(true);
       }
    
    virtual void doSomething() override;
    virtual ~Oil() {}
};

class Boulder : virtual public Prop {
public:
    //constructor set up stuff in initialization list
    Boulder(int startX, int startY, double size, int depth)
           : Prop(IID_BOULDER, startX, startY, size, depth, down), Actor(IID_BOULDER, startX, startY, down), GraphObject(IID_BOULDER, startX, startY,down, 1.0 , 1) {
               GraphObject::setVisible(true);
       }
    virtual void doSomething() override;
    virtual ~Boulder() {}
    
private:
    //state 0 1 2 falling
};

class Gold : virtual public Prop {
public:
    //constructor set up stuff in initialization list
    Gold(int startX, int startY, double size, int depth)
           : Prop(IID_GOLD, startX, startY, size, depth, down), Actor(IID_GOLD, startX, startY, down), GraphObject(IID_GOLD, startX, startY,down, 1.0 , 2) {
               //if start of game will be hidden in ice {
               GraphObject::setVisible(false);
               //pick-up able by Iceman
               //will remain in permanent state (won't disappear until Iceman picks up}
               
               //else if dropped by IceMan, will appear on screen{
               //GraphObject::setVisible(true);
               //pick-up able by Protestors
               //will remain in temp state (will disappear if Protestor picks up or disappear if they don't pick up}
       }
    virtual void doSomething() override;
    virtual ~Gold() {}
};

class SonarKit : virtual public Prop {
public:
    SonarKit(int startX, int startY, double size, int depth)
           : Prop(IID_SONAR, startX, startY, size, depth, right), Actor(IID_SONAR, startX, startY, right), GraphObject(IID_SONAR, startX, startY,right, 1.0 , 2) {
               GraphObject::setVisible(true);
               //pick-up able Iceman
               //will be in temp state (limited num of ticks b4 disappearing
               // numTicks will exist T = max(100, 300 – 10*current_level_number)
       }
    virtual void doSomething() override;
    virtual ~SonarKit() {}
};

class WaterPool : virtual public Prop {
public:
    WaterPool(int startX, int startY, double size, int depth)
           : Prop(IID_WATER_POOL, startX, startY, size, depth, right), Actor(IID_WATER_POOL, startX, startY, right), GraphObject(IID_WATER_POOL, startX, startY,right, 1.0 , 2) {
               GraphObject::setVisible(true);
               //pick-up able Iceman
               //will be in temp state (limited num of ticks b4 disappearing
               // numTicks will exist T = max(100, 300 – 10*current_level_number)
       }
    virtual void doSomething() override;
    virtual ~WaterPool() {}
};

#endif ////ACTOR_H_
