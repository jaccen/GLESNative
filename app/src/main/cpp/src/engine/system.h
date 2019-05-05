#pragma once

#include "common/exenode.h"

namespace duar
{

////////////////////////////////////////////////////////////////////////////////
enum class SystemType : u8
{
    MESSAGE,  //
    RENDER,   //
    RESOURCE, //
};

using ISystem = ExeNode< SystemType >;

template< typename T >
using TSystem = TExeNode< T, SystemType >;

extern std::unique_ptr< ISystem >& get_engine_system();

}; // namespace duar