/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "../../../lib/phy/upper/uplink_request_processor_impl.h"
#include "../support/prach_buffer_test_doubles.h"
#include "upper_phy_rx_symbol_request_notifier_test_doubles.h"
#include "srsgnb/phy/support/prach_buffer_context.h"
#include "srsgnb/phy/support/support_factories.h"
#include <gtest/gtest.h>

using namespace srsgnb;

namespace {

TEST(UplinkRequestProcessor, process_prach_request_produces_event_that_request_capture_prach)
{
  upper_phy_rx_symbol_request_notifier_spy   symbol_notifier;
  std::vector<std::unique_ptr<prach_buffer>> config_prach_pool;
  config_prach_pool.push_back(std::make_unique<prach_buffer_spy>());
  std::unique_ptr<prach_buffer_pool> prach_pool = create_prach_buffer_pool(std::move(config_prach_pool));
  uplink_request_processor_impl      ul_request_processor(symbol_notifier, *prach_pool);
  prach_buffer_context               context;
  context.slot   = slot_point(0, 0, 0);
  context.sector = 0;

  ASSERT_FALSE(symbol_notifier.has_prach_result_been_notified());

  ul_request_processor.process_prach_request(context);

  ASSERT_TRUE(symbol_notifier.has_prach_result_been_notified());
}

} // namespace