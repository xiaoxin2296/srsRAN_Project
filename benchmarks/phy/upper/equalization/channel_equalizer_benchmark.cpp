/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "srsgnb/phy/upper/channel_estimation.h"
#include "srsgnb/phy/upper/equalization/equalization_factories.h"
#include "srsgnb/phy/upper/re_measurement.h"
#include "srsgnb/ran/cyclic_prefix.h"
#include "srsgnb/ran/spatial_topology.h"
#include "srsgnb/support/benchmark_utils.h"
#include "srsgnb/support/srsgnb_test.h"
#include <complex>
#include <getopt.h>
#include <random>

using namespace srsgnb;

// Random generator.
static std::mt19937 rgen(0);

static unsigned nof_repetitions = 1000;
static unsigned nof_prb         = 106;
static bool     silent          = false;

static void usage(const char* prog)
{
  fmt::print("Usage: {} [-R repetitions] [-s silent]\n", prog);
  fmt::print("\t-R Repetitions [Default {}]\n", nof_repetitions);
  fmt::print("\t-s Toggle silent operation [Default {}]\n", silent);
  fmt::print("\t-h Show this message\n");
}

static void parse_args(int argc, char** argv)
{
  int opt = 0;
  while ((opt = getopt(argc, argv, "R:sh")) != -1) {
    switch (opt) {
      case 'R':
        nof_repetitions = std::strtol(optarg, nullptr, 10);
        break;
      case 's':
        silent = (!silent);
        break;
      case 'h':
      default:
        usage(argv[0]);
        exit(0);
    }
  }
}

int main(int argc, char** argv)
{
  parse_args(argc, argv);

  std::shared_ptr<channel_equalizer_factory> equalizer_factory = create_channel_equalizer_factory_zf();
  TESTASSERT(equalizer_factory);

  std::unique_ptr<channel_equalizer> equalizer = equalizer_factory->create();
  TESTASSERT(equalizer);

  // Channel estimate distributions.
  std::uniform_real_distribution<float> ch_mag_dist(0.1F, 1.0F);
  std::uniform_real_distribution<float> ch_phase_dist(0.0F, TWOPI);

  // Symbol distribution.
  std::uniform_real_distribution<float> symbol_dist(-1.0F, 1.0F);

  static_re_measurement<cf_t, MAX_RB * NRE, MAX_NSYMB_PER_SLOT, MAX_PORTS>                        rx_symbols;
  static_re_measurement<cf_t, MAX_RB * NRE, MAX_NSYMB_PER_SLOT, pusch_constants::MAX_NOF_LAYERS>  eq_symbols;
  static_re_measurement<float, MAX_RB * NRE, MAX_NSYMB_PER_SLOT, pusch_constants::MAX_NOF_LAYERS> eq_noise_vars;
  channel_estimate                                                                                ch_est;

  benchmarker perf_meas("Channel Equalizer", nof_repetitions);

  for (spatial_topology topology : {spatial_topology::siso,
                                    spatial_topology::miso_2x1,
                                    spatial_topology::miso_3x1,
                                    spatial_topology::miso_4x1,
                                    spatial_topology::mimo_2x2}) {
    // Get dimensions.
    unsigned nof_rx_ports     = topology.get_nof_rx_ports();
    unsigned nof_tx_layers    = topology.get_nof_tx_layers();
    unsigned nof_ofdm_symbols = MAX_NSYMB_PER_SLOT;
    unsigned nof_subcarriers  = nof_prb * NRE;

    re_measurement_dimensions rx_symbol_dims = {nof_subcarriers, nof_ofdm_symbols, nof_rx_ports};
    re_measurement_dimensions tx_symbol_dims = {nof_subcarriers, nof_ofdm_symbols, nof_tx_layers};

    // Resize data structures.
    rx_symbols.resize(rx_symbol_dims);
    eq_symbols.resize(tx_symbol_dims);
    eq_noise_vars.resize(tx_symbol_dims);

    // Resize channel estimates.
    channel_estimate::channel_estimate_dimensions ch_dims;
    ch_dims.nof_prb       = nof_prb;
    ch_dims.nof_symbols   = nof_ofdm_symbols;
    ch_dims.nof_rx_ports  = nof_rx_ports;
    ch_dims.nof_tx_layers = nof_tx_layers;
    ch_est.resize(ch_dims);

    for (unsigned i_rx_port = 0; i_rx_port != nof_rx_ports; ++i_rx_port) {
      // Generate Rx symbols.
      span<cf_t> symbols = rx_symbols.get_slice(i_rx_port);
      std::generate(symbols.begin(), symbols.end(), [&rgen = rgen, &symbol_dist]() {
        return cf_t(symbol_dist(rgen), symbol_dist(rgen));
      });

      for (unsigned i_tx_layer = 0; i_tx_layer != nof_tx_layers; ++i_tx_layer) {
        // Generate estimates.
        span<cf_t> ests = ch_est.get_path_ch_estimate(i_rx_port, i_tx_layer);
        std::generate(ests.begin(), ests.end(), [&rgen = rgen, &ch_mag_dist, &ch_phase_dist]() {
          return std::polar(ch_mag_dist(rgen), ch_phase_dist(rgen));
        });

        // Set the noise variance.
        ch_est.set_noise_variance(0.1F, i_rx_port, i_tx_layer);
      }
    }

    // Number of equalized resource elements.
    unsigned nof_processed_re = nof_subcarriers * nof_ofdm_symbols * nof_tx_layers;

    // Measurement description.
    std::string meas_descr = "ZF " + topology.to_string();

    // Equalize.
    perf_meas.new_measure(
        meas_descr, nof_processed_re, [&eq_symbols, &eq_noise_vars, &rx_symbols, &ch_est, &equalizer]() {
          equalizer->equalize(eq_symbols, eq_noise_vars, rx_symbols, ch_est, 1.0F);
        });
  }

  if (!silent) {
    perf_meas.print_percentiles_throughput("symbols");
  }

  return 0;
}