#include "foundation/filesystem/vfs.h"
#include "foundation/log/log_system.h"
#include "foundation/utility/string_utils.h"

#include <cassert>
#include <fstream>
#include <limits>
#include <sstream>
#include <utility>

#ifdef WIN32
#include <Shlwapi.h>
#else
extern "C"
{
#include <glob.h>
}
#endif

using namespace muggle::vfs;

Blob::Blob(void* data, size_t size) : data_(data), size_(size)
{}
Blob::~Blob()
{
    if (data_)
    {
        free(data_);
    }
}

const void* Blob::data() const
{
    return data_;
}
size_t Blob::size() const
{
    return size_;
}

bool NativeFileSystem::isFolderExists(const std::filesystem::path& name)
{
    return std::filesystem::exists(name) && std::filesystem::is_directory(name);
}

bool NativeFileSystem::isFileExists(const std::filesystem::path& name)
{
    return std::filesystem::exists(name) && std::filesystem::is_regular_file(name);
}

std::shared_ptr<IBlob> NativeFileSystem::readFile(const std::filesystem::path& name)
{
    std::ifstream file(name, std::ios::binary);

    if (!file.is_open())
    {
        LOG_ERROR("Open File: {} Failed!", name.string());
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    uint64_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* data = static_cast<char*>(malloc(size));

    assert(data != nullptr);

    file.read(data, size);

    if (!file.good())
    {
        LOG_ERROR("Read File: {} Failed!", name.string());
        assert(0);
        return nullptr;
    }

    return std::make_shared<Blob>(data, size);
}

bool NativeFileSystem::writeFile(const std::filesystem::path& name, const void* data, size_t size)
{
    std::ofstream file(name, std::ios::binary);

    if (!file.is_open())
    {
        LOG_ERROR("Open File: {} Failed!", name.string());
        return false;
    }

    if (size > 0)
    {
        file.write(static_cast<const char*>(data), static_cast<std::streamsize>(size));
    }

    if (!file.good())
    {
        LOG_ERROR("Write File: {} Failed!", name.string());
        assert(0);
        return false;
    }

    return true;
}

static int enumerateNativeFiles(const char* pattern, bool directories, enumerate_callback_t callback)
{
#ifdef WIN32
    WIN32_FIND_DATAA findData;
    HANDLE           hFind = FindFirstFileA(pattern, &findData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            return 0;
        }
        return static_cast<int>(Status::Failed);
    }

    int numEntries = 0;

    do
    {
        const bool isDirectory = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        const bool isDot       = strcmp(findData.cFileName, ".") == 0;
        const bool isDotDot    = strcmp(findData.cFileName, "..") == 0;

        if (isDirectory == directories && !isDot && !isDotDot)
        {
            callback(findData.cFileName);
            ++numEntries;
        }

    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);

    return numEntries;
#else
    glob64_t globMatches;
    int      globResult = glob64(pattern, 0, nullptr, &globMatches);

    if (globResult == 0)
    {
        int numEntries = 0;

        for (int i = 0; i < globMatches.gl_pathc; ++i)
        {
            const char*                      globentry = (glob_matches.gl_pathv)[i];
            std::error_code                  ec, ec2;
            std::filesystem::directory_entry entry(globentry, ec);
            if (!ec)
            {
                if (directory_entry == entry.is_directory(ec2) && !ec2)
                {
                    callback(entry.path().filename().native());
                    ++numEntries;
                }
            }
        }

        globfree64(&globMatches);

        return numEntries;
    }

    if (globResult == GLOB_NOMATCH)
        return 0;

    return static_cast<int>(Status::Failed);

#endif
}

int NativeFileSystem::enumerateFiles(const std::filesystem::path&    path,
                                     const std::vector<std::string>& extensions,
                                     enumerate_callback_t            callback,
                                     bool                            allowDuplicates)
{
    (void)allowDuplicates;

    if (extensions.empty())
    {
        std::string patten = (path / "*").generic_string();
        return enumerateNativeFiles(patten.c_str(), false, callback);
    }

    int numEntries = 0;
    for (const auto& ext : extensions)
    {
        std::string pattern = (path / ("*" + ext)).generic_string();
        int         result  = enumerateNativeFiles(pattern.c_str(), false, callback);

        if (result < 0)
            return result;

        numEntries += result;
    }

    return numEntries;
}

int NativeFileSystem::enumerateDirectories(const std::filesystem::path& path,
                                           enumerate_callback_t         callback,
                                           bool                         allowDuplicates)
{
    (void)allowDuplicates;

    std::string pattern = (path / "*").generic_string();
    return enumerateNativeFiles(pattern.c_str(), true, callback);
}

RelativeFileSystem::RelativeFileSystem(std::shared_ptr<IFileSystem> fs, const std::filesystem::path& basePath) :
    underlyingFS_(std::move(fs)), basePath_(basePath.lexically_normal())
{}

bool RelativeFileSystem::isFolderExists(const std::filesystem::path& name)
{
    return underlyingFS_->isFolderExists(basePath_ / name.relative_path());
}

bool RelativeFileSystem::isFileExists(const std::filesystem::path& name)
{
    return underlyingFS_->isFileExists(basePath_ / name.relative_path());
}

std::shared_ptr<IBlob> RelativeFileSystem::readFile(const std::filesystem::path& name)
{
    return underlyingFS_->readFile(basePath_ / name.relative_path());
}

bool RelativeFileSystem::writeFile(const std::filesystem::path& name, const void* data, size_t size)
{
    return underlyingFS_->writeFile(basePath_ / name.relative_path(), data, size);
}

