#ifndef LAUNCHER_H
#define LAUNCHER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "app.h"

void StartLauncherTask();
void LauncherTask(void *argument);

#ifdef __cplusplus
}
#endif

#endif
