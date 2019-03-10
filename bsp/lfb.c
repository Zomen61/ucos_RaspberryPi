/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "uart.h"
#include "mbox.h"
#include "homer.h"
#include "lena.h"

#define screenX 1024
#define screenY 768

/* PC Screen Font as used by Linux Console */
typedef struct {
    unsigned int magic;
    unsigned int version;
    unsigned int headersize;
    unsigned int flags;
    unsigned int numglyph;
    unsigned int bytesperglyph;
    unsigned int height;
    unsigned int width;
    unsigned char glyphs;
} __attribute__((packed)) psf_t;
extern volatile unsigned char _binary_font_font_psf_start;

psf_t *font;

unsigned int width, height, pitch;
unsigned char *lfb,*nowPos;
unsigned int colCN,rowCN;
unsigned int colCC,rowCC;
unsigned char overSrceen = 0;

char keyin[1024];
unsigned int keyPos;

/**
 * Set screen resolution to 1024x768
 */
void lfb_init()
{
    mbox[0] = 35*4;
    mbox[1] = MBOX_REQUEST;

    mbox[2] = 0x48003;  //set phy wh
    mbox[3] = 8;
    mbox[4] = 8;
    mbox[5] = 1024;         //FrameBufferInfo.width
    mbox[6] = 768;          //FrameBufferInfo.height

    mbox[7] = 0x48004;  //set virt wh
    mbox[8] = 8;
    mbox[9] = 8;
    mbox[10] = 1024;        //FrameBufferInfo.virtual_width
    mbox[11] = 768;         //FrameBufferInfo.virtual_height
    
    mbox[12] = 0x48009; //set virt offset
    mbox[13] = 8;
    mbox[14] = 8;
    mbox[15] = 0;           //FrameBufferInfo.x_offset
    mbox[16] = 0;           //FrameBufferInfo.y.offset
    
    mbox[17] = 0x48005; //set depth
    mbox[18] = 4;
    mbox[19] = 4;
    mbox[20] = 32;          //FrameBufferInfo.depth

    mbox[21] = 0x48006; //set pixel order
    mbox[22] = 4;
    mbox[23] = 4;
    mbox[24] = 1;           //RGB, not BGR preferably

    mbox[25] = 0x40001; //get framebuffer, gets alignment on request
    mbox[26] = 8;
    mbox[27] = 8;
    mbox[28] = 4096;        //FrameBufferInfo.pointer
    mbox[29] = 0;           //FrameBufferInfo.size

    mbox[30] = 0x40008; //get pitch
    mbox[31] = 4;
    mbox[32] = 4;
    mbox[33] = 0;           //FrameBufferInfo.pitch

    mbox[34] = MBOX_TAG_LAST;

    if(mbox_call(MBOX_CH_PROP) && mbox[20]==32 && mbox[28]!=0) {
        mbox[28]&=0x3FFFFFFF;
        width=mbox[5];
        height=mbox[6];
        pitch=mbox[33];
        lfb=(void*)((unsigned long)mbox[28]);
        nowPos = lfb;
    } else {
        uart_string("Unable to set screen resolution to 1024x768x32\n");
    }

    font = (psf_t*)&_binary_font_font_psf_start;

    for(int i=0;i<1024;i++){
       keyin[i]=0;
    }
    keyPos = 0;

    colCN = (screenX / font->width) - 1;
    rowCN = (screenY / font->height) - 1;

    colCC = 0;
    rowCC = 0;
}

/**
 * Show a picture
 */
void lfb_showpicture()
{
    int x,y;
    unsigned char *ptr=lfb;
    char *data = homer_data, pixel[4];

    ptr += (height-homer_height)/2*pitch + (width-homer_width)*2;
    for(y=0;y<homer_height;y++) {
        for(x=0;x<homer_width;x++) {
            HEADER_PIXEL(data, pixel);
            *((unsigned int*)ptr)=*((unsigned int *)&pixel);
            ptr+=4;
        }
        ptr+=pitch-homer_width*4;
    }
}

void lfb_showlena()
{
    int x,y;
    unsigned char *ptr=lfb;
    unsigned char *data = rawData;
    
    ptr += (height-img_height)/2*pitch + (width-img_width)*2;
    for(y=0;y<img_height;y++) {
        for(x=0;x<img_width;x++) {
            for(int i=0;i<4;i++){
                *((unsigned char*)ptr)=data[i];
                ptr++;
            }
            data += 4;
        }
        ptr+=pitch-img_width*4;
    }
}

