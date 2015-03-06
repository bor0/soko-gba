Soko-gba v1.0 by Boro Sitnikovski
=================================

To clone me: Write `git clone git@github.com:bor0/soko-gba.git`

1. To install dependencies:
    - Download http://sourceforge.net/projects/devkitpro/files/devkitARM/devkitARM_r44/devkitARM_r44-x86_64-linux.tar.bz2/download
    - Download http://sourceforge.net/projects/devkitpro/files/libgba/libgba-20150106.tar.bz2/download
2. Set DEVKITARM and DEVKITPRO accordingly.
3. Write "make" to get soko executable.
4. Write "make clean" to remove unnecessary files.

Screenshots:

![Alt text](/screenshots/gameplay.png?raw=true "Soko")

Game logic is based on my previous Soko game `git@github.com:bor0/soko`

Due to the orthogonality between game logic and other parts of the app, porting the game was easy. All I needed to replace was the drawing and music stuff.

Boro Sitnikovski

Licensed under GPL.  Read LICENSE for more information.

March, 2015
