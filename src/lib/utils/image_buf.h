#ifndef _IMAGE_BUF_H_
#define _IMAGE_BUF_H_

#include <stdint.h>

typedef struct ImageBuf{
    uint8_t *pImgData;
    uint32_t length;
    uint16_t width;
    uint16_t height;
}ImageBuf_s;


#endif