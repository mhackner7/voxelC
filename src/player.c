#include "player.h"


#define PLAYER_HEIGHT   2.0f 
#define PLAYER_WIDTH    0.9f
#define PLAYER_DEPTH    0.3f



Player init_player(u64 x, u64 y, u64 z) {
    Player player = {
        .x = x, 
        .y = y, 
        .z = z,
        .height = PLAYER_HEIGHT,
        .width  = PLAYER_WIDTH,
        .depth  = PLAYER_DEPTH,
    };
    init_cam(&player.cam);
};

void update_player(Player *player, Window *window, SDL_Event *event, GLuint shader, f64 delta_time) {

    if (!player->win_focused) {

        /** UPDATE INPUT FUNCTIONS TO TAKE PLAYER
         * 
         * player position needs to be updated independently of camera position 
         * 
         * (in case the camera is switched to third person)
         * 
         */

        handle_mouse_pst(event, &player->cam, window, delta_time);

        update_camera_vectors(&player->cam);

        handle_key_pst(&player->cam, delta_time);

        handle_keytaps(event, delta_time);

        update_mvp(&player->cam, window, shader);
    }
}