#pragma once

#include <mbgl/gl/extension.hpp>
#include <mbgl/gl/gl.hpp>

namespace mbgl {
namespace gl {
namespace extension {

class ExternalObjects {
public:
    template <typename Fn>
    ExternalObjects(const Fn& loadExtension)
      : createMemoryObjects(
            loadExtension({ { "GL_EXT_memory_object_fd", "glCreateMemoryObjectsEXT" } })),
        deleteMemoryObjects(
            loadExtension({ { "GL_EXT_memory_object_fd", "glDeleteMemoryObjectsEXT" } })),
        importMemoryFd(
            loadExtension({ { "GL_EXT_memory_object_fd", "glImportMemoryFdEXT" } })),
        texStorageMem2D(
            loadExtension({ { "GL_EXT_memory_object_fd", "glTexStorageMem2DEXT" } })) {
  }

  const ExtensionFunction<void(GLsizei n, GLuint *memoryObjects)> createMemoryObjects;
  const ExtensionFunction<void(GLsizei n, const GLuint *memoryObjects)> deleteMemoryObjects;
  const ExtensionFunction<void(GLuint memory, GLuint64 size, GLenum handleType, GLint fd)> importMemoryFd;
  const ExtensionFunction<void(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset)> texStorageMem2D;
};

} // namespace extension
} // namespace gl
} // namespace mbgl
