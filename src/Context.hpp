#pragma once

#include <memory>
#include <string>

namespace shimera {

/**
 * Owns library-wide GL setup. Create it ONCE, after the host window exists.
 *
 * Loading GL function pointers requires a current context, and only the host can make one.
 * So create() detects the mistake and throws a message naming the call they are missing, per library.
 */
class Context {
    public:
        // Throws shimera::InitError if there is no current context, or GL cannot load.
        static Context create();

        ~Context();
        Context(Context&&) noexcept;
        Context& operator=(Context&&) noexcept;
        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;

        [[nodiscard]] static std::string getRendererInfo() ;

    private:
        Context();
};

}
