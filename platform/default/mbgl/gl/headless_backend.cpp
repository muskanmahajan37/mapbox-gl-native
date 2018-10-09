#include <mbgl/gl/headless_backend.hpp>
#include <mbgl/gl/context.hpp>
#include <mbgl/renderer/backend_scope.hpp>

#include <cassert>
#include <stdexcept>
#include <type_traits>

namespace mbgl {

class HeadlessBackend::View {
public:
    View(gl::Context& context, Size size_, bool useTextureMemoryExternal)
        : color(context.createTexture(size_, gl::TextureFormat::RGBA, 0, gl::TextureType::UnsignedByte, useTextureMemoryExternal ? gl::TextureMemory::External : gl::TextureMemory::Local)),
          depthStencil(context.createRenderbuffer<gl::RenderbufferType::DepthStencil>(size_)),
          framebuffer(context.createFramebuffer(color, depthStencil)) {
    }

    gl::Texture color;
    gl::Renderbuffer<gl::RenderbufferType::DepthStencil> depthStencil;
    gl::Framebuffer framebuffer;
};

HeadlessBackend::HeadlessBackend(Size size_)
    : size(size_) {
}

HeadlessBackend::HeadlessBackend(Size size_, void* app_instance, gl::ExternalTextureAllocateCB allocate_cb, gl::ExternalTextureFreeCB free_cb)
    : size(size_) {
  BackendScope guard { *this };
  getContext().setExternalTextureCallbacks(app_instance, allocate_cb, free_cb);
  viewUseTextureMemoryExternal = true;
}

HeadlessBackend::~HeadlessBackend() {
    BackendScope guard { *this };
    view.reset();
    context.reset();
}

gl::ProcAddress HeadlessBackend::getExtensionFunctionPointer(const char* name) {
    assert(impl);
    return impl->getExtensionFunctionPointer(name);
}

void HeadlessBackend::activate() {
    active = true;

    if (!impl) {
        createImpl();
    }

    assert(impl);
    impl->activateContext();
}

void HeadlessBackend::deactivate() {
    assert(impl);
    impl->deactivateContext();
    active = false;
}

void HeadlessBackend::bind() {
    gl::Context& context_ = getContext();

    if (!view) {
        view = std::make_unique<View>(context_, size, viewUseTextureMemoryExternal);
    }

    context_.bindFramebuffer = view->framebuffer.framebuffer;
    context_.scissorTest = false;
    context_.viewport = { 0, 0, size };
}

Size HeadlessBackend::getFramebufferSize() const {
    return size;
}

void HeadlessBackend::updateAssumedState() {
    // no-op
}

void HeadlessBackend::setSize(Size size_) {
    size = size_;
    view.reset();
}

PremultipliedImage HeadlessBackend::readStillImage() {
    return getContext().readFramebuffer<PremultipliedImage>(size);
}

} // namespace mbgl
