#ifndef SHIMERA_SFML_HPP
#define SHIMERA_SFML_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../../ITest.hpp"
#include "backend/BackendFactory.hpp"
#include "effects/DistortionEffect.hpp"
#include "../../BenchmarkReport.hpp"

class BenchmarkSfml : public ITest {
public:
    BenchmarkSfml() = default;
    BenchmarkSfml(const std::string &testName, sf::RenderWindow &window);
    ~BenchmarkSfml() override = default;

    std::string getName() const override;
    void run() override;
    void setupScene(BenchmarkReport &report);
    void renderScene(float &time);

    private:
        std::string m_name;
        sf::RenderWindow &m_window;
        shimera::IBackend *m_backend;
        shimera::IFrameBuffer *m_sceneFramebuffer;
        shimera::DistortionEffect* m_distortionEffect;
        sf::CircleShape m_triangle;
        sf::RectangleShape m_rectangle;
        sf::CircleShape m_circle;
};

#endif //SHIMERA_SFML_HPP