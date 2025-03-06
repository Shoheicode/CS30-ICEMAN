# ICEMAN

## Table of Contents:

<details>
  <summary>Project Overview</summary>
  
- [Project Description](#description)
- [Game Description](#game-description)
- [Project Members](#members)
  
</details>

<details>
  <summary>Student World Class</summary>
  
- [StudentWorld](#StudentWorld)
- [Game Description](#game-description)
- [Project Members](#members)
</details>

<details>
  <summary>Actors</summary>

- <details>
    <summary>IceMan</summary>
  </details>
- <details>
    <summary>Protestors</summary>
  </details>
  
</details>

---

## Description

This project contains IceMan, a project which encorporates multiple inheritance, algorithims, and data structures to create a working video game.

This project tested our ability to work in a team setting, developing and creating different code as well as utilize different code bases in order to create functional working code.

## Game Description

The game "Iceman" involves the player controlling an Iceman character to dig through underground tunnels in an oil field to collect barrels of oil. The player starts with three lives and must complete multiple levels, each with a unique configuration. The Iceman is placed at the top-middle of the oil field at the start of each level with full health, water for the squirt gun, and a sonar charge.

#### Game Setup and Objective:

- The player must dig and find all hidden oil barrels to progress to the next level.
- Each level has boulders, gold nuggets, and barrels of oil randomly distributed. Only boulders are visible initially.

#### Iceman's Actions:

- The Iceman can move using arrow keys, dig through ice, and avoid boulders.
- The squirt gun can be used to stun protesters.
- Gold nuggets can be used to bribe protesters.
- Sonar charges reveal hidden items within a radius.

#### Enemies (Protesters):

- Regular Protesters: Easier to deal with, can be bribed or stunned.
- Hardcore Protesters: More challenging, cannot be bribed, and track the Iceman's location if close enough.
- Both types of protesters roam the field and irritate the Iceman by shouting, which decreases his health.

#### Game Mechanics:

- Iceman loses health when shouted at by protesters or hit by falling boulders.
- Lives are lost when health reaches zero; the game ends when all lives are lost.
- Water pools occasionally appear for refilling the squirt gun.
- Friends drop sonar kits to help find hidden items.

Points are earned for various actions such as squirting protesters, dropping boulders on them, and collecting items.
#### Scoring:
- Regular Protester gives up: 100 points
- Hardcore Protester gives up: 250 points
- Boulder bonks Protester: 500 points
- Bribing Regular Protester: 25 points
- Bribing Hardcore Protester: 50 points
- Barrel of oil: 1000 points
- Gold Nugget: 10 points
- Water: 100 points
- Sonar Kit: 75 points

#### Game Play:

- Game play is divided into ticks, with each tick allowing the Iceman and other objects to perform actions.
- The Iceman can:
  - move using WSAD or the Arrow Keys
  - shoot using space bar
  - use sonar using ___
  - restart the level with the escape key.
- Protesters and other objects have their own actions and interactions within each tick.

## Members

### Jason's Portion:
My name is Jason and this is the project I created for my Advanced C++ Course.

In my Advanced C++ Course, I was involved in a complex project focused on developing a video game using many advanced C++ concepts such as concurrency, inheritance, and the Standard Template Library (STL). As a member of the programming team, my responsibilities included designing and implementing key algorithms, conducting thorough research on the implementation and usage of certain data structures to optimize the game running, and collaborating with many team members to merge our code into one project.

During the project, one of the things I had to do was to learn how to implement a first-breadth search algorithm into a new system. Using an array data structure and queue, I managed to create an algorithm that allowed the enemy to find the exit after the enemy died. At first, this caused the game to run slower due to taking up a lot of storage and processing power but through concurrency, it managed to significantly speed up the program and manage to make the running of the algorithim work. I also utilized a vector to make the map for game and keep track of the characters. 
I worked with a high level of independence in my day-to-day tasks as the project was not based on a timeline given by our professor. I also took ownership of specific modules within the project including making critical decisions about algorithm selection, optimizing code for performance, and troubleshooting issues that arose during the development process. Additionally, I actively participated in team meetings, providing insights and updates on my progress, and collaborating with colleagues to address challenges and make decisions. Throughout the project, I utilized version control systems such as Github to manage code changes and ensure seamless collaboration with other team members. 

When deciding our project parameters, I suggested using Jira to help manage our work and make a timeline for when certain parts of the project would need to get done like the
character movement and collision detection. Our team optimized and created code more effectively through these programs and kept everyone on the same page on tasks that needed to be completed. The project's success resulted from our collective efforts, with each team member contributing specialized skills and knowledge to
achieve our common goals.

This experience not only enhanced my technical and computer skills but also honed my ability to work effectively within a collaborative and dynamic team environment.

---

## StudentWorld 
### Description
`StudentWorld` is a C++ class that manages the game environment, object spawning, and game mechanics for a game inspired by the "Ice Man" concept. It extends `GameWorld` and is responsible for handling game objects, collision detection, AI logic, and player interactions.

### Features
- **Object Management:**
  - Spawns game objects like oil, boulders, gold nuggets, and ice.
  - Removes dead objects from the game world.
  - Manages player interactions with objects.

- **Pathfinding:**
  - Implements breadth-first search (BFS) to determine paths.
  - Calculates shortest paths for AI characters (protesters) to reach the player.
  - Detects obstacles (ice and boulders) to adjust movement decisions.

- **Player Interaction:**
  - Allows the player to collect and use gold, sonar, and water.
  - Detects proximity between player and game objects.
  - Updates the game status text dynamically.

### File Structure
- `StudentWorld.h` - Header file containing class declarations.
- `StudentWorld.cpp` - Implementation of `StudentWorld` class functions.
- `Actor.h` - Base class defining game objects.
- `Actor.cpp` - Implementation of actors such as Ice, Gold, Boulder, and Protester.

### Code Breakdown

#### 1. **Object Spawning**
- **`createIceMap()`**: Creates a 2D vector representing the ice map with a predefined empty gap.
- **`spawnOil(int oNum)`**: Randomly places oil barrels, ensuring proper spacing.
- **`spawnBoulders(int bNum)`**: Randomly places boulders, clearing the ice from the area.
- **`spawnNuggets(int num)`**: Randomly places gold nuggets within the game area.

#### 2. **Object Removal**
- **`removeDeadObjects()`**: Iterates through the list of actors and deletes those marked as dead.

#### 3. **Pathfinding and AI Movement**
- **`findPath(int x, int y, int objx, int objy)`**: Uses BFS to generate the shortest path for AI-controlled characters.
- **`getLeadingPathDistance(int x, int y)`**: Determines the best direction for AI to move toward a target.
- **`findPathToIceMan(int x, int y, int maxMoves)`**: AI-controlled Protesters use this to find a path to the player.

#### 4. **Player Actions**
- **`dropGold(int x, int y)`**: Drops gold for AI characters to pick up.
- **`useSonar(int x, int y)`**: Reveals hidden objects within a radius.
- **`useSpray(int x, int y)`**: Fires water to hit AI-controlled enemies.
- **`completeLevel()`**: Checks if the player has collected all required oil barrels.

#### 5. **Collision and Movement Logic**
- **`blockedByRocks(int x, int y)`**: Checks if a boulder is blocking a movement.
- **`blockedByIce(int x, int y)`**: Checks if ice is blocking movement.
- **`blockedbyRocksOrIce(int x, int y, Actor::Direction d)`**: Combines both checks for ease of movement logic.
- **`checkFacingDirection(int x, int y, Actor::Direction d)`**: Determines if an object is facing the player.

#### 6. **Game Status Updates**
- **`updateTextBox()`**: Updates the heads-up display (HUD) with current game stats.
- **`formatText(...)`**: Formats the game status information into a readable string.

### Dependencies
- Standard C++ libraries (`<vector>`, `<list>`, `<string>`, `<queue>`, `<cmath>`, `<iostream>`)

### Future Improvements
- Optimize `removeDeadObjects()` by avoiding unnecessary memory operations.
- Improve AI movement efficiency using heuristic pathfinding (e.g., A* algorithm).
- Implement difficulty scaling with more advanced AI behaviors.

---

## Actors

