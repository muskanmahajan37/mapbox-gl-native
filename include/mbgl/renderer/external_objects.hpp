#pragma once

namespace mbgl {
namespace gl {

using ExternalTextureAllocateCB = std::add_pointer_t<int(void*, uint32_t, uint32_t)>;
using ExternalTextureFreeCB = std::add_pointer_t<void(void*, int)>;

} // namespace gl
} // namespace mbgl
