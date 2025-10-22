
# Game design considerations

More details on my thoughts are available in the [development blog] (https://averagebear.game.blog/).

## Rules
1. No AI-generated content. 
2. No Digital Rights Management (DRM). No anti-piracy.
3. No Achievements.
4. Minimal "fanservice"
5. Permadeath
	- Few if any story-convenient exceptions.
	- Workaround: Units can live through the DnD-like Agony state.
6. No Phoenix/Casual mode
7. No Ambush Spawns
8. No Teleportation
	- Maybe except rescue staves
9. No Time travel
10. No Dragons

## Narrative is king.
- Keyword: Subtlelty. Not overly explicit and frank.
- Everything in the game tells a story.
Themes: 
- Death
	- Of Empires & People
	- Tragedy and Glory of War
- Slavery to Gods & Kings
- Fighting Demons within & without

## Units have two hands
- Swords & shields
- Dual Wielding & Two-handing
- Handedness: Right, left or Ambidextrous

## Linear, chapter-based
- Camp between chapters where you can move around on the map, go to shops, talk to people.
	- Replaces menu navigation?
- Army movement on the world map only if it makes narrative sense
	- No enemy encounters on world map
- Side chapters unlockable.
- Notes taken in books magically present on a newgame

## Mounts 
- No mount on promotion, get mounts through story/gameplay. 
- Almost any unit/class can have a mount: horse, pegasus, etc.
	- Some mounts are scared of magic.
	- Some mounts have unit/classes preferences. 
- Mounts get attached to rider.

## Items
- Breakable
- Becomes unusable "broken trash" item when broken
- Weapon's remaining number of uses can be increased using whetstones

## Skills:
- Limited number of simpler skills
- Skills more tightly coupled with narrative
- 5 Skills maximum per unit: 4 Passives, 1 Active.

## Questionable:
- IronMan Mode 
	- Automatic game save + deletion of previous save 
- Forcible IronMan Mode
	- Force player into IronMan mode. The game always saves, no retries. 
- Cities are visitable like camps?

## Under the hood:
- Multithreading? 
	- Might be useful to load assets in the background
- OpenGL?
	- Hardware acceleration, make use of GPU. 

## Long-term
Playable on
1. Smartphones (iOS/Android)
2. Consoles?

# Roadmap
- Turn system
	1. AI for enemy attacks		(0.1)
	2. Switching between armies (0.2)
- Design
	1. Weapons 	(1.1)
		* Types 	(1.1)
	2. Tiles 	(0.3)
	3. Maps
		* Test chapter 	(1.0)
		* Other chapters
	4. Units 		(0.2)
		* Names 		(0.2)
		* Bases 		(0.1)
		* Caps 			(0.1)
		* Promotions 	(0.1)
- Animations
	1. Cursor idle 		(1.0) 
	2. Cursor slide 	(1.0) 
	3. Unit walking 	(0.1)
	4. Unit idle 		(0.1)
	5. Unit portraits
	6. Turn transition 	(1.0)
- Menus
	1. Dynamic Unit Actions (1.0)
	2. Unit Statistics 		(0.3)
	3. Unit Attack 			(0.2)
	4. Options
- Grid
	1. Unit movement on grid 	(1.0) 
	2. Attackable tiles 		(1.0)
	3. Moveable tiles 			(1.0)
	4. Danger tiles 			(1.0)
	5. Camera Pan and Zoom 		(1.0)
- Controllers
	1. Gamepad Controller 		(0.4: D-Pad + Joystics + 2 buttons)
	2. Keyboard Controller 		(0.3: Move Keys + 2 buttons)
	3. Mouse Controller 		(0.2: Movement + 2 buttons)
	4. Touch Controller
	5. Contextual inputs 		(0.2)
	6. Mouse clickable buttons 	(0.2)
- Music and Sounds
	1. Music 			(1.0)
	1. Sound effects 	(1.0)
	2. Scene sounds
	3. Songs			
- Art
	1. Art style 		(0.3)
	2. Unit concepts
	3. Unit portraits
	4. Map units 		(0.3)
	5. Tiles 			(0.2)
	6. Custom Mouse 	(0.2)
	7. Title Screen 	(0.2)
	8. World Map
- Gameplay systems
	1. Convoy 	(1.0)
	2. Camp  	(0.2)
	3. Combat 	(2.0)
- Story
	1. Main thrust 				(0.3)
	2. Chapter introductions
	3. Side-chapters
	4. Camp Events
	5. Relationship Events
	6. Supports

## Credits
:copyright: Average Bear Games, made by Gabriel Taillon
