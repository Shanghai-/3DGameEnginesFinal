#ifndef WARMUPAPP_H
#define WARMUPAPP_H

#include <memory>

#include "engine/frame/Application.h"
#include "WarmupScreen.h"

class WarmupApp : public Application
{
public:
    WarmupApp(View *v);
    ~WarmupApp();
};

#endif // WARMUPAPP_H
