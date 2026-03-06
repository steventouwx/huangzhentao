// SPDX-License-Identifier: MediaTekProprietary
#pragma once

#include <android/binder_interface_utils.h>
#include <android/binder_manager.h>
#include <binder/ProcessState.h>
#include <map>
#include <memory>
#include <unordered_map> // Include the correct header for unordered_map
#include "radio_service.h"
#include "RfxDispatchThread.h"

class RadioServiceProxy {
private:
  // static std::unique_ptr<RadioServiceProxy> sInstance;
  RadioServiceProxy() = default;
  std::unordered_map<int, std::shared_ptr<RadioService>> mRadioNetwork;
  std::unordered_map<int, std::shared_ptr<RadioService>> mMtkRadioExNetwork;

  std::unordered_map<int, std::shared_ptr<RadioService>> mRadioModem;
  std::unordered_map<int, std::shared_ptr<RadioService>> mMtkRadioExModem;

  std::unordered_map<int, std::shared_ptr<RadioService>> mRadioSim;
  std::unordered_map<int, std::shared_ptr<RadioService>> mMtkRadioExSim;

  std::unordered_map<int, std::shared_ptr<RadioService>> mRadioData;
  std::unordered_map<int, std::shared_ptr<RadioService>> mMtkRadioExData;

  std::unordered_map<int, std::shared_ptr<RadioService>> mRadioIms;
  std::unordered_map<int, std::shared_ptr<RadioService>> mMtkRadioExIms;

  std::unordered_map<int, std::shared_ptr<RadioService>> mRadioMessaging;
  std::unordered_map<int, std::shared_ptr<RadioService>> mMtkRadioExMessaging;

  std::unordered_map<int, std::shared_ptr<RadioService>> mRadioVoice;
  std::unordered_map<int, std::shared_ptr<RadioService>> mMtkRadioExVoice;

  std::unordered_map<int, std::shared_ptr<RadioService>> mMtkRadioExEcall;

  std::unordered_map<int, std::shared_ptr<RadioService>> mRadioConfig;
  std::unordered_map<int, std::unordered_map<int, std::shared_ptr<RadioService>>> mRadios;//tag, slot, service
  RadioServiceProxy(const RadioServiceProxy &) = delete;
  RadioServiceProxy &operator=(const RadioServiceProxy &) = delete;

public:
  static RadioServiceProxy &getInstance();
  void init(RfxDispatchThread* dispatchThread);
  std::shared_ptr<RadioService> getProxy(int hal, int slot);
  bool isAidlReady();

public:
  static constexpr int HAL_SERVICE_RADIO = 0;
  /**
   * HAL service type that supports the HAL APIs implementation of IRadioData
   */
  static constexpr int HAL_SERVICE_DATA = 1;

  /**
   * HAL service type that supports the HAL APIs implementation of
   * IRadioMessaging
   */
  static constexpr int HAL_SERVICE_MESSAGING = 2;

  /**
   * HAL service type that supports the HAL APIs implementation of IRadioModem
   */
  static constexpr int HAL_SERVICE_MODEM = 3;

  /**
   * HAL service type that supports the HAL APIs implementation of IRadioNetwork
   */
  static constexpr int HAL_SERVICE_NETWORK = 4;

  /**
   * HAL service type that supports the HAL APIs implementation of IRadioSim
   */
  static constexpr int HAL_SERVICE_SIM = 5;

  /**
   * HAL service type that supports the HAL APIs implementation of IRadioVoice
   */
  static constexpr int HAL_SERVICE_VOICE = 6;

  /**
   * HAL service type that supports the HAL APIs implementation of IRadioIms
   */
  static constexpr int HAL_SERVICE_IMS = 7;

  static constexpr int HAL_SERVICE_RADIO_EX = 8;
  /**
   * HAL service type that supports the HAL APIs implementation of
   * IMtkRadioExData
   */
  static constexpr int HAL_SERVICE_DATA_EX = 9;

  /**
   * HAL service type that supports the HAL APIs implementation of
   * IMtkRadioExMessaging
   */
  static constexpr int HAL_SERVICE_MESSAGING_EX = 10;

  /**
   * HAL service type that supports the HAL APIs implementation of
   * IMtkRadioExModem
   */
  static constexpr int HAL_SERVICE_MODEM_EX = 11;

  /**
   * HAL service type that supports the HAL APIs implementation of
   * IMtkRadioExNetwork
   */
  static constexpr int HAL_SERVICE_NETWORK_EX = 12;

  /**
   * HAL service type that supports the HAL APIs implementation of
   * IMtkRadioExSim
   */
  static constexpr int HAL_SERVICE_SIM_EX = 13;

  /**
   * HAL service type that supports the HAL APIs implementation of
   * IMtkRadioExVoice
   */
  static constexpr int HAL_SERVICE_VOICE_EX = 14;

  /**
   * HAL service type that supports the HAL APIs implementation of
   * IMtkRadioExIms
   */
  static constexpr int HAL_SERVICE_IMS_EX = 15;

  /**
   * HAL service type that supports the HAL APIs implementation of
   * IMtkRadioExEcall
   */
  static constexpr int HAL_SERVICE_ECALL_EX = 16;

  /**
   * HAL service type that supports the HAL APIs implementation of IRadioConfig
   */
  static constexpr int HAL_SERVICE_CONFIG = 17;
  static constexpr int HAL_SERVICE_MAX = 18;
};
