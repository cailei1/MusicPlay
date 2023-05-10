//
// Created by 蔡磊 on 2022/9/8.
//
#include <stdio.h>
#include <stdlib.h>

#include "libavcodec/avcodec.h"

char *testPrint(char *said) {
    printf("hello %s", said);
    av_register_hwaccel(NULL);
    return "hello wrodl";

}