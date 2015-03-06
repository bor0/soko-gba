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
#include "level.h"
#include "soko_gba.h"
#include <stdlib.h>
#include <assert.h>

struct levels {
    char *level;
    int w, h;
} all_levels[] = {
    {"0011100000141000001211111113234114236111111131000001410000011100", 8, 8}
    , {"1111111111122222222112224222211222222221122232342112423622211222232221122224222112222222211111111111", 10, 10}
    , {"1111111111122222228112111222211214122221121312222112171262211222222221122222222112222222211111111111", 10, 10}
    , {"0000001111100000014221000000141211111111412112623232321121212121111222222210011111111100", 11, 8}
    , {"111110000122211000123221000112321111011164221001224121001222221001111111", 9, 8}
    , {"1111111112224421122633211111121100012210000122100001221000011110", 8, 8}
    , {"222222212222111112123421432721432211112212522222622123411122221432878322218521112211112222227874222", 11, 9}
};

int main(void) {
    int i;

    struct level *map;

    /* initialize GBA */
    sokogba_initialize();

    for (i = 0; i < sizeof(all_levels) / sizeof(all_levels[0]); i++) {
        /* attempt to parse level */
        map = read_level(all_levels[i].level, all_levels[i].w, all_levels[i].h);

        assert(map != NULL);

        /* main GBA loop */
        sokogba_main(map);

        /* release level */
        free_level(&map);
    }

    return 1;

}
