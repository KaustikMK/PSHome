/* Compile with: orbis-clang main.c -o fake_shell.elf */
#include <orbis/libkernel.h>
#include <orbis/SystemService.h>
#include <orbis/Pad.h>
#include <orbis2d.h>
#include <debugnet.h>
#include <stdio.h>
#include <string.h>

#define NUM_TILES 5
#define TILE_W 300
#define TILE_H 170
#define TILE_SPACING 340

typedef struct Tile {
    const char *name;
} Tile;

static Tile tiles[NUM_TILES] = {
    { "Game One" },
    { "Game Two" },
    { "Game Three" },
    { "Game Four" },
    { "Game Five" }
};

static int selected = 0;

static Orbis2dConfig *conf2d;
static OrbisPadConfig *confPad;

static void init_app()
{
    conf2d = orbis2dGetConf();
    orbis2dInit(conf2d);

    confPad = orbisPadGetConf();
    orbisPadInit(confPad);
}

static void finish_app()
{
    orbisPadFinish();
    orbis2dFinish();
}

static void render_tiles()
{
    orbis2dStartDrawing();
    orbis2dClearBuffer(0x002f6690); /* static blue background */

    int baseX = 960 - TILE_W / 2;
    int y = 400;

    for (int i = 0; i < NUM_TILES; ++i) {
        int x = baseX + (i - selected) * TILE_SPACING;
        uint32_t box = 0x404040ff;
        orbis2dDrawRectColor(x, y, TILE_W, TILE_H, box);
        if (i == selected) {
            orbis2dDrawRectColor(x - 4, y - 4, TILE_W + 8, TILE_H + 8, 0xffffffff);
        }
        orbis2dDrawString(x + 20, y + 70, 0xffffffff, tiles[i].name);
    }

    orbis2dSwapBuffers();
}

static void show_launch_message(const char *name)
{
    char text[64];
    snprintf(text, sizeof(text), "Launching %s", name);
    orbis2dStartDrawing();
    orbis2dClearBuffer(0x002f6690);
    orbis2dDrawString(960 - (strlen(text) * 8) / 2, 540, 0xffffffff, text);
    orbis2dSwapBuffers();
    sceKernelUsleep(2000000);
}

static void handle_input(int *running)
{
    orbisPadUpdate();
    OrbisPadData pad = *orbisPadGetData(0);

    if (pad.buttons.pressed & ORBIS_PAD_OPTIONS) {
        *running = 0;
        return;
    }
    if (pad.buttons.pressed & ORBIS_PAD_LEFT) {
        selected = (selected + NUM_TILES - 1) % NUM_TILES;
    }
    if (pad.buttons.pressed & ORBIS_PAD_RIGHT) {
        selected = (selected + 1) % NUM_TILES;
    }
    if (pad.buttons.pressed & ORBIS_PAD_CROSS) {
        show_launch_message(tiles[selected].name);
    }
}

int main(int argc, char *argv[])
{
    init_app();

    int running = 1;
    while (running) {
        handle_input(&running);
        render_tiles();
        sceKernelUsleep(16000);
    }

    finish_app();
    return 0;
}

