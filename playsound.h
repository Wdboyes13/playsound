#pragma once
#include <stdint.h>
#include <stddef.h>

int pa_init(size_t nsnds);
int pa_load(const char* path, size_t* id);
int pa_play(size_t id);
int pa_getpos(size_t id, size_t* pos);
int pa_setpos(size_t id, size_t pos);
int pa_setloop(size_t id, int loop);
int pa_isplaying(size_t id);
int pa_stop(size_t id);
int pa_unload(size_t id);
int pa_cleanup();