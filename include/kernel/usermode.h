#pragma once

#include <stdint.h>

#include <arch/user_context.h>

int64_t jump_to_user_mode(uint64_t entry, uint64_t stack);