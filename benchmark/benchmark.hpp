#pragma once

#include <stdint.h>
#include <chrono>
#include <iomanip>
#include <benchmark/benchmark.h>

namespace viper::kv_bm {

static constexpr double SCALE_FACTOR = 1.0;
static constexpr uint64_t NUM_BASE_PREFILLS = 100'000'000;
static constexpr uint64_t NUM_BASE_OPS = 50'000'000;
static constexpr uint64_t NUM_OPS = NUM_BASE_OPS * SCALE_FACTOR;

static constexpr uint64_t NUM_PREFILLS = NUM_BASE_PREFILLS * SCALE_FACTOR;
static constexpr uint64_t NUM_INSERTS = NUM_OPS;
static constexpr uint64_t NUM_UPDATES = NUM_OPS;
static constexpr uint64_t NUM_FINDS = NUM_OPS;
static constexpr uint64_t NUM_DELETES = NUM_OPS;
static constexpr uint64_t MAX_DATA_SIZE = NUM_PREFILLS + NUM_INSERTS;
static constexpr uint64_t NUM_REPETITIONS = 1;
static constexpr uint64_t NUM_MAX_THREADS = 36;
static constexpr uint64_t NUM_UTIL_THREADS = 36;
static constexpr benchmark::TimeUnit BM_TIME_UNIT = benchmark::TimeUnit::kMicrosecond;

template <typename T, size_t N>
struct BMRecord {
    static_assert(N > 0, "N needs to be greater than 0");
    static constexpr size_t total_size = N * sizeof(T);

    BMRecord() {}

    BMRecord(T x) {
        data.fill(x);
    }

    inline bool operator==(const BMRecord& other) const {
        return data == other.data;
    }

    BMRecord<T, N>& from_str(const std::string& bytes) {
        assert(bytes.size() == total_size);
        const char* raw = static_cast<const char*>(bytes.data());
        char* raw_data = static_cast<char*>(static_cast<void*>(data.data()));
        std::copy(raw, raw + total_size, raw_data);
        return *this;
    }

    void update_value() {
        data[0]++;
    }

    std::array<T, N> data;
};

using Offset = uint64_t;

using KeyType8 = BMRecord<uint64_t, 1>;
using KeyType16 = BMRecord<uint64_t, 2>;
using KeyType100 = BMRecord<uint32_t, 25>;
using ValueType8 = BMRecord<uint64_t, 1>;
using ValueType100 = BMRecord<uint32_t, 25>;
using ValueType200 = BMRecord<uint64_t, 25>;
using ValueType900 = BMRecord<uint32_t, 225>;

static constexpr char VIPER_POOL_FILE[] = "/dev/dax0.2";
static constexpr char DB_NVM_DIR[] = "/mnt/nvram-viper/dbfiles";
static constexpr char DB_FILE_DIR[] = "/home/lawrence.benson/dbfiles";
static constexpr char RESULT_FILE_DIR[] = "/home/lawrence.benson/clion/viper/results/";
static constexpr char CONFIG_DIR[] = "/home/lawrence.benson/clion/viper/benchmark/config/";
static const uint64_t BM_POOL_SIZE = (1024l*1024*1024) * 1;  // 1GB

std::string get_time_string();
std::string get_output_file(const std::string& bm_name);
int bm_main(std::vector<std::string> args);

}  // namespace viper::kv_bm