#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#define SND(id) (&ctx.snds[id])
#define MSND(id) (&ctx.snds[id].snd)

struct pa_snd {
    ma_sound snd;
    int inited;
};

struct pa_ctx {
    ma_engine* engine;
    struct pa_snd* snds;
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
        ctx.snds = calloc(nsnds, sizeof(struct pa_snd));
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
        struct pa_snd* new_snds = realloc(ctx.snds, sizeof(struct pa_snd) * (ctx.nsnds + 3));
        if (!new_snds) {
            return -1;
        }
        ctx.snds = new_snds;
        ctx.nsnds += 3;
    }

    *id = ctx.ldsnds;

    if (ma_sound_init_from_file(ctx.engine, path, MA_SOUND_FLAG_DECODE, NULL, NULL, MSND(ctx.ldsnds++)) != MA_SUCCESS) {
        return -1;
    }

    SND(*id)->inited = 1;
    return 0;
}

int pa_play(size_t id) {
    if (!ctx.inited) return -1;
    if (ma_sound_start(MSND(id)) != MA_SUCCESS) {
        return -1;
    }
    return 0;
}

int pa_getpos(size_t id, size_t* pos) {
    if (!ctx.inited) return -1;
    *pos = ma_sound_get_time_in_pcm_frames(MSND(id));
    return 0;
}

int pa_setpos(size_t id, size_t pos) {
    if (!ctx.inited) return -1;
    ma_sound_set_start_time_in_pcm_frames(MSND(id), pos);
    return 0;
}

int pa_setloop(size_t id, int loop) {
    if (!ctx.inited) return -1;
    ma_sound_set_looping(MSND(id), loop);
    return 0;
}

int pa_isplaying(size_t id) {
    if (!ctx.inited) return -1;
    return ma_sound_is_playing(MSND(id));
}

int pa_stop(size_t id) {
    if (!ctx.inited) return -1;
    ma_sound_stop(MSND(id));
    return 0;
}

int pa_unload(size_t id) {
    if (!ctx.inited) return -1;
    ma_sound_uninit(MSND(id));
    SND(id)->inited = 0;
    return 0;
}

int pa_cleanup() {
    for (size_t i = 0; i < ctx.ldsnds; i++) {
        if (SND(i)->inited) {
            SND(i)->inited = 0;
            ma_sound_uninit(MSND(i));
        }
    }

    free(ctx.snds);
    free(ctx.engine);
    return 0;
}