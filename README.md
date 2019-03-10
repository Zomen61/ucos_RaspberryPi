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
在原本的mini uart上加入中斷，並使用生產者與消費者演算法將從uart輸入的字元輸出到console上。

## update(2019/03/10)
加入raspberry pi mailbox call，以及framebuffer啟用，目前可以在uart上用鍵盤輸入文字，文字會被顯示到hdmi輸出。

在按下enter時，程式會判斷是否為指令，目前指令有:
1. homer.img
印出homer圖。
2. lena
印出lena圖。
3. clear
將整個畫面清空。

