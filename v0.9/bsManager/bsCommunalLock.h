#ifndef  BS_COMMUNAL_LOCK_H_20111130
#define  BS_COMMUNAL_LOCK_H_20111130
#include <stdlib.h>
#include <QMutex>
#include <stdio.h>

extern "C" void bsManagerObjectLock();
extern "C" void bsManagerObjectUnLock();
#endif
