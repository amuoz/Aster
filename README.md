# Aster

## Setting up
### Mac
git clone --recursive https://github.com/amuoz/Aster.git
Install https://cmake.org/download/
`brew install cmake`
`mkdir Build`
`cd Build`
`cmake -G "Xcode" ..`
Double click in `Aster/Build/Aster.xcodeproj`
Inside Xcode:
  `Product`	>	`Scheme`	>	`Edit	Scheme` > `Run` > `Executable` > `Aster`
Press play (upper left corner)
### Windows
git clone --recursive https://github.com/amuoz/Aster.git
Install https://cmake.org/download/
`mkdir Build`
`cd Build`
`cmake -G "Visual Studio 19" "Win64" ..`
Double click in `Aster/Build/Aster.sln`
Inside Visual Studio:
`Aster` > Set as startup project

## Actor
Base class for all the entities in the game (blocks, enemies, player, power ups, ..).

When crafting a new entity you should override ONLY WHEN NECESSARY the following methods:
**Update**: Tick logic.
**Draw**: Rendering. Override only if special rendering.
**TakeDamage**: What to do when entity takes damage.
**Move**: Override only if moving entity.
**OnContact**: Collision with other entity collider.

Unreal engine actor lifecycle:
https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/Actors/ActorLifecycle/index.html

## Level
Level descriptor defined in **one.json** inside Levels folder.

**LevelInfo** variable holds all the level information.

Add new types of entities in the level: push it into **Actors** level variable. This is done in **Level::Load** method.

## Game
Main class.

**Asset** loading in **Game::InitGame** (.png, .json, shaders, .ttf, ..)

**State** variable tells game current state (Game Active, Game Over).

## Player Controller
User **input** and user **interface**.

**Posses** takes control of player actor in runtime.

## Physics engine
Physics engine simulates **collisions** and resolve them.

To add collisions to an entity call **AddDynamicActor** from entity's constructor. This will create a **PhysicActor** which is the representation of the entity in the physics engine. An entity can have multiple collisions (PhysicActor).

Disable **bCheckCollision** if you don't want the physics engine to check for collisions.

Activate **justReport** if you only want to detect non-blocking collision overlaps.

Activate **bSimulate** if you want the movement of the entity to be simulated by physics.