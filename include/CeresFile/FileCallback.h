#pragma once
#include <filesystem>
#include <functional>
#include "FileAction.h"

typedef std::function<void(const std::filesystem::path&, FileAction action)>
    FileCallback;