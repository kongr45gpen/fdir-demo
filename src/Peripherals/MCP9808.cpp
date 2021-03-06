/**
 * @author Grigoris Pavlakis <grigpavl@ece.auth.gr>
 */

#include <Logger.hpp>
#include "Peripherals/MCP9808.hpp"

MCP9808::MCP9808(uint8_t i2c) : i2c(i2c) {
}

bool MCP9808::setHystTemp(uint16_t temp) {
    return setReg(MCP9808_REG_CONFIG, MCP9808_CONFIG_THYST_MASK, temp);
}

bool MCP9808::setLowPwrMode(uint16_t setting) {
    return setReg(MCP9808_REG_CONFIG, MCP9808_CONFIG_SHDN_MASK, setting);
}

bool MCP9808::setCritTempLock(uint16_t setting) {
    return setReg(MCP9808_REG_CONFIG, MCP9808_CONFIG_TCRIT_LOCK_MASK, setting);
}

bool MCP9808::setTempWinLock(uint16_t setting) {
    return setReg(MCP9808_REG_CONFIG, MCP9808_CONFIG_WINLOCK_MASK, setting);
}

bool MCP9808::clearInterrupts() {
    return setReg(MCP9808_REG_CONFIG, MCP9808_CONFIG_IRQ_CLEAR_MASK, MCP9808_CONFIG_IRQ_CLEAR);
}

bool MCP9808::setAlertStatus(uint16_t setting) {
    return setReg(MCP9808_REG_CONFIG, MCP9808_CONFIG_ALERT_STATUS_MASK, setting);
}

bool MCP9808::setAlertControl(uint16_t setting) {
    return setReg(MCP9808_REG_CONFIG, MCP9808_CONFIG_ALERT_CONTROL_MASK, setting);
}

bool MCP9808::setAlertSelection(uint16_t setting) {
    return setReg(MCP9808_REG_CONFIG, MCP9808_CONFIG_ALERT_SELECT_MASK, setting);

}

bool MCP9808::setAlertPolarity(uint16_t setting) {
    return setReg(MCP9808_REG_CONFIG, MCP9808_CONFIG_ALERT_POLARITY_MASK, setting);

}

bool MCP9808::setAlertMode(uint16_t setting) {
    return setReg(MCP9808_REG_CONFIG, MCP9808_CONFIG_ALERT_MODE_MASK, setting);
}

bool MCP9808::setResolution(uint16_t setting) {
    return setReg(MCP9808_REG_RESOLUTION, MCP9808_RES_MASK, setting << 8);
    // since the bits we're interested in are located in the less significant byte, and
    // the I2C protocol reads MSB-first, while the register fits only 8 bits,
    // we shift by 8 to bring them to the MSB and thus store them.

}

bool MCP9808::isIDok() {
    uint16_t id;

    if (!readReg(MCP9808_REG_MFGID, id)) {
        return false;
    }

    return id == MCP9808_MANUFACTURER_ID;
}

bool MCP9808::getTemp(float32_t& result) {
    uint16_t data;
    if (!readReg(MCP9808_REG_TEMP, data)) {
        return false;
    }

    // keep bit 12 only (the sign bit) and if it's 1, we start counting from -2^12
    // TODO: find a way to test negative temperatures
    if ((data & 0x100000)) {
        result += 1 / (float)(2 << 12);
    }

    // start summing the other bits
    for (uint8_t i = 0; i < 12; i++) {
        if (data & 0x01) { // we need the LSB only in every iteration
            result += powf(2, i-4);  // add the appropriate power of 2 to the result
        }
        data >>= 1;  // then toss out the bit
    }

    return true;
}
