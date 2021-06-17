#ifndef MCP9808DRIVER_MCP9808_HPP
#define MCP9808DRIVER_MCP9808_HPP

#include "MCP9808-constants.hpp"
#include <cstdint>
#include <arm_neon.h>
#include <definitions.h>

/**
 * MCP9808 temperature sensor driver
 *
 * This is a simple, (almost) feature-complete, documented (to the best of my ability) driver to use the
 * MCP9808 sensor on STM32 microcontrollers. All STM32-specific functions are used solely within the file
 * <b>MCP9808-internal.cpp</b> to allow for portability.
 *
 * Almost all settings are accessible via the provided functions, apart from whatever is a TODO,
 * of course ;-) using the constants provided in each function's javadoc string.
 * Tell me if I've missed something.
 *
 * For more details about the operation of the sensor, see the datasheet found at:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/25095A.pdf
 *
 * @todo Create a function which enables setting a custom address other than 0b000
 * @todo Create functions for setting the T_LOWER, T_UPPER and T_CRIT registers
 *
 * @author Grigoris Pavlakis <grigpavl@ece.auth.gr>
 * @author Konstantinos Kanavouras (Adaptation to Microchip + FreeRTOS)
 */

class MCP9808 {
    /**
     * I2C handle provided by ST
     */
    uint8_t i2c;

    static const int timeout = 100;

    void TWI_Initialize() const {
        if (i2c == 0) {
            return TWIHS0_Initialize();
        } else if (i2c == 2) {
            return TWIHS2_Initialize();
        }
    }

    [[nodiscard]] TWIHS_ERROR TWI_ErrorGet() const {
        if (i2c == 0) {
            return TWIHS0_ErrorGet();
        } else if (i2c == 2) {
            return TWIHS2_ErrorGet();
        }
    }

    [[nodiscard]] bool TWI_IsBusy() const {
        if (i2c == 0) {
            return TWIHS0_IsBusy();
        } else if (i2c == 2) {
            return TWIHS2_IsBusy();
        }
    }

    [[nodiscard]] bool TWI_Read(uint16_t address, uint8_t *pdata, size_t length) const {
        if (i2c == 0) {
            return TWIHS0_Read(address, pdata, length);
        } else if (i2c == 2) {
            return TWIHS2_Read(address, pdata, length);
        }
    }

    [[nodiscard]] bool TWI_Write(uint16_t address, uint8_t *pdata, size_t length) const {
        if (i2c == 0) {
            return TWIHS0_Write(address, pdata, length);
        } else if (i2c == 2) {
            return TWIHS2_Write(address, pdata, length);
        }
    }

    [[nodiscard]] bool TWI_WriteRead(uint16_t address, uint8_t *wdata, size_t wlength, uint8_t *rdata, size_t rlength) const {
        if (i2c == 0) {
            return TWIHS0_WriteRead(address, wdata, wlength, rdata, rlength);
        } else if (i2c == 2) {
            return TWIHS2_WriteRead(address, wdata, wlength, rdata, rlength);
        }
    }

    /**
    * Write a value to a register (see the constants in MCP9808-constants.hpp)
    * NOTE: this writes data as they are, so be careful!
    *
    * @param addr the address of the desired register
    * @param data the data octets to be written
    */
    [[nodiscard]] bool writeReg(uint8_t addr, uint16_t &data);

    /**
    * Safely change a setting on the register
    * This is the recommended function to use when changing settings,
    * and is used in all public functions that change settings.
    *
    * @param addr the address of the desired register
    * @param mask the appropriate bitmask to access the particular
    * setting bit or group of bits (found in mcp9808-constants.hpp)
    * @param setting the new value of the setting to be changed
    * (also found in mcp9808-constants.hpp)
    */
    [[nodiscard]] bool setReg(uint8_t addr, uint16_t mask, uint16_t setting);

