#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <random>
#include <filesystem>
#include <chrono>
#include <algorithm>
#include <functional>

class Benchmarks {
private:
    std::vector<uint8_t> data;

public:
    // Constructor for initializing the data array with 100 MB
    Benchmarks() : data(100 * 1024 * 1024) {
        Setup();
    }

    // Setup method to initialize data with random bytes
    void Setup() {
        std::mt19937 generator(1729);
        std::uniform_int_distribution<int> distribution(0, 255);
        for (auto &byte : data) {
            byte = static_cast<uint8_t>(distribution(generator));
        }
    }

    // CPU benchmark: calculates an approximation of π using a series
    double Cpu() {
        double pi = 0.0;
        for (int i = 1; i <= 500'000'000; ++i) {
            pi += 1.0 / (i * i);
        }
        pi = std::sqrt(pi * 6);
        return pi;
    }

    // Disk benchmark: write and delete a temporary file multiple times
    void Disk() {
        for (int i = 0; i < 10; ++i) {
            std::string fileName = std::filesystem::temp_directory_path() / "tempfile.bin";
            std::ofstream file(fileName, std::ios::binary);
            file.write(reinterpret_cast<const char*>(data.data()), data.size());
            file.close();
            std::filesystem::remove(fileName);
        }
    }

    // Memory benchmark: perform random access and sum values in the data array
    int Memory() {
        std::mt19937 generator(1729);
        std::uniform_int_distribution<size_t> distribution(0, data.size() - 1);

        int sum = 0;
        for (int i = 0; i < 20'000'000; ++i) {
            sum += data[distribution(generator)];
        }
        return sum;
    }
};

void runBenchmark(const std::string& name, std::function<void()> func) {
    constexpr int NUM_RUNS = 100;
    std::vector<double> times;
    times.reserve(NUM_RUNS);

    for (int i = 0; i < NUM_RUNS; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        times.push_back(elapsed.count());
    }

    double minTime = *std::min_element(times.begin(), times.end());
    double maxTime = *std::max_element(times.begin(), times.end());
    double avgTime = std::accumulate(times.begin(), times.end(), 0.0) / NUM_RUNS;

    double variance = 0.0;
    for (const auto& t : times) {
        variance += (t - avgTime) * (t - avgTime);
    }
    variance /= NUM_RUNS;
    double stdDev = std::sqrt(variance);

    std::cout << name << ","
            << minTime << ","
            << maxTime << ","
            << avgTime << ","
            << stdDev << "\n";
}

int main() {
    Benchmarks benchmarks;

    std::cout << "benchmark_name, min time, max time, avg_time, std_dev\n";

    runBenchmark("CPU", [&benchmarks]() { benchmarks.Cpu(); });
    runBenchmark("Disk", [&benchmarks]() { benchmarks.Disk(); });
    runBenchmark("Memory", [&benchmarks]() { benchmarks.Memory(); });

    return 0;
}