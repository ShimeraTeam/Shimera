#include "ContextTest.hpp"
#include "backend/BackendFactory.hpp"
#include "EffectPipeline.inl"
#include "effects/DistortionEffect.hpp"

using namespace shimera;

TEST(BackendFactory, CreateBackendWithoutContext) {
    const IBackend* backend = BackendFactory::create();
    EXPECT_NE(backend, nullptr);
    delete backend;
}

TEST_F(ContextTest, CreateBackend) {
    const IBackend* backend = BackendFactory::create();
    EXPECT_NE(backend, nullptr);
    delete backend;
}

TEST(IFrameBuffer, CreateFrameBufferWithoutContext) {
    GTEST_SKIP(); // undefined behavior occurs without a valid OpenGL context.
    const IBackend* backend = BackendFactory::create();
    EXPECT_NE(backend, nullptr);
    EXPECT_THROW(backend->createFrameBuffer(800, 600), std::runtime_error);
    delete backend;
}

TEST_F(ContextTest, CreateFrameBuffer) {
    IBackend* backend = BackendFactory::create();
    EXPECT_NE(backend, nullptr);
    EXPECT_NE(backend->createFrameBuffer(800, 600), nullptr);
    delete backend;
}

TEST(EffectPipeline, CreateEffectPipelineWithoutContext) {
    IBackend* backend = BackendFactory::create();
    EXPECT_NE(backend, nullptr);
    const EffectPipeline pipeline(backend, 800, 600);
    EXPECT_EQ(pipeline.size(), 0);
    delete backend;
}

TEST_F(ContextTest, CreateEffectPipeline) {
    IBackend* backend = BackendFactory::create();
    EXPECT_NE(backend, nullptr);
    const EffectPipeline pipeline(backend, 800, 600);
    EXPECT_EQ(pipeline.size(), 0);
    delete backend;
}

TEST_F(ContextTest, AddEffect) {
    IBackend* backend = BackendFactory::create();
    EXPECT_NE(backend, nullptr);
    EffectPipeline pipeline(backend, 800, 600);
    EXPECT_EQ(pipeline.size(), 0);
    pipeline.addEffect<DistortionEffect>();
    EXPECT_EQ(pipeline.size(), 1);
    delete backend;
}

TEST_F(ContextTest, GetEffect) {
    IBackend* backend = BackendFactory::create();
    EXPECT_NE(backend, nullptr);
    EffectPipeline pipeline(backend, 800, 600);
    EXPECT_EQ(pipeline.size(), 0);
    pipeline.addEffect<DistortionEffect>();
    EXPECT_EQ(pipeline.size(), 1);
    const auto& effect = pipeline.get<DistortionEffect>();
    EXPECT_EQ(&effect, &pipeline.get<DistortionEffect>());
    delete backend;
}

TEST_F(ContextTest, GetEffectNotFound) {
    IBackend* backend = BackendFactory::create();
    EXPECT_NE(backend, nullptr);
    EffectPipeline pipeline(backend, 800, 600);
    EXPECT_EQ(pipeline.size(), 0);
    EXPECT_THROW(pipeline.get<DistortionEffect>(), std::out_of_range);
    delete backend;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}