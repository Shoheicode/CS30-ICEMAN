#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor
bool Actor::outOfField(int x, int y, Actor::Direction d){
    switch(d){
        case Actor::up:
            if(y + 2 >= 62){
                return true;
            }
            break;
        case Actor::down:
            if(y + 5 <= 1){
                return true;
            }
            break;
        case Actor::left:
            if(x + 5 <= 1){
                return true;
            }
            break;
        case Actor::right:
            if(x -2 >= 63){
                return true;
            }
            break;
        case Actor::none:
            return false;
            break;
    }
    return false;
}


//Iceman
bool IceMan::isAlive(){
//    if (m_lives == 0){
//        return false;//ends level, terminates game
//    }
    return true;
}

void IceMan::doSomething(){
    if (isAlive()){
        //if IceMan overlaps with Ice objects{
        //calls StudentWorld object getWorld->destroyIce..?
        //destroys Ice from the 4x4 area occupied by the Iceman (from x, y to x+3,y+3 inclusive)
        //playSound(SOUND_DIG);
        //}
        overlap(studW);
        int a;
        
        if (getWorld()->getKey(a) == true) { //player pressed key
            switch (a){
                case KEY_PRESS_ESCAPE:
                    //isDead();
                    break;
                case KEY_PRESS_SPACE:
                    //shoot water
                case KEY_PRESS_LEFT:
                    if (getDirection() != left){
                        setDirection(left); //turn direction DON'T MOVE
                    }
                    else if (outOfField(getX(), getY(), getDirection())){ //out of scope
                        break;
                    }
                    else{
                        moveTo(getX() - 1, getY());
                        studW->increaseScore(100);
                    }
                    break;
                case KEY_PRESS_RIGHT:
                    if (getDirection() != right){
                        setDirection(right); //turn direction DON'T MOVE
                    }
                    else if (outOfField(getX(), getY(), getDirection())){ //out of scope
                        break;
                    }
                    else{
                        if (getX() != 61) {
                            //move if nothing blocks it
                            moveTo(getX() + 1, getY());
                        }
                        
                    }
                    break;
                case KEY_PRESS_UP:
                    if (getDirection() != up){
                        setDirection(up); //turn direction DON'T MOVE
                    }
                    else if (outOfField(getX(), getY(), getDirection())){ //out of scope
                        break;
                    }
                    else{
                        if (getY() != 60) {
                            //move if nothing blocks it
                            moveTo(getX(), getY() + 1);
                        }
                    }
                    break;
                case KEY_PRESS_DOWN:
                    if (getDirection() != down){
                        setDirection(down); //turn direction DON'T MOVE
                    }
                    else if (outOfField(getX(), getY(), getDirection())){ //out of scope
                        break;
                    }
                    else{
                        //move if nothing blocks it {
                        if (getY() != 0) {
                            moveTo(getX(), getY() - 1);
                        }
                        //}
                    }
                    break;
//                case KEY_PRESS_Z:
//                    if (sC < 0){
//                        //use charge,
//                        playSound(SOUND_SONAR);
//                        sC--;
//                        //nearby (radius of 12) props set to visible
//                    }
                    //break;
                    case KEY_PRESS_TAB:
                    if (gold < 0){
                        //drop gold for 100 ticks,
                        gold--;
                       // and set to temp state, pick up-able by pro and hpro
                        }
                        break;
                    // }
            }
        }
    }
    else{
        return;
    }

}

void IceMan::overlap(StudentWorld* world) {
    if (getY() <= 59) {
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                if (getY()+y <= 59) {
                    if (world->getMap().at(getY() + y).at(getX() + x) != nullptr) {
                        Ice* temp = world->getMap().at(getY() + y).at(getX() + x);
                        world->getMap().at(getY() + y).at(getX() + x) = nullptr;
                        delete temp;
                    }
                    else {
                        cout << "I AM NOT OVERLAPPING" << endl;
                    }
                }
            }
        }
    }
}

void IceMan::getAnnoyed(int dAmage){
    //if is shouted at
    dAmage--;
    dAmage--;
        //if damage ==0
        //setState("dead");
        //playSound(SOUND_PLAYER_GIVE_UP);
}

//protester
void Protester::doSomething(){
    //if (isAlive()){
    //      if in rest state { do nothing}
    //else{
    // return
    //}
}
