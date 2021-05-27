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
              std::string file_prefix, std::vector<int> &nums) {
    // This
    std::cout << "I am process_num: " << process_num << std::endl;
    if (process_num == 8) {
        std::cout << "I am process_num 1, sleeping" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    nums[process_num] = process_num * 8;
    return process_num;
}

void test() { std::cout << "This is a test" << std::endl; }

void runThreads(uint16_t num_processes, uint32_t num_integers,
                std::string file_prefix) {
    std::vector<std::thread> threads;
    std::vector<int> nums(num_processes);
    for (int ii = 0; ii < num_processes; ii++) {
        threads.push_back(std::thread(writeFile, ii, num_integers, file_prefix,
                                      std::ref(nums)));
        // t.join();
    }

    int ii = 0;
    for (std::thread &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
        std::cout << "----------------- " << ii++ << " -----------------"
                  << std::endl;
        int jj = 0;
        for (const int num : nums) {
            std::cout << jj++ << " num num: " << num << std::endl;
        }
    }

    for (const int num : nums) {
        std::cout << "num num: " << num << std::endl;
    }
    // this
}

int main(int argc, char **argv) {
    auto [num_integers, file_prefix, num_cores] = parseArgs(argc, argv);

    std::cout << "Writing " << num_integers << " integers into " << num_cores
              << " files, with file prefix '" << file_prefix << "'"
              << std::endl;

    runThreads(num_cores, num_integers, file_prefix);
    return 0;
}

