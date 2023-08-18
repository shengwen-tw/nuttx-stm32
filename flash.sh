#!/usr/bin/env bash

bin=nuttx/nuttx.bin
mcu="stm32f7" #"st32f0", "stm32f4", "stm32f7"

function flash_stm32f0() {
    openocd -f interface/stlink.cfg \
            -f target/stm32f0x.cfg \
            -c "init" \
            -c "reset init" \
            -c "halt" \
            -c "flash write_image erase $bin" \
            -c "verify_image $bin" \
            -c "reset run" -c shutdown
}

function flash_stm32f4() {
    openocd -f interface/stlink.cfg \
            -f target/stm32f4x.cfg \
            -c "init" \
            -c "reset init" \
            -c "halt" \
            -c "flash write_image erase $bin" \
            -c "verify_image $bin" \
            -c "reset run" -c shutdown
}

function flash_stm32f7() {
    openocd -f interface/stlink-v2.cfg \
            -c "transport select hla_swd" \
            -f target/stm32f7x.cfg \
            -c "reset_config none separate" \
            -c "init" \
            -c "reset init" \
            -c "halt" \
            -c "flash write_image erase $bin 0x8000000" \
            -c "verify_image $bin 0x8000000" \
            -c "reset run" -c shutdown
}

if [ "$mcu" == "stm32f0" ]; then
    flash_stm32f0
elif [ "$mcu" == "stm32f4" ]; then
    flash_stm32f4
elif [ "$mcu" == "stm32f7" ]; then
    flash_stm32f7
else
    echo "Invalid STM32 model!"
fi
