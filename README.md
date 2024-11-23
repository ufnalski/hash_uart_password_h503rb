# HASH, CRC and RNG peripherals (STM32H503RB)
This repo is here to lure you into experimenting with HASH (e.g. SHA-256), CRC and RNG peripherals. I should probably make separate STM32CubeIDE projects devoted to each topic. It was an unstructured incremental process. The technical debt[^1] is huge here. Sorry for so many #ifs, including nested ones :innocent: If randomness is your cap of coffee, check my repos devoted to TRNG/HRNG-s such as the [Lavarand using OV2640 camera module](https://github.com/ufnalski/ov2640_lavarand_h743zi2) or the [thermal noise based RNG](https://github.com/ufnalski/hx711_thermal_noise_rng_g431kb). Be warned not to fall down the rabbit hole - plenty of corridors with countless doors :door:

[^1]: [Technical debt](https://en.wikipedia.org/wiki/Technical_debt) (Wikipedia)

![HASH, CRC and RNG in action](/Assets/Images/hash_crc_rng_in_action.jpg)

![CRC and HASH in serial terminal](/Assets/Images/uart_debug_output.JPG)

> [!TIP]
> Make sure to configure your serial monitor not to append e.g. "/r/n" when sending the password.

> [!TIP]
> The HASH peripheral is not ubiquitous. Don't panic. You can always do it on the main core with the help of libraries such as the [Mbed TLS](https://github.com/Mbed-TLS/mbedtls).

# Missing files?
Don't worry :slightly_smiling_face: Just hit Alt-K to generate /Drivers/CMCIS/ and /Drivers/STM32H5xx_HAL_Driver/ based on the .ioc file. After a couple of seconds your project will be ready for building.

# Pre-workshop materials
## CRC
* [CRC Calculator](https://www.sunshine2k.de/coding/javascript/crc/crc_js.html) (Bastian Molkenthin)
* [Understanding and implementing CRC (Cyclic Redundancy Check) calculation](https://www.sunshine2k.de/articles/coding/crc/understanding_crc.html) (Bastian Molkenthin)
* [How do CRCs work?](https://www.youtube.com/watch?v=izG7qT0EpBw) (Ben Eater)
* [Hardware build: CRC calculation](https://www.youtube.com/watch?v=sNkERQlK8j8) (Ben Eater)
* [crc-hamming-weights](https://github.com/dmhacker/crc-hamming-weights) (David Hacker)
* [Cyclic Redundancy Code (CRC) Polynomial Selection For Embedded Networks](https://users.ece.cmu.edu/~koopman/roses/dsn04/koopman04_crc_poly_embedded.pdf) (Philip Koopman and Tridib Chakravarty)

## HASH
* [Cryptographic hash function](https://en.m.wikipedia.org/wiki/Cryptographic_hash_function) (Wikipedia)
* [Hash Calculator Online](https://www.pelock.com/products/hash-calculator) (Bartosz Wojcik)
* [Online Tools](https://emn178.github.io/online-tools/)
* [Online Tools (source on GitHub)](https://github.com/emn178/online-tools)

## RNG
* [STM32G4 - RNG](https://www.st.com/content/ccc/resource/training/technical/product_training/group0/22/32/23/22/be/bc/46/5a/STM32G4-Security-Random_Number_Generator_RNG/files/STM32G4-Security-Random_Number_Generator_RNG.pdf/_jcr_content/translations/en.STM32G4-Security-Random_Number_Generator_RNG.pdf) (STMicroelectronics)
* [Electronic dice demo](https://github.com/ufnalski/roll_a_die_with_charlie_l432kc)
* [Thermal noise based random number generator](https://github.com/ufnalski/hx711_thermal_noise_rng_g431kb)

## EEPROM
* [How To Interface an I2C EEPROM With STM32](https://embeddedprojects101.com/how-to-interface-an-i2c-eeprom-with-stm32/) (Mohamed Boubaker)
* [EEPROM and STM32](https://controllerstech.com/eeprom-and-stm32/) (ControllersTech)
* [EEPROM Library for STM32 using HAL](https://github.com/controllerstech/STM32/tree/master/EEPROM_STM32) (ControllersTech)
* [24xx EEPROM library for stm32 HAL](https://github.com/nimaltd/ee24) (Nima Askari)
* [Gravity I2C EEPROM Data Storage Module](https://wiki.dfrobot.com/Gravity__I2C_EEPROM_Data_Storage_Module_SKU__DFR0117) (DFRobot)
* [256K I2C CMOS Serial EEPROM](https://ww1.microchip.com/downloads/en/DeviceDoc/24AA256-24LC256-24FC256-Data-Sheet-20001203V.pdf) (Microchip)

## Keypad
* [How to Use 4×4 keypad with STM32](https://controllerstech.com/use-4x4-keypad-with-stm32/) (ControllersTech)
* [Matrix Keypad library](https://github.com/nimaltd/KeyPad) (Nima Askari)
* [STM32 Keypad Interfacing Library 4×4 | Keypad Driver With Examples](https://deepbluembedded.com/stm32-keypad-interfacing-library/) (Khaled Magdy)

# Call for action
Create your own [home laboratory/workshop/garage](http://ufnalski.edu.pl/control_engineering_for_hobbyists/2024_dzien_popularyzacji_matematyki/Dzien_Popularyzacji_Matematyki_2024.pdf)! Get inspired by [ControllersTech](https://www.youtube.com/@ControllersTech), [DroneBot Workshop](https://www.youtube.com/@Dronebotworkshop), [Andreas Spiess](https://www.youtube.com/@AndreasSpiess), [GreatScott!](https://www.youtube.com/@greatscottlab), [ElectroBOOM](https://www.youtube.com/@ElectroBOOM), [Phil's Lab](https://www.youtube.com/@PhilsLab), [atomic14](https://www.youtube.com/@atomic14), [That Project](https://www.youtube.com/@ThatProject), [Paul McWhorter](https://www.youtube.com/@paulmcwhorter), [Max Imagination](https://www.youtube.com/@MaxImagination), [Nikodem Bartnik](https://www.youtube.com/@nikodembartnik), and many other professional hobbyists sharing their awesome projects and tutorials! Shout-out/kudos to all of them!

> [!WARNING]
> Control engineering - do try this at home :sunglasses:

190+ challenges to start from: [Control Engineering for Hobbyists at the Warsaw University of Technology](http://ufnalski.edu.pl/control_engineering_for_hobbyists/Control_Engineering_for_Hobbyists_list_of_challenges.pdf).

Stay tuned!
