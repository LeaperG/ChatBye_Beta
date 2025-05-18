#pragma once

enum class HostBindingMode
{
    DynamicPort,  // search ports on failure
    FixedPort,    // bind strictly to the specified port
};