void lfb_print(char s){
    // draw next character if it's not zero

    // get the offset of the glyph. Need to adjust this to support unicode table
    unsigned char *glyph = (unsigned char*)&_binary_font_font_psf_start +
        font->headersize + (((unsigned char)s)<font->numglyph?s:0)*font->bytesperglyph;
    // calculate the offset on screen
    unsigned int offs = nowPos - lfb;
    // variables
    int i,j, line,mask, bytesperline=(font->width+7)/8;
    // handle carrige return

    if(s=='\r') {
        nowPos = lfb + (rowCC * font->height * pitch);
    } else if(s=='\n') {
        rowCC++;
        if(rowCC >= rowCN){
            nowPos = lfb;
            rowCC = 0;
        }else{
            nowPos = lfb + (offs + pitch);
        }

        if(overSrceen == 0){
            keyin[keyPos] = '\0';
            if(strcmp(keyin,"clear")==0){
                lfb_clear();
            }else if(strcmp(keyin,"homer.img") == 0){
                lfb_showpicture();
            }else if(strcmp(keyin,"lena") == 0){
                lfb_showlena();
            }
            keyPos = 0;
        }

    } else {

        keyin[keyPos] = s;
        keyPos++;

        colCC++;
        if(colCC >= colCN){
            rowCC++;
            nowPos = lfb + (rowCC * font->height * pitch);
            offs = nowPos - lfb;
            colCC = 0;
        }

        // display a character
        for(j=0;j<font->height;j++){
            // display one row
            line=offs;
            mask=1<<(font->width-1);
            for(i=0;i<font->width;i++){
                // if bit set, we use white color, otherwise black
                *((unsigned int*)(lfb + line))=((int)*glyph) & mask?0xFFFFFF:0;
                mask>>=1;
                line+=4;
            }
            // adjust to next line
            glyph+=bytesperline;
            offs+=pitch;
        }
        nowPos += (font->width - 1)*4;
    }  
}


void lfb_print_Pos(int x, int y, char *s){

    // get our font
    psf_t *font = (psf_t*)&_binary_font_font_psf_start;
    // draw next character if it's not zero
    while(*s) {
        // get the offset of the glyph. Need to adjust this to support unicode table
        unsigned char *glyph = (unsigned char*)&_binary_font_font_psf_start +
         font->headersize + (*((unsigned char*)s)<font->numglyph?*s:0)*font->bytesperglyph;
        // calculate the offset on screen
        int offs = (y * font->height * pitch) + (x * (font->width+1) * 4);
        // variables
        int i,j, line,mask, bytesperline=(font->width+7)/8;
        // handle carrige return
        if(*s=='\r') {
            x=0;
        } else
        // new line
        if(*s=='\n') {
            x=0; y++;
        } else {
            // display a character
            for(j=0;j<font->height;j++){
                // display one row
                line=offs;
                mask=1<<(font->width-1);
                for(i=0;i<font->width;i++){
                    // if bit set, we use white color, otherwise black
                    *((unsigned int*)(lfb + line))=((int)*glyph) & mask?0xFFFFFF:0;
                    mask>>=1;
                    line+=4;
                }
                // adjust to next line
                glyph+=bytesperline;
                offs+=pitch;
            }
            x++;
        }
        // next character
        s++;
    }
}

void lfb_clear(){
    int x,y;
    unsigned char *ptr=lfb;

    for(y=0;y<screenY;y++) {
        for(x=0;x<screenY;x++) {
            *((unsigned int*)ptr)= 0x00000000;
            ptr+=4;
        }
    }
    nowPos = lfb;
}

int strcmp(char *str1,char *str2){
    while(*str1 != '\0' && *str2 != '\0'){
        if(*str1 > *str2){
            return 1;
        }else if (*str1 < *str2){
            return -1;
        }

        str1++;
        str2++;
    }

    if(*str1 == '\0' && *str2 == '\0'){
        return 0;
    }else{
        if(*str1 == '\0'){
            return -1;
        }else{
            return 1;
        }
    }
}