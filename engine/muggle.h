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
    auto exe_dir = vfs::getCurrentProcessDirectory();
    gFileSystem->mount("/ROOT", exe_dir.parent_path());

    gLoggerSystem = new LogSystem();

    if (gFileSystem->isFolderExists("/ROOT/content"))
    {
        LOG_INFO("content folder exists")
    }
    else
    {
        LOG_ERROR("content folder does not exist")
    }

    LOG_INFO("Muggle initialized");
}

void terminate()
{
    LOG_INFO("Muggle terminated")

    delete gLoggerSystem;
    delete gFileSystem;
}
} // namespace muggle