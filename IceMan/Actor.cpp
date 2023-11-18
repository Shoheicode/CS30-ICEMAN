#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
//Iceman
bool IceMan::isAlive(){
    if (lives == 0){
        return false;//ends level, terminates game
    }
    return true;
}
void IceMan::doSomething(){
    if (isAlive()){
        //if IceMan overlaps with Ice objects{
        //calls StudentWorld object
        //destroys Ice from the 4x4 area occupied by the Iceman (from x, y to x+3,y+3 inclusive)
        //play SOUND_DIG sound
        //}
        
        //else if (getWorld()->getKey(ch) == true) { //player pressed key
            // case Escape: isDead()
            // case KEY_PRESS_SPACE: shoot water
            //case KEY_PRESS_LEFT: if direction != left turn direction DON'T MOVE
            //case KEY_PRESS_RIGHT: if direction == right move, if nothing blocking iceMan
            //case KEY_PRESS_Z: if has any sonarCharge, use charge, decrement charge by one, nearby (radius of 12) props set to visible
            //case KEY_PRESS_TAB: drop gold for 100 ticks, if have in inventory, decrement gold and set to temp state, pick up-able by pro and hpro
        // }
    }
    else{
        return;
    }

}

//protester
void Protester::doSomething(){
    //if (isAlive()){
    //      if in rest state { do nothing}
    //else{
    // return
    //}
}
