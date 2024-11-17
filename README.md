Dungeon Crawler is a retro-inspired, text-based adventure game where players navigate a dungeon filled with treasures, monsters, and magical artifacts. 

The objective is to collect at least one treasure and escape the dungeon alive through the exit while avoiding lethal monsters and navigating impassable obstacles.

Players control their adventurer using simple w, a, s, d keys to move and e to stay in place. 

The dungeon features dynamic resizing triggered by magic amulets, while monsters actively pursue the player when in line of sight. 

The game is built with modular, efficient C++ code, leveraging dynamic memory allocation and a 2D array representation for the dungeon map. 

o : This represents the player, also known as the adventurer, navigating the dungeon.
$ : Treasure, which the player needs to collect. At least one treasure is required to unlock the exit.
@ : A magic amulet that resizes the dungeon, expanding it with additional sections while adding more challenges and opportunities.
M : A monster that chases the player when in line of sight. Monsters are lethal upon contact.
+, -, | : Impassable obstacles, forming the walls or barriers within the dungeon.
? : A door that leads to the next level of the dungeon.
! : The dungeon's exit, which can only be used if the player has collected at least one treasure.
