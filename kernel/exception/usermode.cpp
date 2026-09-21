#include <stdint.h>

#include <arch/arch.h>
#include <arch/exception_context.h>

#include <kernel/usermode.h>

int64_t jump_to_user_mode(uint64_t entry, uint64_t stack) {
    arch::jump_to_user_mode(entry, stack);

    return 0;
}