int RelativeFileSystem::enumerateFiles(const std::filesystem::path&    path,
                                       const std::vector<std::string>& extensions,
                                       enumerate_callback_t            callback,
                                       bool                            allowDuplicates)
{
    return underlyingFS_->enumerateFiles(basePath_ / path.relative_path(), extensions, callback, allowDuplicates);
}

int RelativeFileSystem::enumerateDirectories(const std::filesystem::path& path,
                                             enumerate_callback_t         callback,
                                             bool                         allowDuplicates)
{
    return underlyingFS_->enumerateDirectories(basePath_ / path.relative_path(), callback, allowDuplicates);
}

void VFileSystem::mount(const std::filesystem::path& path, std::shared_ptr<IFileSystem> fs)
{
    if (findMountPoint(path, nullptr, nullptr))
    {
        LOG_ERROR("Cannot mount a filesystem at {}: there is another FS that includes this path", path.string());
        return;
    }

    mountPoints_.push_back(std::make_pair(path.lexically_normal().generic_string(), fs));
}

void VFileSystem::mount(const std::filesystem::path& path, const std::filesystem::path& nativePath)
{
    mount(path, std::make_shared<RelativeFileSystem>(std::make_shared<NativeFileSystem>(), nativePath));
}

bool VFileSystem::unmount(const std::filesystem::path& path)
{
    std::string spath = path.lexically_normal().generic_string();

    for (size_t index = 0; index < mountPoints_.size(); index++)
    {
        if (mountPoints_[index].first == spath)
        {
            mountPoints_.erase(mountPoints_.begin() + index);
            return true;
        }
    }

    return false;
}

bool VFileSystem::findMountPoint(const std::filesystem::path& path,
                                 std::filesystem::path*       pRelativePath,
                                 IFileSystem**                ppFS)
{
    std::string spath = path.lexically_normal().generic_string();

    for (auto mp : mountPoints_)
    {
        if (spath.find(mp.first, 0) == 0 &&
            ((spath.length() == mp.first.length()) || (spath[mp.first.length()] == '/')))
        {
            if (pRelativePath)
            {
                std::string relative = spath.substr(mp.first.size() + 1);
                *pRelativePath       = relative;
            }

            if (ppFS)
            {
                *ppFS = mp.second.get();
            }
            return true;
        }
    }

    return false;
}

bool VFileSystem::isFolderExists(const std::filesystem::path& name)
{
    std::filesystem::path relativePath;
    IFileSystem*          fs = nullptr;

    if (findMountPoint(name, &relativePath, &fs))
    {
        return fs->isFolderExists(relativePath);
    }

    return false;
}

bool VFileSystem::isFileExists(const std::filesystem::path& name)
{
    std::filesystem::path relativePath;
    IFileSystem*          fs = nullptr;

    if (findMountPoint(name, &relativePath, &fs))
    {
        return fs->isFileExists(relativePath);
    }

    return false;
}

std::shared_ptr<IBlob> VFileSystem::readFile(const std::filesystem::path& name)
{
    std::filesystem::path relativePath;
    IFileSystem*          fs = nullptr;

    if (findMountPoint(name, &relativePath, &fs))
    {
        return fs->readFile(name);
    }

    return nullptr;
}

bool VFileSystem::writeFile(const std::filesystem::path& name, const void* data, size_t size)
{
    std::filesystem::path relativePath;
    IFileSystem*          fs = nullptr;

    if (findMountPoint(name, &relativePath, &fs))
    {
        return fs->writeFile(name, data, size);
    }

    return false;
}

int VFileSystem::enumerateFiles(const std::filesystem::path&    path,
                                const std::vector<std::string>& extensions,
                                enumerate_callback_t            callback,
                                bool                            allowDuplicates)
{
    std::filesystem::path relativePath;
    IFileSystem*          fs = nullptr;

    if (findMountPoint(path, &relativePath, &fs))
    {
        return fs->enumerateFiles(path, extensions, callback, allowDuplicates);
    }

    return static_cast<int>(Status::PathNotFound);
}

int VFileSystem::enumerateDirectories(const std::filesystem::path& path,
                                      enumerate_callback_t         callback,
                                      bool                         allowDuplicates)
{
    std::filesystem::path relativePath;
    IFileSystem*          fs = nullptr;

    if (findMountPoint(path, &relativePath, &fs))
    {
        return fs->enumerateDirectories(path, callback, allowDuplicates);
    }

    return static_cast<int>(Status::PathNotFound);
}

static void appendPatternToRegex(const std::string& pattern, std::stringstream& regex)
{
    for (char c : pattern)
    {
        switch (c)
        {
            case '?':
                regex << "[^/]?";
                break;
            case '*':
                regex << "[^/]+";
                break;
            case '.':
                regex << "\\.";
                break;
            default:
                regex << c;
        }
    }
}

std::string muggle::vfs::getFileSearchRegex(const std::filesystem::path&    path,
                                            const std::vector<std::string>& extensions)
{
    std::filesystem::path normalizedPath    = path.lexically_normal();
    std::string           normalizedPathStr = normalizedPath.generic_string();

    std::stringstream regex;
    appendPatternToRegex(normalizedPathStr, regex);
    if (!muggle::string_utils::ends_with(normalizedPathStr, "/") && !normalizedPath.empty())
        regex << '/';
    regex << "[^/]+";

    if (!extensions.empty())
    {
        regex << '(';
        bool first = true;
        for (const auto& ext : extensions)
        {
            if (!first)
                regex << '|';
            appendPatternToRegex(ext, regex);
            first = false;
        }
        regex << ')';
    }

    return regex.str();
}