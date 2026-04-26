#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include "backend/BackendFactory.hpp"
#include "effects/DistortionEffect.hpp"
#include "effects/ChromaticAberration.hpp"

#ifdef _WIN32
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
#endif

int main() {
    sf::RenderWindow window(sf::VideoMode({960, 540}), "shimera memory test - sfml");
    window.setActive(true);
    glewInit();

#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // Test 1 : backend
    {
        const IBackend *backend = BackendFactory::create();
        delete backend;
    }

    // Test 2 : framebuffers
    {
        IBackend *backend = BackendFactory::create();
        const IFrameBuffer *fb1 = backend->createFrameBuffer(960, 540);
        const IFrameBuffer *fb2 = backend->createFrameBuffer(960, 540);
        delete fb1;
        delete fb2;
        delete backend;
    }

    // Test 3 : post processor
    {
        IBackend *backend = BackendFactory::create();
        const IPostProccessor *pp = backend->createPostProcessor(
            "../../../../res/shader/postprocessing/postprocess.vert",
            "../../../../res/shader/postprocessing/normal.frag"
        );
        delete pp;
        delete backend;
    }

    // Test 4 : effects
    {
        IBackend *backend = BackendFactory::create();
        const DistortionEffect *effect = new DistortionEffect(backend);
        delete effect;
        delete backend;
    }

    // Test 5 : create framebuffer in a loop
    {
        IBackend *backend = BackendFactory::create();
        for (int i = 0; i < 100; i++) {
            const IFrameBuffer *fb = backend->createFrameBuffer(960, 540);
            delete fb;
        }
        delete backend;
    }

#ifdef _WIN32
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtDumpMemoryLeaks();
#endif

    return 0;
}