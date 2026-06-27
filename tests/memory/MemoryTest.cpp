#include "MemoryTest.hpp"
#include "backend/BackendFactory.hpp"
#include "effects/DistortionEffect.hpp"

#ifdef _WIN32
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
#endif

MemoryTest::MemoryTest(const std::string &testName) {
    m_name = testName;
}

std::string MemoryTest::getName() const {
    return m_name;
}

void MemoryTest::run() {

    #ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // Test 1 : backend
    {
        const shimera::IBackend *backend = shimera::BackendFactory::create();
        delete backend;
    }

    // Test 2 : framebuffers
    {
        shimera::IBackend *backend = shimera::BackendFactory::create();
        const shimera::IFrameBuffer *fb1 = backend->createFrameBuffer(960, 540);
        const shimera::IFrameBuffer *fb2 = backend->createFrameBuffer(960, 540);
        delete fb1;
        delete fb2;
        delete backend;
    }

    // Test 3 : post processor
    {
        shimera::IBackend *backend = shimera::BackendFactory::create();
        const shimera::IPostProcessor *pp = backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/normal.frag"
        );
        delete pp;
        delete backend;
    }

    // Test 4 : effects
    {
        shimera::IBackend *backend = shimera::BackendFactory::create();
        const shimera::DistortionEffect *effect = new shimera::DistortionEffect(backend);
        delete effect;
        delete backend;
    }

    // Test 5 : create framebuffer in a loop
    {
        shimera::IBackend *backend = shimera::BackendFactory::create();
        for (int i = 0; i < 100; i++) {
            const shimera::IFrameBuffer *fb = backend->createFrameBuffer(960, 540);
            delete fb;
        }
        delete backend;
    }

#ifdef _WIN32
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtDumpMemoryLeaks();
#endif

}
