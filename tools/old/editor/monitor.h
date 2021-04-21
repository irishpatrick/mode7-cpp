#ifndef MONITOR_H
#define MONITOR_H

#include <stdint.h>

int32_t monitor_init(int32_t, int32_t);
void monitor_destroy();
void monitor_clear();
void monitor_update();
void monitor_draw();
void monitor_set_viewport(int32_t, int32_t, float);
void monitor_draw_line(int32_t, int32_t, int32_t, int32_t);
int32_t monitor_should_quit();

#endif /* MONITOR_H */