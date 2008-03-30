/*
 * vsync.c - End-of-frame handling on RISC OS.
 *
 * Written by
 *  Andreas Dehmel <dehmel@forwiss.tu-muenchen.de>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "vice.h"

#include "wimp.h"
#include "types.h"
#include "vsync.h"
#include "vsyncarch.h"
#include "resources.h"
#include "video.h"
#include "sound.h"
#include "ui.h"
#include "kbd.h"
#include "kbdbuf.h"
#include "joystick.h"
#include "maincpu.h"




/* Exported variables */
int NumberOfRefreshes = 0;
int EmuWindowHasInputFocus = 0;


#define MAX_SKIPPED_FRAMES	10

static double refresh_frequency;

static long cycles_per_sec;

/* Function to call at end of every frame */
static void (*vsync_hook)(void);

static int refresh_rate;

static int warp_mode_enabled;



static int set_refresh_rate(resource_value_t v, void *param)
{
  if ((int)v < 0)
    return -1;
  refresh_rate = (int)v;
  return 0;
}


static int set_warp_mode(resource_value_t v, void *param)
{
  warp_mode_enabled = (int)v;
  sound_set_warp_mode(warp_mode_enabled);
  return 0;
}





void suspend_speed_eval(void)
{
}


static resource_t resources[] = {
  {"RefreshRate", RES_INTEGER, (resource_value_t)0,
    (resource_value_t*)&refresh_rate, set_refresh_rate, NULL },
  {"WarpMode", RES_INTEGER, (resource_value_t)0,
    (resource_value_t*)&warp_mode_enabled, set_warp_mode, NULL },
  {NULL}
};


int vsync_init_resources(void)
{
  return resources_register(resources);
}


int vsync_init_cmdline_options(void)
{
  return 0;
}


void vsync_set_machine_parameter(double refresh_rate, long cycles)
{
    refresh_frequency = refresh_rate;
    cycles_per_sec = cycles;
}

void vsync_init(void (*hook)(void))
{
  vsync_hook = hook;
}


static unsigned short frame_counter = 0xffff;
static unsigned short num_skipped_frames = 0;
static int skip_counter=0;

int do_vsync(int been_skipped)
{
  int skip_next_frame = 0;

  if (FullScreenMode != 0)
  {
    EmuWindowHasInputFocus = 1;
  }
  else
  {
    RO_Caret caret;

    Wimp_GetCaretPosition(&caret);
    /* This will be used by keyboard and joystick code */
    EmuWindowHasInputFocus = (canvas_for_handle(caret.WHandle) == NULL) ? 0 : 1;
  }

  vsync_hook();

  joystick();

  ui_poll();

  video_full_screen_plot_status();

  if (warp_mode_enabled) {
    if (skip_counter < MAX_SKIPPED_FRAMES) {
      skip_next_frame = 1;
      skip_counter++;
    } else {
      skip_counter = 0;
    }
    sound_flush(0);
  } else if (refresh_rate != 0) {
    if (skip_counter < refresh_rate - 1) {
      skip_next_frame = 1;
      skip_counter++;
    } else {
      skip_counter = 0;
    }
    sound_flush(CurrentSpeedLimit);
  } else {
    if (skip_counter >= NumberOfRefreshes) {
      NumberOfRefreshes = -1;
      skip_counter = 0;
    } else {
      if (skip_counter < MAX_SKIPPED_FRAMES) {
        skip_next_frame = 1;
        skip_counter++;
      } else {
        skip_counter = 0;
      }
    }
    sound_flush(CurrentSpeedLimit);
  }

  if (frame_counter >= refresh_frequency * 2) {
    num_skipped_frames = 0;
    frame_counter = 0;
  } else {
    frame_counter++;
  }

  if (skip_next_frame == 0) NumberOfRefreshes++;

  kbd_buf_flush();

  return skip_next_frame;
}


int vsync_disable_timer(void)
{
  return 0;
}

