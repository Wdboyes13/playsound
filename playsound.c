#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

struct pa_ctx {
    ma_engine* engine;
    ma_sound* snds;
    size_t nsnds;
    size_t ldsnds;
    int inited;
};

static struct pa_ctx ctx = {NULL, NULL, 0, 0, 0};

// initializes play_audio stuff
// pass 0 to nsnds for allocating the array for sounds on load
// but if the number of sounds is known at init, its faster to pass that number here
// will return -1 on error, 0 on success
int pa_init(size_t nsnds) {
    ctx.engine = malloc(sizeof(ma_engine));
    if (!ctx.engine) {
        return -1;
    }

    if (ma_engine_init(NULL, ctx.engine) != MA_SUCCESS) {
        free(ctx.engine);
        return -1;
    }

    ctx.nsnds = nsnds;
    if (nsnds > 0) {
        ctx.snds = calloc(nsnds, sizeof(ma_engine));
        if (!ctx.snds) {
            ma_engine_uninit(ctx.engine);
            free(ctx.engine);
            return -1;
        }
    }

    ctx.ldsnds = 0;
    ctx.inited = 1;
    return 0;
}

int pa_load(const char* path, size_t* id) {
    if (!ctx.inited) return -1;

    if (ctx.ldsnds > ctx.nsnds) {
        ma_sound* new_snds = realloc(ctx.snds, ctx.nsnds + 3);
        if (!new_snds) {
            return -1;
        }
        ctx.snds = new_snds;
    }

    *id = ctx.ldsnds;

    if (ma_sound_init_from_file(ctx.engine, path, MA_SOUND_FLAG_DECODE, NULL, NULL, &ctx.snds[ctx.ldsnds++]) != MA_SUCCESS) {
        return -1;
    }
    return 0;
}

int pa_play(size_t id) {
    if (!ctx.inited) return -1;
    if (ma_sound_start(&ctx.snds[id]) != MA_SUCCESS) {
        return -1;
    }
    return 0;
}

int pa_getpos(size_t id, size_t* pos) {
    if (!ctx.inited) return -1;
    *pos = ma_sound_get_time_in_pcm_frames(&ctx.snds[id]);
    return 0;
}

int pa_setpos(size_t id, size_t pos) {
    if (!ctx.inited) return -1;
    ma_sound_set_start_time_in_pcm_frames(&ctx.snds[id], pos);
    return 0;
}

int pa_setloop(size_t id, int loop) {
    if (!ctx.inited) return -1;
    ma_sound_set_looping(&ctx.snds[id], loop);
    return 0;
}

int pa_isplaying(size_t id) {
    if (!ctx.inited) return -1;
    return ma_sound_is_playing(&ctx.snds[id]);
}

int pa_stop(size_t id) {
    if (!ctx.inited) return -1;
    ma_sound_stop(&ctx.snds[id]);
    return 0;
}

int pa_unload(size_t id) {
    if (!ctx.inited) return -1;
    ma_sound_uninit(&ctx.snds[id]);
    return 0;
}

int pa_cleanup() {
    for (size_t i = 0; i < ctx.ldsnds; i++) {
        ma_sound_uninit(&ctx.snds[i]);
    }
    free(ctx.snds);
    free(ctx.engine);
    return 0;
}