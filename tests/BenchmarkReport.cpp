#include <nlohmann/json.hpp>
#include <fstream>
#include "BenchmarkReport.hpp"

using json = nlohmann::json;

BenchmarkReport &BenchmarkReport::setGpu(const std::string& gpu) {
    m_gpu = gpu;
    return *this;
}

BenchmarkReport &BenchmarkReport::setBackend(const std::string& backend) {
    m_backend = backend;
    return *this;
}

BenchmarkReport &BenchmarkReport::setEffects(const std::string& effects) {
    m_effects = effects;
    return *this;
}

BenchmarkReport &BenchmarkReport::setAvgFps(double avgFps) {
    m_avgFps = avgFps;
    return *this;
}

BenchmarkReport &BenchmarkReport::setTotalMs(double totalMs) {
    m_totalMs = totalMs;
    return *this;
}

BenchmarkReport &BenchmarkReport::setFrames(double frames) {
    m_frames = frames;
    return *this;
}

BenchmarkReport &BenchmarkReport::setVramUsed(GLint vramUsed) {
    m_vramUsed = vramUsed;
    return *this;
}

bool BenchmarkReport::exceedsDeviation(double baseline, double current, double threshold) {
    double gap = std::abs(current - baseline) / baseline;
    if (gap > threshold) {
        return true;
    }
    return false;
}

void BenchmarkReport::save(const std::string& path) {
    json results;

    if (m_gpu.contains("llvmpipe")) {
        return;
    }

    {
        std::ifstream existing(path);
        if (existing.is_open()) {
            try {
                existing >> results;
            }
            catch (...) {
                results = json::object();
            }
        }
    }

    json entry;

    if (results.contains(m_gpu) && results[m_gpu].contains(m_backend) && results[m_gpu][m_backend].contains(m_effects)) {
        const json& tmp = results[m_gpu][m_backend][m_effects];
        entry = tmp;
        if (exceedsDeviation(tmp["avgFps"].get<double>(), m_avgFps, 0.10)) {
            entry["avgFps"]  = m_avgFps;
            entry["totalMs"] = m_totalMs;
        }
    } else {
        entry["avgFps"]  = m_avgFps;
        entry["totalMs"] = m_totalMs;
    }

    entry["frames"]  = m_frames;
    entry["vramUsed"]  = m_vramUsed;

    if (!results.contains(m_gpu)) {
        results[m_gpu] = json::object();
    }

    results[m_gpu][m_backend][m_effects] = entry;

    std::ofstream file(path);
    file << results.dump(4);
}