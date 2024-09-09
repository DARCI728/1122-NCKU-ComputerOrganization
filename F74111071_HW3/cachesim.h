// See LICENSE for license details.

#ifndef _RISCV_CACHE_SIM_H
#define _RISCV_CACHE_SIM_H

#include <cstdint>
#include <cstring>
#include <map>
#include <queue>
#include <string>

#include "common.h"
#include "memtracer.h"

class cache_sim_t {
   public:
    cache_sim_t(size_t sets, size_t ways, size_t linesz, const char* name);
    cache_sim_t(const cache_sim_t& rhs);
    virtual ~cache_sim_t();

    void access(uint64_t addr, size_t bytes, bool store);
    void clean_invalidate(uint64_t addr, size_t bytes, bool clean, bool inval);
    void print_stats();
    void set_miss_handler(cache_sim_t* mh) { miss_handler = mh; }
    void set_log(bool _log) { log = _log; }

    static cache_sim_t* construct(const char* config, const char* name);

   protected:
    static const uint64_t VALID = 1ULL << 63;
    static const uint64_t DIRTY = 1ULL << 62;

    virtual uint64_t* check_tag(uint64_t addr);
    virtual uint64_t victimize(uint64_t addr);

    cache_sim_t* miss_handler;

    size_t sets;       // how many cache entries
    size_t ways;       // how many blocks in an entry
    size_t linesz;     // block size
    size_t idx_shift;  // offset

    size_t* way_idx;  // record way index of each set (which way to replace)
    uint64_t* tags;

    uint64_t read_accesses;
    uint64_t read_misses;
    uint64_t bytes_read;
    uint64_t write_accesses;
    uint64_t write_misses;
    uint64_t bytes_written;
    uint64_t writebacks;

    std::string name;
    bool log;

    void init();
};

class fa_cache_sim_t : public cache_sim_t {
   public:
    fa_cache_sim_t(size_t ways, size_t linesz, const char* name);
    uint64_t* check_tag(uint64_t addr);
    uint64_t victimize(uint64_t addr);
};

class cache_memtracer_t : public memtracer_t {
   public:
    cache_memtracer_t(const char* config, const char* name) {
        cache = cache_sim_t::construct(config, name);
    }
    ~cache_memtracer_t() {
        delete cache;
    }
    void set_miss_handler(cache_sim_t* mh) {
        cache->set_miss_handler(mh);
    }
    void clean_invalidate(uint64_t addr, size_t bytes, bool clean, bool inval) {
        cache->clean_invalidate(addr, bytes, clean, inval);
    }
    void set_log(bool log) {
        cache->set_log(log);
    }
    void print_stats() {
        cache->print_stats();
    }

   protected:
    cache_sim_t* cache;
};

class icache_sim_t : public cache_memtracer_t {
   public:
    icache_sim_t(const char* config, const char* name = "I$")
        : cache_memtracer_t(config, name) {}
    bool interested_in_range(uint64_t UNUSED begin, uint64_t UNUSED end, access_type type) {
        return type == FETCH;
    }
    void trace(uint64_t addr, size_t bytes, access_type type) {
        if (type == FETCH) cache->access(addr, bytes, false);
    }
};

class dcache_sim_t : public cache_memtracer_t {
   public:
    dcache_sim_t(const char* config, const char* name = "D$")
        : cache_memtracer_t(config, name) {}
    bool interested_in_range(uint64_t UNUSED begin, uint64_t UNUSED end, access_type type) {
        return type == LOAD || type == STORE;
    }
    void trace(uint64_t addr, size_t bytes, access_type type) {
        if (type == LOAD || type == STORE) cache->access(addr, bytes, type == STORE);
    }
};

#endif