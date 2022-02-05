#pragma once

#include "foundation/filesystem/vfs.h"
#include "foundation/log/log_system.h"

namespace muggle
{
LogSystem* gLoggerSystem;

void init()
{
    gLoggerSystem = new LogSystem();
}

void terminate()
{
    delete gLoggerSystem;
}
} // namespace muggle