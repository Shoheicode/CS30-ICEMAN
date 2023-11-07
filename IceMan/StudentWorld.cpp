#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

// while (The player has lives left) 
// { 
// Prompt_the_user_to_start_playing(); // “press a key to start” Initialize_the_game_world(); // you’re going to write this func 
// while (The player is still alive) 
// { 
// // each pass through this loop is a tick (1/20th of a sec) 
// // you’re going to write code to do the following 
// Ask_all_actors_to_do_something(); 
// If_any_actors_died_then_delete_them_from_the_world(); 
// // we write this code to handle the animation for  
// you Animate_all_of_the_alive_actors_to_the_screen();  
// Sleep_for_50ms_to_give_the_user_time_to_react(); 
// } 
// // the player died – you’re going to write this code  
// Cleanup_all_game_world_objects(); // you’re going to write this  if (The player has more lives) 
// Prompt_the_player_to_continue(); 
// } 
// Tell_the_user_the_game_is_over(); // “game over!”; we provide this 
