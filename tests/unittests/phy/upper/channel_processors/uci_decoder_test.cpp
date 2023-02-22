/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

/// \file
/// \brief UCI decoder unit test.
///
/// Detects short messages from sequences of log-likelihood ratios. Both the LLR sequences and the corresponding
/// messages (for comparison purposes) are provided as test vectors.

#include "uci_decoder_test_data.h"
#include "srsran/phy/upper/channel_coding/channel_coding_factories.h"
#include "srsran/phy/upper/channel_processors/channel_processor_factories.h"
#include "srsran/support/srsran_test.h"

/// \cond
using namespace srsran;

int main()
{
  std::shared_ptr<short_block_detector_factory> short_block_detector_factory = create_short_block_detector_factory_sw();
  TESTASSERT(short_block_detector_factory);

  uci_decoder_factory_sw_configuration uci_decoder_factory_sw_config;
  uci_decoder_factory_sw_config.decoder_factory = short_block_detector_factory;
  std::shared_ptr<uci_decoder_factory> uci_decoder_factory =
      create_uci_decoder_factory_sw(uci_decoder_factory_sw_config);
  TESTASSERT(uci_decoder_factory);

  std::unique_ptr<uci_decoder> decoder = uci_decoder_factory->create();
  TESTASSERT(decoder);

  for (const auto& test_data : uci_decoder_test_data) {
    uci_decoder::configuration dec_cfg = {};
    dec_cfg.modulation                 = test_data.config.modulation;

    const std::vector<uint8_t> message = test_data.message.read();
    TESTASSERT_EQ(message.size(), test_data.message_length, "Error reading message.");
    std::vector<log_likelihood_ratio> llr = test_data.llr.read();
    TESTASSERT_EQ(llr.size(), test_data.llr_length, "Error reading LLRs.");

    std::vector<uint8_t> message_test(message.size());
    uci_status           status = decoder->decode(message_test, llr, dec_cfg);
    TESTASSERT_NEQ(status, uci_status::unknown, "Decoder in an unknown status.");
    TESTASSERT_NEQ(status, uci_status::invalid, "Invalid detection.");
    TESTASSERT(std::equal(message_test.cbegin(), message_test.cend(), message.cbegin()), "Detection went wrong.");
  }
}
/// \endcond
