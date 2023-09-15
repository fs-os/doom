#include <stdio.h>
#include <time.h>       /* sleep_ms */
#include <kernel/pit.h> /* pit_get_ticks */

#include "doomgeneric.h"

static uint32_t start_tick;

void DG_Init() {
    //
}

void DG_DrawFrame() {
    /* TODO: This should change once we add window abstractions */
    static int y_margin          = 0;
    static int x_margin          = 0;
    uint32_t fb_w                = 0;
    uint32_t fb_h                = 0;
    static volatile uint32_t* fb = NULL;

    if (fb == NULL) {
        fb_w = fb_get_width();
        fb_h = fb_get_height();
        fb   = fb_getptr();

        if (DOOMGENERIC_RESX > fb_w || DOOMGENERIC_RESY > fb_h)
            return;

        y_margin = (fb_h - DOOMGENERIC_RESY) / 2;
        x_margin = (fb_h - DOOMGENERIC_RESX) / 2;
    }

    /* Swap DG_ScreenBuffer with framebuffer */
    for (uint32_t y = 0; y < DOOMGENERIC_RESY; y++) {
        for (uint32_t x = 0; x < DOOMGENERIC_RESX; x++) {
            const uint32_t fb_y = y + y_margin;
            const uint32_t fb_x = x + x_margin;

            fb[fb_y * fb_w + fb_x] = DG_ScreenBuffer[y * DOOMGENERIC_RESX + x];
        }
    }
}

void DG_SleepMs(uint32_t ms) {
    sleep_ms(ms);
}

uint32_t DG_GetTicksMs() {
    /* PIT is configured to tick at 1000Hz, once each ms */
    return pit_get_ticks() - start_tick;
}

int DG_GetKey(int* pressed, unsigned char* key) {
    //
}

void DG_SetWindowTitle(const char* title) {
    /* We don't have windows yet */
    (void)title;
}

/*-----------------------------------------------------------------------------*/

int main_doom(int argc, char** argv) {
    /* Used for DG_GetTicksMs */
    start_tick = pit_get_ticks();

    doomgeneric_Create(argc, argv);

    for (;;)
        doomgeneric_Tick();

    return 0;
}
