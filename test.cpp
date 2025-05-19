//
// Created by 13776 on 2025/5/19.
//
#include"mouse.h"
int main() {

    for(int i=0;i<6;i++) {
        for(int j=0;j<6;j++) {
            MouseHandler(i,j);
            _sleep(1000);
        }
    }
}
