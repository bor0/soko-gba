/*
This file is part of Soko-gba.

Soko-gba is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Soko-gba is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Soko-gba. If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef GAME_H
#define GAME_H
#include "level.h"

void play(struct level *, int, int);
int check_win(struct level *);
#endif
