# FreeRTOS FDIR test

## Installation
1. You will need to install `openocd` with `cmsis-dap` support. You will need to `sudo apt install libhidapi-dev`.
2. Copy `60-openocd.rules` to `/etc/udev/rules.d/` and run `sudo udevadm control --reload-rules && sudo udevadm trigger`

## Connecting to a board
This project is designed to be executed on the ATSAMV71-XULT development board, connected
via USB or via SEGGER J-Link. To test the connection with OpenOCD, you can run:
```bash
openocd -f /usr/local/share/openocd/scripts/interface/cmsis-dap.cfg
```