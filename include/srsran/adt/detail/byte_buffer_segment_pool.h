/*
 *
 * Copyright 2021-2023 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

#include "srsran/support/memory_pool/fixed_size_memory_block_pool.h"

namespace srsran {
namespace detail {

/// Pool of byte buffer segments.
struct byte_buffer_pool_tag {};
using byte_buffer_segment_pool = fixed_size_memory_block_pool<byte_buffer_pool_tag>;

/// Get default byte buffer segment pool. Initialize pool if not initialized before.
inline detail::byte_buffer_segment_pool& get_default_byte_buffer_segment_pool()
{
  // Initialize byte buffer segment pool, if not yet initialized.
  constexpr static size_t default_byte_buffer_segment_pool_size = 16384;
  constexpr static size_t default_byte_buffer_segment_size      = 1024;
  static auto&            pool = detail::byte_buffer_segment_pool::get_instance(default_byte_buffer_segment_pool_size,
                                                                     default_byte_buffer_segment_size);
  return pool;
}

} // namespace detail

/// Initialize pool of byte buffer segments with specific number of segments. Assert if the pool was already initialized
/// with a different number of segments.
inline void init_byte_buffer_segment_pool(std::size_t nof_segments, std::size_t memory_block_size = 1024)
{
  auto& pool = detail::byte_buffer_segment_pool::get_instance(nof_segments, memory_block_size);
  srsran_assert(nof_segments == pool.nof_memory_blocks(),
                "The pool was already initialized with a different number of segments ({} != {})",
                nof_segments,
                pool.nof_memory_blocks());
  srsran_assert(memory_block_size > 64U, "memory blocks must be larger than the segment control header");
}

} // namespace srsran