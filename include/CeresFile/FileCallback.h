#pragma once
#include <filesystem>
#include <functional>
#include "FileAction.h"

namespace CeresFile {

typedef std::function<void(const std::filesystem::path&, FileAction action)>
    FileCallback;

}
