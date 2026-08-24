#pragma once

#include <string>
#include <string_view>

#ifndef ORBIT_SIM_ROOT
#define ORBIT_SIM_ROOT ""
#endif

// Joins the CMake-defined project root with a path relative to the repo (forward slashes).
inline std::string orbitAssetPath(std::string_view relative)
{
    while (!relative.empty() && relative[0] == '/')
        relative.remove_prefix(1);

    std::string root(ORBIT_SIM_ROOT);
    if (root.empty())
        return std::string(relative);

    if (root.back() != '/')
        root += '/';
    root.append(relative);
    return root;
}
