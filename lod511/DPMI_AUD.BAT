@echo off

rem This batch file demonstrates the use of MOD music under DPMI mode.

rem Load the DPMI version. IRQ and DMA numbers must be specified!
rem The defaults are for a SoundBlaster-16 at irq=5, dma=1, and base=220

game_dpm /l /audiosb16 /irq5 /dma1 /base220 /ega

rem             ^        ^     ^    ^-Specify your sound card's base address
rem             :        :     :------Set your sound cards DMA level here
rem             :        :------------And put the sound card IRQ here
rem             ----------------------Change driver to match your sound card

