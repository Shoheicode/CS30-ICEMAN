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
void moveTo(int x, int y) {}
};
class HardcoreProtester: virtual public Protester {
public:
//…
};

class Prop : virtual public Actor{
private:
    
public:
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

	}
};

#endif // ACTOR_H_
