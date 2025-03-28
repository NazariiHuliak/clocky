#ifndef EEPROMUTILS_H
#define EEPROMUTILS_H

#include "Arduino.h"
#include <EEPROM.h>
#include <vector>

#include "../src/utils/currency/CurrencyUtils.h"
#include "../src/utils/log/Log.h"

// *****************
// Common EEPROM utils
// *****************
inline String readEEPROM(int address, uint16_t size) {
    if (size == 255 || size > EEPROM_SIZE - address - 1) {
        Log::error("EEPROM: length out of range");
        return "";
    }
    Log::info("EEPROM: Reading at index: ", String(address), " with length: ", String(size));

    String res;
    for (int i = address; i < size + address; ++i) {
        res += static_cast<char>(EEPROM.read(i));
    }

    return res;
}

inline void writeEEPROM(int address, String value, uint16_t size) {
    if (size > EEPROM_SIZE - address - 1) {
        Log::error("EEPROM: length out of range");
        return;
    }
    Log::info("EEPROM: Writing \"", value, "\" at index: ", String(address), " with length: ", String(size));

    for (int i = 0; i < size; ++i) {
        EEPROM.put(address + i, value[i]);
    }
}

inline String lengthAwareReadEEPROM(int address) {
    uint8_t len = EEPROM.read(address);
    return readEEPROM(address + 1, len);
}

inline void lengthAwareWriteEEPROM(int address, String value) {
    int len = value.length();
    if (len > EEPROM_SIZE - address - 1) {
        Log::error("EEPROM: length out of range");
        return;
    }
    Log::info("EEPROM: Written length (", String(len), ") of the message: ", value);

    EEPROM.put(address, len);
    writeEEPROM(address + 1, value, len);
    EEPROM.commit();
}

// *****************
// WiFi EEPROM utils
// *****************
inline bool isWifiSettingsExistsInEEPROM() {
    String ssidWifi = lengthAwareReadEEPROM(SSID_INDEX);
    String passwordWifi = lengthAwareReadEEPROM(PASSWORD_INDEX);

    return ssidWifi != nullptr && ssidWifi.length() > 0 && passwordWifi != nullptr;
}

inline Pair<String, String> readWifiDataFromEEPROM() {
    String ssidWifi = lengthAwareReadEEPROM(SSID_INDEX);
    String passwordWifi = lengthAwareReadEEPROM(PASSWORD_INDEX);

    return Pair<String, String>{ssidWifi, passwordWifi};
}

inline void writeWifiDataInEEPROM(const String &ssidWifi, const String &passwordWifi) {
    lengthAwareWriteEEPROM(SSID_INDEX, ssidWifi);
    lengthAwareWriteEEPROM(PASSWORD_INDEX, passwordWifi);
}

// **************************
// CurrencyPairs EEPROM utils
// **************************
inline bool isCurrencyPairsExistInEEPROM() {
    int numPairs = EEPROM.read(CURRENCY_PAIR_INDEX);
    return numPairs > 0;
}

inline void writeCurrencyPairs(const std::vector<Pair<String, String> > &currencyPairs) {
    int numPairs = currencyPairs.size();

    // TODO: Add check whether currency pairs are the same

    if (numPairs > MAX_CURRENCY_PAIRS) {
        Log::error("Number of currency pairs exceeds the maximum allowed: ", String(numPairs));
        return;
    }

    EEPROM.write(CURRENCY_PAIR_INDEX, numPairs);

    int index = CURRENCY_PAIR_INDEX + 1;

    for (const auto &pair: currencyPairs) {
        String currencyPair = pair.key + ":" + pair.value;

        if (currencyPair.length() != CURRENCY_PAIR_SIZE) {
            Log::error("Error: Currency pair size is not equal to allowed size.");
            continue;
        }
        writeEEPROM(index, currencyPair, CURRENCY_PAIR_SIZE);

        index += CURRENCY_PAIR_SIZE;
    }

    EEPROM.commit();
}

inline std::vector<Pair<Currency, Currency> > readCurrencyPairs() {
    std::vector<Pair<Currency, Currency> > currencyPairs;

    if (!isCurrencyPairsExistInEEPROM()) {
        Serial.println("No data found in EEPROM.");
        return currencyPairs;
    }

    int numPairs = EEPROM.read(CURRENCY_PAIR_INDEX);
    Serial.println(numPairs);
    if (numPairs > MAX_CURRENCY_PAIRS) {
        Log::error("EEPROM: Number of currency pairs exceeds the maximum allowed.");
        return currencyPairs;
    }

    int index = CURRENCY_PAIR_INDEX + 1;

    for (int i = 0; i < numPairs; i++) {
        String currencyPair = readEEPROM(index, CURRENCY_PAIR_SIZE);
        index += CURRENCY_PAIR_SIZE;

        if (currencyPair.length() != CURRENCY_PAIR_SIZE) {
            Log::error("EEPROM: Currency pair size is not equal to allowed size.");
            continue;
        }
        if (currencyPair.indexOf(':') != 3) {
            Log::error("EEPROM: Currency pair [", String(i), "] has wrong format.");
            continue;
        }

        String from = currencyPair.substring(0, 3);
        String to = currencyPair.substring(4);

        currencyPairs.push_back(Pair<Currency, Currency>(CurrencyUtils::toEnum(from), CurrencyUtils::toEnum(to)));
    }

    return currencyPairs;
}

#endif