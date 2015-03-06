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
#include <stdlib.h>
#include <assert.h>
#include "level.h"

void free_level(struct level **map)
{
    int i;

    if (*map != NULL) {
        if ((*map)->data != NULL) {
            for (i = 0; i < (*map)->height; i++) {
                if ((*map)->data[i] != NULL) {
                    free((*map)->data[i]);
                }
            }

            free((*map)->data);
        }

        free(*map);
        *map = NULL;
    }
}

struct level *read_level(char *level, int w, int h)
{
    int i, j, soko = 0, cnt = 0;
    struct level *map = NULL;
    char tmp;

    map = malloc(sizeof(struct level));

    assert(map != NULL);

    map->width = w;
    map->height = h;

    map->data = malloc(sizeof(char *) * map->height);

    assert(map->data != NULL);

    for (i = 0; i < map->height; i++) {
        map->data[i] = malloc(sizeof(char) * map->width);

        assert(map->data[i] != NULL);

        for (j = 0; j < map->width; j++, cnt++) {
            tmp = level[cnt];
            assert(tmp != '\0');

            map->data[i][j] = tmp - '0';

            if (map->data[i][j] == LEVEL_SOKOBAN) {
                /* soko already specified elsewhere. treat other soko values as walls */
                if (soko == 1) {
                    map->data[i][j] = LEVEL_WALL;
                } else {
                    soko = 1;
                    map->soko.x = j;
                    map->soko.y = i;
                }
            }
        }
    }

    /* if there is no soko specified in map assign it to pos (0, 0) */
    if (!soko) {
        map->soko.x = map->soko.y = 0;
        map->data[0][0] = LEVEL_SOKOBAN;
    }

    map->soko.player_on_beacon = map->soko.key_count = 0;

    return map;
}
