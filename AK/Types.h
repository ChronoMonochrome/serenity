/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Platform.h>
#include <AK/StdLibExtras.h>

using u64 = __UINT64_TYPE__;
using u32 = __UINT32_TYPE__;
using u16 = __UINT16_TYPE__;
using u8 = __UINT8_TYPE__;
using i64 = __INT64_TYPE__;
using i32 = __INT32_TYPE__;
using i16 = __INT16_TYPE__;
using i8 = __INT8_TYPE__;

#ifndef KERNEL
using f32 = float;
static_assert(__FLT_MANT_DIG__ == 24 && __FLT_MAX_EXP__ == 128);

using f64 = double;
static_assert(__DBL_MANT_DIG__ == 53 && __DBL_MAX_EXP__ == 1024);

#    if __LDBL_MANT_DIG__ == 64 && __LDBL_MAX_EXP__ == 16384
#        define AK_HAS_FLOAT_80 1
using f80 = long double;
#    elif __LDBL_MANT_DIG__ == 113 && __LDBL_MAX_EXP__ == 16384
#        define AK_HAS_FLOAT_128 1
using f128 = long double;
#    endif
#endif

#ifdef AK_OS_SERENITY

using size_t = __SIZE_TYPE__;
using ssize_t = AK::Detail::MakeSigned<size_t>;

using ptrdiff_t = __PTRDIFF_TYPE__;

using intptr_t = __INTPTR_TYPE__;
using uintptr_t = __UINTPTR_TYPE__;

using uint8_t = u8;
using uint16_t = u16;
using uint32_t = u32;
using uint64_t = u64;

using int8_t = i8;
using int16_t = i16;
using int32_t = i32;
using int64_t = i64;

using pid_t = int;

#else
#    include <stddef.h>
#    include <stdint.h>
#    include <sys/types.h>

#    ifdef __ptrdiff_t
using __ptrdiff_t = __PTRDIFF_TYPE__;
#    endif

#    if defined(AK_OS_WINDOWS) && !defined(__CYGWIN__)
using ssize_t = AK::Detail::MakeSigned<size_t>;
using mode_t = unsigned short;
#    endif
#endif

using FlatPtr = AK::Detail::Conditional<sizeof(void*) == 8, u64, u32>;

constexpr u64 KiB = 1024;
constexpr u64 MiB = KiB * KiB;
constexpr u64 GiB = KiB * KiB * KiB;
constexpr u64 TiB = KiB * KiB * KiB * KiB;
constexpr u64 PiB = KiB * KiB * KiB * KiB * KiB;
constexpr u64 EiB = KiB * KiB * KiB * KiB * KiB * KiB;

namespace AK_REPLACED_STD_NAMESPACE { // NOLINT(cert-dcl58-cpp) nullptr_t must be in ::std:: for some analysis tools
using nullptr_t = decltype(nullptr);
}

namespace AK {

using nullptr_t = AK_REPLACED_STD_NAMESPACE::nullptr_t;

static constexpr FlatPtr explode_byte(u8 b)
{
    FlatPtr value = b;
    if constexpr (sizeof(FlatPtr) == 4)
        return value << 24 | value << 16 | value << 8 | value;
    else if (sizeof(FlatPtr) == 8)
        return value << 56 | value << 48 | value << 40 | value << 32 | value << 24 | value << 16 | value << 8 | value;
}

static_assert(explode_byte(0xff) == static_cast<FlatPtr>(0xffffffffffffffffull));
static_assert(explode_byte(0x80) == static_cast<FlatPtr>(0x8080808080808080ull));
static_assert(explode_byte(0x7f) == static_cast<FlatPtr>(0x7f7f7f7f7f7f7f7full));
static_assert(explode_byte(0) == 0);

constexpr size_t align_up_to(const size_t value, const size_t alignment)
{
    return (value + (alignment - 1)) & ~(alignment - 1);
}

constexpr size_t align_down_to(const size_t value, const size_t alignment)
{
    return value & ~(alignment - 1);
}

enum class [[nodiscard]] TriState : u8 {
    False,
    True,
    Unknown
};

enum MemoryOrder {
    memory_order_relaxed = __ATOMIC_RELAXED,
    memory_order_consume = __ATOMIC_CONSUME,
    memory_order_acquire = __ATOMIC_ACQUIRE,
    memory_order_release = __ATOMIC_RELEASE,
    memory_order_acq_rel = __ATOMIC_ACQ_REL,
    memory_order_seq_cst = __ATOMIC_SEQ_CST
};

}

#if USING_AK_GLOBALLY
using AK::align_down_to;
using AK::align_up_to;
using AK::explode_byte;
using AK::MemoryOrder;
using AK::nullptr_t;
using AK::TriState;
#endif
