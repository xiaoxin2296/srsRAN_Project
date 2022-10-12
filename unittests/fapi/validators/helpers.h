/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

#include "srsgnb/adt/expected.h"
#include "srsgnb/fapi/validator_report.h"
#include <gtest/gtest.h>

namespace unittest {

/// Class that defines the vector test for the validators.
template <typename T>
class test_group
{
public:
  // Auxiliary struct.
  struct data {
    unsigned value;
    bool     result;
  };

  // Aliases for the iterators.
  using iterator       = data*;
  using const_iterator = const data*;

  // Iterators.
  iterator       begin() { return &*values.begin(); }
  iterator       end() { return &*values.end(); }
  const_iterator begin() const { return &*values.cbegin(); }
  const_iterator end() const { return &*values.cend(); }

  test_group(std::function<void(T& pdu, int value)> f, const char* prop, std::vector<data> values) :
    f(std::move(f)), prop(prop), values(std::move(values))
  {
  }

  /// Configure the Next case of the test case. Returns the test result.
  void update_msg(T& pdu, int value) const { f(pdu, value); }

  /// Returns the property name.
  const char* property() const { return prop; }

private:
  std::function<void(T& pdu, int value)> f;
  const char*                            prop = "";
  std::vector<data>                      values;
};

/// Builds and returns a valid CRC.indication message. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.8.
srsgnb::fapi::crc_indication_message build_valid_crc_indication();

/// Builds and returns a valid DL CSI-RS pdu. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.2.3.
srsgnb::fapi::dl_csi_rs_pdu build_valid_dl_csi_pdu();

/// Builds and returns a valid DL PDCCH PDU. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.2.1.
srsgnb::fapi::dl_pdcch_pdu build_valid_dl_pdcch_pdu();

/// Builds and returns a valid DL PDSCH PDU. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.2.2.
srsgnb::fapi::dl_pdsch_pdu build_valid_dl_pdsch_pdu();

/// Builds and returns a valid DL SSB pdu. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.2.4.
srsgnb::fapi::dl_ssb_pdu build_valid_dl_ssb_pdu();

/// Builds and returns a valid DL TTI request message. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.2.
srsgnb::fapi::dl_tti_request_message build_valid_dl_tti_request();

/// Builds and returns a valid ERROR.indication message. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.3.6.1.
srsgnb::fapi::error_indication_message build_valid_error_indication();

/// Builds and returns a valid OUT_OF_SYNC ERROR.indication message. Every parameter is within the range defined in
/// SCF-222 v4.0 Section 3.3.6.1.
srsgnb::fapi::error_indication_message build_valid_out_of_sync_error_indication();

/// Builds and returns a RACH.indication message. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.11.
srsgnb::fapi::rach_indication_message build_valid_rach_indication();

/// Builds and returns a valid Rx_Data.indication message. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.7.
srsgnb::fapi::rx_data_indication_message build_valid_rx_data_indication();

/// Builds and returns a valid SLOT.indication message. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.1.
srsgnb::fapi::slot_indication_message build_valid_slot_indication();

/// Builds and returns a valid Tx_Data.request message. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.6.
/// \note Payload pointer is set to \c nullptr.
srsgnb::fapi::tx_data_request_message build_valid_tx_data_request();

/// Builds and returns a valid UCI.indication message. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.9.
srsgnb::fapi::uci_indication_message build_valid_uci_indication();

/// Builds and returns a valid UCI PUSCH PDU. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.9.1.
srsgnb::fapi::uci_pusch_pdu build_valid_uci_pusch_pdu();

/// Builds and returns a valid UCI PUCCH format 0/1 PDU. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.9.2.
srsgnb::fapi::uci_pucch_pdu_format_0_1 build_valid_uci_pucch_format01_pdu();

/// Builds and returns a valid UCI PUCCH format 2/3/4 PDU. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.9.3.
srsgnb::fapi::uci_pucch_pdu_format_2_3_4 build_valid_uci_pucch_format234_pdu();

/// Builds and returns a valid UL_DCI.request message. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.4.
srsgnb::fapi::ul_dci_request_message build_valid_ul_dci_request();

/// Builds and returns a valid UL PRACH PDU. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.3.1.
srsgnb::fapi::ul_prach_pdu build_valid_ul_prach_pdu();

/// Builds and returns a valid UL PUCCH format 0 PDU. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.3.3.
srsgnb::fapi::ul_pucch_pdu build_valid_ul_pucch_f0_pdu();

/// Builds and returns a valid UL PUCCH format 1 PDU. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.3.3.
srsgnb::fapi::ul_pucch_pdu build_valid_ul_pucch_f1_pdu();

/// Builds and returns a valid UL PUCCH format 2 PDU. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.3.3.
srsgnb::fapi::ul_pucch_pdu build_valid_ul_pucch_f2_pdu();

/// Builds and returns a valid UL PUCCH format 3 PDU. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.3.3.
srsgnb::fapi::ul_pucch_pdu build_valid_ul_pucch_f3_pdu();

/// Builds and returns a valid UL PUCCH format 4 PDU. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.3.3.
srsgnb::fapi::ul_pucch_pdu build_valid_ul_pucch_f4_pdu();

/// Builds and returns a valid UL PUSCH PDU. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.3.2.
srsgnb::fapi::ul_pusch_pdu build_valid_ul_pusch_pdu();

/// Builds and returns a valid UL_TTI.request. Every parameter is within the range defined in SCF-222 v4.0
/// Section 3.4.3.
srsgnb::fapi::ul_tti_request_message build_valid_ul_tti_request();

/// Google test helpers.
template <typename T>
struct pdu_field_data {
  std::string                            property;
  std::function<void(T& pdu, int value)> fun;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const pdu_field_data<T>& arg)
{
  os << arg.property;
  return os;
};

struct test_case_data {
  unsigned value;
  bool     result;
};

using validator_report = srsgnb::fapi::validator_report;
using message_type_id  = srsgnb::fapi::message_type_id;
using error_report     = srsgnb::fapi::validator_report::error_report;

inline std::ostream& operator<<(std::ostream& os, const test_case_data& arg)
{
  os << "Value = " << std::to_string(arg.value) << ", Result = " << std::to_string(arg.result);
  return os;
};

template <typename T, typename F>
class ValidateFAPIField
{
public:
  virtual ~ValidateFAPIField() = default;

protected:
  void execute_test(pdu_field_data<T> property, test_case_data params, std::function<T()> builder, F validator)
  {
    T    pdu    = given_the_pdu(property, params, builder);
    bool result = when_executing_the_validation(pdu, validator);
    then_check_the_report(result, property.property, params.result);
  }

private:
  T given_the_pdu(pdu_field_data<T> property, test_case_data params, std::function<T()> builder)
  {
    T pdu = builder();
    property.fun(pdu, params.value);
    return pdu;
  };

