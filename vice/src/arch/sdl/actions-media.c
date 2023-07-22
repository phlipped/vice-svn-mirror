/** \file   actions-media.c
 * \brief   UI action implementations for media recording (SDL)
 *
 * \author  Bas Wassink <b.wassink@ziggo.nl>
 */

/*
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

#include <stddef.h>
#include <stdbool.h>

#include "sound.h"
#include "screenshot.h"
#include "uiactions.h"
#include "uimenu.h"

#include "actions-media.h"


/** \brief  Show dialog to record audio action
 *
 * \param[in]   self    action map
 */
static void media_record_audio_action(ui_action_map_t *self)
{
    sdl_ui_menu_item_activate_by_action(self->action);
}

/** \brief  Show dialog to take screenshot action
 *
 * \param[in]   self    action map
 */
static void media_record_screenshot_action(ui_action_map_t *self)
{
    sdl_ui_menu_item_activate_by_action(self->action);
}

/** \brief  Show dialog to record video action
 *
 * \param[in]   self    action map
 */
static void media_record_video_action(ui_action_map_t *self)
{
    sdl_ui_menu_item_activate_by_action(self->action);
}

/** \brief  Stop recording audio/video action
 *
 * \param[in]   self    action map
 */
static void media_stop_action(ui_action_map_t *self)
{
    if (sound_is_recording()) {
        sound_stop_recording();
    }
    if (screenshot_is_recording()) {
        screenshot_stop_recording();
    }
}


/** \brief  List of mappings for media recording actions */
static const ui_action_map_t media_actions[] = {
    {   .action  = ACTION_MEDIA_RECORD_AUDIO,
        .handler = media_record_audio_action,
        .dialog  = true
    },
    {   .action  = ACTION_MEDIA_RECORD_SCREENSHOT,
        .handler = media_record_screenshot_action,
        .dialog  = true
    },
    {   .action  = ACTION_MEDIA_RECORD_VIDEO,
        .handler = media_record_video_action,
        .dialog  = true
    },
    {   .action  = ACTION_MEDIA_STOP,
        .handler = media_stop_action
    },
    UI_ACTION_MAP_TERMINATOR
};


/** \brief  Register media recording actions */
void actions_media_register(void)
{
    ui_actions_register(media_actions);
}
