#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: virtual public GraphObject {
protected:
int imageID;
int xCoord;
int yCoord;
//amount of ticks?
public:
    Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) :
        GraphObject(imageID, startX, startY, left, 1.0, depth) {};
    //void setVisible(bool shouldIDisplay);
    //unsigned int getScore() const;
    unsigned int getLevel() const;
    void increaseScore(unsigned int howMuch);
    void setGameStatText(string text);
    bool getKey(int& value);
//void playSound(int soundID);
    void moveTo(int x, int y);
    int getX() const{ return 0;}
    int getY() const{return 0;}

        virtual void doSomething(){ //set to 0? so that every derived object can do a different //thing when asked to do something
            //
            //    for each actor on the level: // range or for loop
            //    if (the actor is still alive)//bool func, func object?
            //    tell the actor to doSomething();
            //    }
        }
    
        virtual ~Actor() {}
  //  …
};
class Protester: virtual public Actor {
protected:
    int hitPoints;
    bool leave_the_oil_field;
    int numSquaresToMoveInCurrentDirection;
    string direction = "left";
public:
//setVisible(bool shouldIDisplay);
    void moveTo(int x, int y) {};
    void getNumSquaresToMoveInCurrentDirection(); //Get the number of squares to move in current direction
    bool overlap(Actor object); // Checks if overlap with specific object
    bool checkDistance(int objectX, int objectY);
    virtual ~Protester() {};
};
class HardcoreProtester: virtual public Protester {
public:
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
    Prop(int imageID, int startX, int startY, float size); //student world
    bool setVisible(){return true;}
    bool canPickUp() {return true;}
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
	int lives;
public:
    IceMan(int startX, int startY) : Actor(IID_PLAYER, startX, startY, left), GraphObject(IID_PLAYER, startX, startY, left) {
        setVisible(true);
    };
	virtual bool isAlive(int lives){
    	if (lives > 0){
        	return true;
    	}
    	else{
        	return false;
    	}
	}
//    	void Iceman::doSomething()
//    	{
//    	...
//    	int ch;
//    	if (getWorld()->getKey(ch) == true)
//    	{
//    	// user hit a key this tick!
//    	switch (ch)
//    	{
//    	case KEY_PRESS_LEFT:
//    	... move player to the left ...;
//    	break;
//    	case KEY_PRESS_RIGHT:
//    	... move player to the right ...;
//    	break;
//    	case KEY_PRESS_SPACE:
//    	... add a Squirt in front of the player...;
//    	break;
//    	// etc…
//    	}
//    	}
//    	...
//    	}
virtual ~IceMan() {}
	
};
class Water : virtual public Prop {
public:
    //constructor set up stuff in initialization list
//    Water (int x, int y) : Prop () { //stuff in ()s
//        setVisible();
//        //pick up able?
//    }
    virtual void doSomething(){}
    virtual ~Water() {}
};

class Oil : virtual public Prop {
public:
    //constructor set up stuff in initialization list
    virtual void doSomething(){}
    virtual ~Oil() {}
};

class Boulders : virtual public Prop {
public:
    //constructor set up stuff in initialization list
    virtual void doSomething(){}
    virtual ~Boulders() {}
    
private:
    //state 0 1 2 falling
};

class Gold : virtual public Prop {
public:
    //constructor set up stuff in initialization list
    virtual void doSomething(){}
    virtual ~Gold() {}
};

class SonarKit : virtual public Prop {
public:
    //constructor set up stuff in initialization list
    virtual void doSomething(){}
    virtual ~SonarKit() {}
};

class WaterRefill : virtual public Prop {
public:
    //constructor set up stuff in initialization list
    virtual void doSomething(){}
    virtual ~WaterRefill() {}
};

#endif ACTOR_H_