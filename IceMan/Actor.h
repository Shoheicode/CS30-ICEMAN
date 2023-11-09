#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: virtual public GraphObject {
public:
	int getX() const{ return 0;}
	int getY() const{return 0;}
    	virtual void doSomething(){
        	//	void Ask_all_actors_to_do_something()
        	//	{
        	//	for each actor on the level:
        	//	if (the actor is still alive)
        	//	tell the actor to doSomething();
        	//	}
    	}
    	virtual ~Actor() {}
  //  …
};
class Protester: virtual public Actor {
public:
//setVisible(bool shouldIDisplay);
    void moveTo(int x, int y) {};
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
    
public:
virtual ~Prop() {}
};

class IceMan : virtual public Actor{
private:
	int lives;
public:
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
    //setVisible(bool shouldIDisplay);
    void moveTo(int x, int y) {}
    virtual ~Water() {}
};

class Oil : virtual public Prop {
public:
    //setVisible(bool shouldIDisplay);
    void moveTo(int x, int y) {}
    virtual ~Oil() {}
};

class Boulders : virtual public Prop {
public:
    //setVisible(bool shouldIDisplay);
    void moveTo(int x, int y) {}
    virtual ~Boulders() {}
};

class Gold : virtual public Prop {
public:
    //setVisible(bool shouldIDisplay);
    void moveTo(int x, int y) {}
    virtual ~Gold() {}
};

class SonarKit : virtual public Prop {
public:
    //setVisible(bool shouldIDisplay);
    void moveTo(int x, int y) {}
    virtual ~SonarKit() {}
};

class WaterRefill : virtual public Prop {
public:
    //setVisible(bool shouldIDisplay);
    void moveTo(int x, int y) {}
    virtual ~WaterRefill() {}
};

#endif // ACTOR_H_
