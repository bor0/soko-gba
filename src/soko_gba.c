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
/* standard includes (GBA) */
#include <stdlib.h>
#include <stdio.h>

/* GBA includes */
#include <BoyScout.h>
#include <gba_console.h>
#include <gba_interrupt.h>
#include <gba_input.h>
#include <gba_video.h>
#include <gba_sprites.h>
#include <gba_systemcalls.h>

/* soko includes */
#include "soko_gba.h"
#include "game.h"

/* auto-generated headers from sprites.bmp and tune.bgf */
#include "sprites.h"
#include "tune_bgf.h"

unsigned int frame;

void consoleCls();

SpriteEntry sprites[128];

/* update music on vertical blank interrupt */
void VblankInterrupt()
{
    BoyScoutUpdateSong();
    frame += 1;
}

/* copy sprites to OAM (object-attribute memory) */
void _copy_OAM()
{
    u16 loop;
    u16 *temp = (u16 *)sprites;
    u16 *oam = (u16 *)OAM;

    for (loop = 0; loop < 128*4; loop++)
    {
        oam[loop] = temp[loop];
    }
}

void _wait_vsync()
{
    while ((volatile u16)REG_VCOUNT != 160);
}

/* read the soko map and update sprites accordingly */
void _render(struct level *map)
{
    int i, j, cnt = 0;

    for (i = 0; i < map->height; i++) {
        for (j = 0; j < map->width; j++) {
            if (map->data[i][j] == 0) {
                sprites[cnt].attribute[0] = 160 | ATTR0_DISABLED;
                sprites[cnt].attribute[1] = 240;
            } else {
                sprites[cnt].attribute[0] = OBJ_256_COLOR | SQUARE | i * 16;
                sprites[cnt].attribute[1] = ATTR1_SIZE_16 | j * 16;
            }
            sprites[cnt].attribute[2] = map->data[i][j] * 8;
            cnt++;
        }
    }
}

/* VBlankIntrWait() takes ~1/60 second */
void _wait(int seconds)
{
    int i;

    for (i = 0; i < seconds * 60; i++) {
        VBlankIntrWait();
    }
}

int _get_input(struct level *map)
{
    scanKeys();
    int held = keysDown();

    if (held & KEY_UP) {
        play(map, 0, -1);
    } else if (held & KEY_DOWN) {
        play(map, 0, 1);
    } else if (held & KEY_LEFT) {
        play(map, -1, 0);
    } else if (held & KEY_RIGHT) {
        play(map, 1, 0);
    } else if (held & KEY_START) {
        return 1;
    }

    return 0;
}

/* initialize sprites by moving them off screen (fully zeroed sprite doesn't mean it's invisible) */
void _clear_sprites()
{
    int i;

    for (i = 0; i < sizeof(sprites) / sizeof(sprites[0]); i++) {
        sprites[i].attribute[0] = 160 | ATTR0_DISABLED;
        sprites[i].attribute[1] = 240;
    }
}

/* copy palette to GBA memory */
void _set_palette(const u16 *palette)
{
    int i;

    for (i = 0; i < 256; i++) {
        OBJ_COLORS[i] = palette[i];
    }
}

/* copy bitmap to GBA memory */
void _decode_bitmap(const u16 *bitmap, u16 *dst, int offset)
{
    int i;

    for (i = 0; i < 128; i++) {
        dst[i + offset] = bitmap[i + offset];
    }
}

void sokogba_initialize()
{
    int i;
    unsigned int nBSSongSize;

    irqInit();

    /* audio */
    /* initialize BoyScout */
    BoyScoutInitialize();

    /* get needed song memory */
    nBSSongSize = BoyScoutGetNeededSongMemory(tune_bgf);

    /* allocate and set BoyScout memory area */
    BoyScoutSetMemoryArea((unsigned int)malloc(nBSSongSize));

    /* open song */
    BoyScoutOpenSong(tune_bgf);

    /* play song and loop */
    BoyScoutPlaySong(1);

    irqEnable(IRQ_VBLANK);
    irqSet(IRQ_VBLANK, VblankInterrupt);

    /* video */
    consoleDemoInit();
    printf("\n\n\nSoko-gba v1.0\nby Boro Sitnikovski\n\nMarch, 2015\n\n\nPress START at any time to\nskip the current level.");
    _wait(7);
    consoleCls();

    SetMode(MODE_1 | OBJ_ENABLE | OBJ_1D_MAP);

    _set_palette(spritesPal);
    _clear_sprites();

    for (i = 0; i < 9; i++) {
        _decode_bitmap(spritesTiles, SPRITE_GFX, 128*i);
    }
}

void sokogba_main(struct level *map)
{
    while (1) {
        if (_get_input(map)) {
            break;
        }

        _render(map);

        _wait_vsync();
        _copy_OAM();

        if (check_win(map) == 1) {
            _wait(3);
            break;
        }
    }

    _clear_sprites();
}
