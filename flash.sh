#!/usr/bin/env bash

TARGET=nuttx/nuttx.bin

openocd -f interface/stlink-v2.cfg \
        -c "transport select hla_swd" \
        -f target/stm32f7x.cfg \
        -c "reset_config none separate" \
        -c "init" \
        -c "reset init" \
        -c "halt" \
        -c "flash write_image erase ${TARGET} 0x8000000" \
        -c "verify_image ${TARGET} 0x8000000" \
        -c "reset run" -c shutdown
