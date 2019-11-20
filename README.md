# OpenGL-game--Flee-Game
This game is a about collecting gems(collect them all before the countdown timer) placed randomly in a maze while adding some evel(colored :D)ghosts as additional complexity. 

The game is implemented using opengl on C++ where the game maze(organized as grid) is dynamically created using DFS (which visits every grid cell randomly only once keeping previous information in memory).
The ghosts are moving randomly in the maze (there's no AI involved left as future work).

### The game has two modes
1) Human vs Ghosts (collect gems and escape from ghosts)
2) Computer mode (ghosts just play against each others where no human involved just watching those random lifeless things :D)

### The game has two implementation versions:
I wanted to check the performance using plain c++ threads instead of event handlers provided by opengl system
1) multi-threaded game
2) using opengl event handlers and timers (recommended)


## Preview Video
[![preview](https://img.youtube.com/vi/1Iph6ac-cDk/0.jpg)](https://youtu.be/1Iph6ac-cDk)


