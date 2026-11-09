#include <stdexcept>
#include <string>

#include "ContextTest.hpp"

using namespace shimera;

TEST(Context, CreateWithoutContext) {
    EXPECT_THROW(Context::create(), InitError);
}

TEST(Context, WithoutContextErrorNamesTheFix) {
    try {
        Context::create();
        FAIL() << "Context::create() should have thrown";
    } catch (const InitError& error) {
        EXPECT_NE(std::string(error.what()).find("must be called after"), std::string::npos);
    }
}

TEST_F(ContextTest, CreateContext) {
    EXPECT_NO_THROW(Context::create());
}

TEST_F(ContextTest, RendererInfo) {
    EXPECT_EQ(Context::getRendererInfo().rfind("OpenGL ", 0), 0);
}

/* Disabled because it hangs: without a context glGetError() never stops returning */
TEST(GLFramebuffer, DISABLED_CreateFrameBufferWithoutContext) {
    EXPECT_THROW(GLFramebuffer(800, 600), std::runtime_error);
}

TEST_F(ContextTest, CreateFrameBuffer) {
    const GLFramebuffer framebuffer(800, 600);
    EXPECT_NE(framebuffer.handle(), 0u);
    EXPECT_EQ(framebuffer.width(), 800);
    EXPECT_EQ(framebuffer.height(), 600);
}

TEST_F(ContextTest, CreateFrameBufferWithBadSize) {
    EXPECT_THROW(GLFramebuffer(-1, 600), std::runtime_error);
    EXPECT_THROW(GLFramebuffer(800, -1), std::runtime_error);
    EXPECT_THROW(GLFramebuffer(0, 600), std::runtime_error);
    EXPECT_THROW(GLFramebuffer(800, 0), std::runtime_error);
}

TEST_F(ContextTest, ResizeFrameBufferWithBadSize) {
    GLFramebuffer framebuffer(800, 600);
    EXPECT_THROW(framebuffer.resize(-1, 600), std::runtime_error);
    EXPECT_EQ(framebuffer.width(), 800);
    EXPECT_EQ(framebuffer.height(), 600);
}

TEST(EffectPipeline, CreateEffectPipelineWithoutContext) {
    const EffectPipeline pipeline(800, 600);
    EXPECT_EQ(pipeline.size(), 0);
}

TEST_F(ContextTest, CreateEffectPipeline) {
    const EffectPipeline pipeline(800, 600);
    EXPECT_EQ(pipeline.size(), 0);
    EXPECT_EQ(pipeline.getWidth(), 800);
    EXPECT_EQ(pipeline.getHeight(), 600);
}

TEST_F(ContextTest, AddEffect) {
    EffectPipeline pipeline(800, 600);
    EXPECT_EQ(pipeline.size(), 0);
    pipeline.add<Distortion>();
    EXPECT_EQ(pipeline.size(), 1);
}

TEST_F(ContextTest, GetEffect) {
    EffectPipeline pipeline(800, 600);
    pipeline.add<Distortion>();
    const auto& effect = pipeline.get<Distortion>();
    EXPECT_EQ(&effect, &pipeline.get<Distortion>());
}

TEST_F(ContextTest, GetEffectNotFound) {
    EffectPipeline pipeline(800, 600);
    EXPECT_THROW(pipeline.get<Distortion>(), std::out_of_range);
}

TEST_F(ContextTest, GetEffectOtherType) {
    EffectPipeline pipeline(800, 600);
    pipeline.add<Vignette>();
    EXPECT_THROW(pipeline.get<Distortion>(), std::out_of_range);
}

TEST_F(ContextTest, GetEffectBadIndex) {
    EffectPipeline pipeline(800, 600);
    pipeline.add<Distortion>().add<Distortion>();
    EXPECT_NE(&pipeline.get<Distortion>(0), &pipeline.get<Distortion>(1));
    EXPECT_THROW(pipeline.get<Distortion>(2), std::out_of_range);
}

TEST_F(ContextTest, RenderWithBadSize) {
    const GLFramebuffer input(kWidth, kHeight);
    EffectPipeline pipeline(-1, kHeight);
    pipeline.add<Vignette>();
    EXPECT_THROW(pipeline.render(input.color()), std::runtime_error);
}

TEST_F(ContextTest, CreateHostTarget) {
    const auto scene = makeHostTarget(kWidth, kHeight);
    EXPECT_NE(scene->getColor().getHandle(), 0u);
    EXPECT_EQ(scene->getWidth(), kWidth);
    EXPECT_EQ(scene->getHeight(), kHeight);
}

TEST_F(ContextTest, ResizeHostTarget) {
    const auto scene = makeHostTarget(kWidth, kHeight);
    scene->resize(kWidth / 2, kHeight / 2);
    EXPECT_NE(scene->getColor().getHandle(), 0u);
    EXPECT_EQ(scene->getWidth(), kWidth / 2);
    EXPECT_EQ(scene->getHeight(), kHeight / 2);
}

TEST_F(ContextTest, RenderHostTargetWithoutEffect) {
    const auto scene = makeHostTarget(kWidth, kHeight);
    EffectPipeline pipeline(kWidth, kHeight);
    scene->begin();
    scene->end();
    EXPECT_NO_THROW(pipeline.render(*scene));
}

TEST_F(ContextTest, RenderHostTargetWithOneEffect) {
    const auto scene = makeHostTarget(kWidth, kHeight);
    EffectPipeline pipeline(kWidth, kHeight);
    pipeline.add<Vignette>();
    scene->begin();
    scene->end();
    EXPECT_NO_THROW(pipeline.render(*scene));
}

TEST_F(ContextTest, RenderHostTargetWithEffectChain) {
    const auto scene = makeHostTarget(kWidth, kHeight);
    EffectPipeline pipeline(kWidth, kHeight);
    pipeline.add<Vignette>().add<Distortion>().add<Bloom>();
    scene->begin();
    scene->end();
    EXPECT_NO_THROW(pipeline.render(*scene));
}

TEST_F(ContextTest, RenderAfterResize) {
    const auto scene = makeHostTarget(kWidth, kHeight);
    EffectPipeline pipeline(kWidth, kHeight);
    pipeline.add<Vignette>().add<Bloom>();
    scene->begin();
    scene->end();
    pipeline.render(*scene);

    scene->resize(kWidth / 2, kHeight / 2);
    pipeline.resize(kWidth / 2, kHeight / 2);
    scene->begin();
    scene->end();
    EXPECT_NO_THROW(pipeline.render(*scene));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
