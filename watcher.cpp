#include <climits>
#include <libgen.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include "efsw/efsw.hpp"

#include "watcher.hpp"

static efsw::FileWatcher* fileWatcher;
static std::map<std::string, std::vector<std::pair<std::string, void(*)(const std::string&)>>> callbacks;

class UpdateListener : public efsw::FileWatchListener
{
public:
    UpdateListener() {}

    void handleFileAction( efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = "" )
    {
        std::string fullpath = dir + (dir[dir.length()-1] != '/' ? "/" : "") + filename;
        for (auto& clb : callbacks[fullpath]) {
            clb.second(clb.first);
        }
    }
};

static UpdateListener* listener;

void watcher_initialize(void)
{
    fileWatcher = new efsw::FileWatcher();
    listener = new UpdateListener();
    fileWatcher->watch();
}

void watcher_add_file(const std::string& filename, void(*clb)(const std::string& filename))
{
    if (!fileWatcher) return;

    auto& vec = callbacks[filename];
    if (std::find(vec.begin(), vec.end(), std::make_pair(filename,clb)) != vec.end()) {
        // we assumed it's already watched
        return;
    }

    char fullpath[PATH_MAX+1];
    realpath(filename.c_str(), fullpath);
    char dir[PATH_MAX+1];
    strcpy(dir, fullpath);
    char* d = dirname(dir);
    if (d != dir)
        strcpy(dir, d);
    fileWatcher->addWatch(dir, listener, false);
    callbacks[fullpath].push_back(std::make_pair(filename, clb));
}