  virtual bool when_executing_the_validation(T pdu, F validator) { return true; };

  void then_check_the_report(bool result, const std::string& property, bool expected_result)
  {
    EXPECT_EQ(result, expected_result);
    if (!result) {
      EXPECT_EQ(1U, report.reports.size());
      const auto& rep = report.reports.back();
      EXPECT_EQ(property, rep.property_name);
    } else {
      EXPECT_TRUE(report.reports.empty());
    }
  };

protected:
  validator_report report = {0, 0};
};

using fapi_error = srsgnb::error_type<validator_report>;

template <typename T>
class ValidateFAPIMessage : public ValidateFAPIField<T, std::function<fapi_error(T& pdu)>>
{
  using base = ValidateFAPIField<T, std::function<fapi_error(T& pdu)>>;

public:
  void execute_test(pdu_field_data<T>                 property,
                    test_case_data                    params,
                    std::function<T()>                builder,
                    std::function<fapi_error(T& pdu)> validator,
                    message_type_id                   msg_type_id)
  {
    base::execute_test(property, params, builder, validator);

    check_message_params(params.result, msg_type_id);
  }

private:
  bool when_executing_the_validation(T pdu, std::function<fapi_error(T& pdu)> validator) override
  {
    fapi_error error = validator(pdu);
    if (error.is_error()) {
      base::report = error.error();
    }

    return !error.is_error();
  };

  void check_message_params(bool result, message_type_id msg_type_id) const
  {
    // In case of error, check the message type.
    if (!result) {
      // Base class checks all the parameters but pdu_type, so it gets checked here. Base also checks that only one
      // error report exists, so the last (or the first) of them can be used for the check.
      const error_report& rep = base::report.reports.back();
      EXPECT_EQ(static_cast<unsigned>(msg_type_id), static_cast<unsigned>(rep.message_type));
    }
  }
};

template <typename T, typename U>
class ValidateFAPIPDU : public ValidateFAPIField<T, std::function<bool(T& pdu, srsgnb::fapi::validator_report& report)>>
{
  using base = ValidateFAPIField<T, std::function<bool(T& pdu, srsgnb::fapi::validator_report& report)>>;

public:
  void execute_test(pdu_field_data<T>                                                   property,
                    test_case_data                                                      params,
                    std::function<T()>                                                  builder,
                    std::function<bool(T& pdu, srsgnb::fapi::validator_report& report)> validator,
                    message_type_id                                                     msg_type_id,
                    U                                                                   pdu_type)
  {
    base::execute_test(property, params, builder, validator);

    check_pdu_params(params.result, msg_type_id, pdu_type);
  }

private:
  bool when_executing_the_validation(T pdu, std::function<bool(T& pdu, validator_report& report)> validator) override
  {
    return validator(pdu, base::report);
  };

  void check_pdu_params(bool result, message_type_id msg_type_id, U pdu_type) const
  {
    // In case of error, check the PDU type.
    if (!result) {
      // Base class checks all the parameters but pdu_type, so it gets checked here. Base also checks that only one
      // error report exists, so the last (or the first) of them can be used for the check.
      const auto& rep = base::report.reports.back();
      EXPECT_EQ(static_cast<unsigned>(pdu_type), static_cast<unsigned>(rep.pdu_type.value()));
      EXPECT_EQ(static_cast<unsigned>(msg_type_id), static_cast<unsigned>(rep.message_type));
    }
  }
};

} // namespace unittest
