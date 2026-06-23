#ifndef SHIMERA_BENCHMARKREPORT_HPP
#define SHIMERA_BENCHMARKREPORT_HPP

#include <GL/glew.h>
#include <string>

class BenchmarkReport {
    public:
    BenchmarkReport() = default;
    ~BenchmarkReport() = default;

    BenchmarkReport &setGpu(const std::string& gpu);
    BenchmarkReport &setBackend(const std::string& backend);
    BenchmarkReport &setEffects(const std::string& effects);
    BenchmarkReport &setAvgFps(double avgFps);
    BenchmarkReport &setTotalMs(double totalMs);
    BenchmarkReport &setFrames(double frames);
    BenchmarkReport &setVramUsed(GLint vramUsed);
    bool exceedsDeviation(double baseline, double current, double threshold);

    void save(const std::string& path);

    private:
        std::string m_gpu;
        std::string m_backend;
        std::string m_effects;
        double m_avgFps;
        double m_totalMs;
        double m_frames;
        GLint m_vramUsed;

};

#endif //SHIMERA_BENCHMARKREPORT_H