    /**
     * Wait until I2C transaction is complete, yielding in the process
     */
    [[nodiscard]] bool wait();

public:
    /**
    * Initialize a new controller object for MCP9808
    * @param i2c handle to the appropriately-set and enabled I2C peripheral
    */
    MCP9808(uint8_t i2c);

    /**
    * Read a value from a register (see the constants in MCP9808-constants.hpp)
    * @param addr the address of the desired register
    * @param buffer the variable where the data will be stored
    */
    bool readReg(uint8_t addr, uint16_t &result);

    /**
     * Set the hysteresis temperature (THYST)
     * Available options are: 0, 1.5, 3, 6 degrees Celsius
     * @param temp one of: MCP9808_CONFIG_THYST_0C, MCP9808_CONFIG_THYST_1_5C, MCP9808_CONFIG_THYST_3C,
     * MCP9808_CONFIG_THYST_6C
     */
    bool setHystTemp(uint16_t temp);

    /**
    * Enter/exit low power mode (SHDN - shutdown mode)
    * @param setting one of: MCP9808_CONFIG_LOWPWR_ENABLE, MCP9808_CONFIG_LOWPWR_DISABLE
    */
    bool setLowPwrMode(uint16_t setting);

    /**
    * Set locking status of the critical temperature (TCRIT) register
    * @param setting one of: MCP9808_CONFIG_TCRIT_LOCK_ENABLE, MCP9808_CONFIG_TCRIT_LOCK_DISABLE
    */
    bool setCritTempLock(uint16_t setting);

    /**
    * Set locking status of the temperature window (T_UPPER, T_LOWER) registers
    * @param setting one of: MCP9808_CONFIG_WINLOCK_ENABLE, MCP9808_CONFIG_WINLOCK_DISABLE
    */
    bool setTempWinLock(uint16_t setting);

    /**
     * Enable or disable temperature alerts.
     * If enabled, alert output is asserted as a comparator/interrupt or critical temperature output
     * @param setting one of: MCP9808_CONFIG_ALERT_ENABLE, MCP9808_CONFIG_ALERT_DISABLE
     */
    bool setAlertStatus(uint16_t setting);

    /**
     * Enable or disable alert control mode.
     * @param setting one of: MCP9808_CONFIG_ALERT_CONTROL_ENABLE, MCP9808_CONFIG_ALERT_CONTROL_DISABLE
     */
    bool setAlertControl(uint16_t setting);

    /**
     * Select the event for which an alert will be emitted, if triggered.
     * If set to MCP9808_CONFIG_ALERT_SELECT_CRITONLY, then an alert is emitted only when
     * T_ambient > T_crit.
     * @param setting one of: MCP9808_CONFIG_ALERT_SELECT_CRITONLY, CONFIG_ALERT_SELECT_ALL
     */
    bool setAlertSelection(uint16_t setting);

    /**
     * Set the polarity of the emitted alert (active-low or active-high)
     * @param setting one of: MCP9808_CONFIG_ALERT_POLARITY_ACTIVE_HI, MCP9808_CONFIG_ALERT_POLARITY_ACTIVE_LOW
     */
    bool setAlertPolarity(uint16_t setting);

    /**
     * Set the alert mode (comparator or interrupt output)
     * @param setting one of: MCP9808_CONFIG_ALERT_MODE_IRQ, MCP9808_CONFIG_ALERT_MODE_COMPARATOR
     */
    bool setAlertMode(uint16_t setting);

    /**
    * Set the interrupts to be cleared on the next read attempt (namely, a temperature
    * reading or a command in general)
    */
    bool clearInterrupts();

    /**
     * Set the measurement resolution.
     * @param setting one of: MCP9808_RES_0_50C, MCP9808_RES_0_25C, MCP9808_RES_0_125C, MCP9808_RES_0_0625C
     */
    bool setResolution(uint16_t setting);

    /**
     * Get the current temperature reading (in Celsius)
     * @param result the variable where the result is going to be stored
     */
    [[nodiscard]] bool getTemp(float32_t &result);
};

#endif //MCP9808DRIVER_MCP9808_HPP
