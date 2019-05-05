#pragma once

#include "common/types.h"

namespace duar
{

////////////////////////////////////////////////////////////////////////////////
struct EngineContext
{
    u32 screen_width;
    u32 screen_height;
    void* win_handle;
    void* gfx_context;
    void* ar_session;
};

extern const EngineContext& get_engine_context();

} // namespace duar