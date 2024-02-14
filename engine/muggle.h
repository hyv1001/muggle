#pragma once

#include "foundation/filesystem/vfs.h"
#include "foundation/log/log_system.h"

namespace muggle
{
vfs::VFileSystem* gFileSystem;
LogSystem*        gLoggerSystem;

void init()
{
    gFileSystem   = new vfs::VFileSystem();
    gLoggerSystem = new LogSystem();
}

void terminate()
{
    delete gLoggerSystem;
    delete gFileSystem;
}
} // namespace muggle