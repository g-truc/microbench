#pragma once

#include <gli/gli.hpp>

gli::texture load_png(char const* Filename);
void save_png(gli::texture const& Texture, char const* Filename);
