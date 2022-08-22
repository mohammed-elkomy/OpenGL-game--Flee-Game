# OpenGL-game--Flee-Game
- A simple opengl automatically generated maze-based game. 
- Collect randomly placed gems before the countdown timer to win.
- Evil ghosts are added to make it a bit harder :3.

The game is implemented using opengl C++ where the maze grid is dynamically created using DFS (which randomly visits every grid cell only once while keeping previous information in memory).
The ghosts are moving randomly in the maze (there's no AI involved, left as future work).

### The game has two modes
1) Human vs Ghosts (collect gems and escape from ghosts)
2) Computer mode (ghosts just play against each others where no human involved just watching those random lifeless things :D)

### The game has two versions of implementation:
I wanted to check the performance using plain c++ threads instead of event handlers provided by opengl system
1) multi-threaded implementation
2) using opengl event handlers and timers (recommended)

**Caveat**: Use visual studio c++ compiler, g++ won't work.

## Preview Video
[![preview](https://img.youtube.com/vi/1Iph6ac-cDk/0.jpg)](https://youtu.be/1Iph6ac-cDk)


