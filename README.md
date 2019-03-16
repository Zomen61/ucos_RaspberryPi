chinese version: - [中文](https://github.com/Zomen61/ucos_RaspberryPi/blob/master/%E4%B8%AD%E6%96%87README.md)
# uCOS-II on Raspberry Pi

This is a very basic port of uCOS-II to Raspberry Pi. The version of uCOS-II we have used is V2.52.  

## Contents of each fold

bsp：Board support package. Handle interrupt,uart and timer
build：Store obj files generated during compilation
h：Head files
init：Startup code
lib：Library for standard c and libc 
port：Porting code 
ucos：Source code of uCOS-II.Here we don't proive the source code, and you can get the code from http://www.micrium.com
usrApp：Entry function: main and a simple application


## How to make
First, you should put the source code of uCOS-II in the fold "ucos".

For Windows: Download the yagarto toolchain. Then you can make directly.
For Linux: Get the toolchain, You maybe need to rewrite the makefile according to the toolchain used.

## update(2019/02/12)
Add an interrupt to the original mini uart and use the producer and consumer algorithms to output the characters entered from uart to the console.

## update(2019/03/10)
Reference: https://github.com/bztsrc/raspi3-tutorial

Add raspberry pi mailbox call, and framebuffer enable, you can now use the keyboard to enter text on uart, the text will be displayed to hdmi output.

When the enter is pressed, the program will determine if it is an instruction. The current instructions are:
1. homer.img
print image "homer"。
2. lena
print image "lena"。
3. clear
clear hdmi outpur(clear screen)。

