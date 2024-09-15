#include <cstdint>
#include <stdint.h>

// TLB Model
typedef uintptr_t vaddr_t;
typedef uintptr_t paddr_t;

// Flag to mark an entry in the modelled hardware TLB as having been set for use as a valid translation.

#define TLB_ENTRY_FLAGS_INUSE

struct tlb_cache_record_t {
    vaddr_t entry_virtual_address;
    paddr_t relevant_physical_address;
    uint16_t permissions;
};

