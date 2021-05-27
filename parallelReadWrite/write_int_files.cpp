#include <bits/stdc++.h>
#include <thread>

const char *USAGE = R"(
Purpose:
    - Write integers to a file in parallel
Usage:
    ./binary <num_integers> <file_prefix> <num_cores>

)";

std::tuple<uint32_t, std::string, uint16_t> parseArgs(int argc, char **argv) {
    if (argc < 4) {
        std::cerr << USAGE << std::endl;
        exit(1);
    }

    // const auto processor_count = std::thread::hardware_concurrency();
    try {
        return {std::stoi(argv[1]), std::string(argv[2]), std::stoi(argv[3])};
    } catch (const std::invalid_argument &ia) {
        std::cerr << USAGE << std::endl << "Invalid argument." << std::endl;
        std::cerr << "WHAT? " << ia.what() << std::endl;
        exit(1);
    }
}

int writeFile(uint16_t process_num, uint32_t num_integers,
              std::string file_prefix) {

    std::stringstream filename;
    filename << file_prefix << "_" << process_num << ".data";
    std::ofstream file(filename.str());
    if (file.is_open()) {
        std::cout << "Opened file: " << filename.str() << std::endl;
        for (uint64_t ii = 0; ii < num_integers; ii++) {
            file << ii << "\n";
        }
        std::cout << "Done writing file: " << filename.str() << std::endl;
    }
    // std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}

void runThreads(uint16_t num_processes, uint32_t num_integers,
                std::string file_prefix) {
    std::vector<std::thread> threads;
    for (int ii = 0; ii < num_processes; ii++) {
        // std::ref(nums); // for passing ref into thread.
        threads.push_back(
            std::thread(writeFile, ii, num_integers, file_prefix));
        // t.join();
    }

    int ii = 0;
    for (std::thread &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

int main(int argc, char **argv) {
    auto [num_integers, file_prefix, num_cores] = parseArgs(argc, argv);

    std::cout << "Writing " << num_integers << " integers into " << num_cores
              << " files, with file prefix '" << file_prefix << "'"
              << std::endl;

    runThreads(num_cores, num_integers, file_prefix);
    return 0;
}

