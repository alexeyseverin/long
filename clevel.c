#include <stdio.h>
#include <stdlib.h>
#include "clevel.h"

mylong readl(const char *s) {
    mylong res;
    int i;
    FILE *fp = fopen(s, "r+");
	if (fp == NULL) {
		printf("file open err\n");
		exit(1);
	}
	
	char sign;
	fread(&sign, 1, 1, fp);
    int signoff = 0;
    if (sign == '-') 
        signoff = 1;
	//file size
	fseek(fp, 0L, SEEK_END);
    int size = ftell(fp) - signoff;
    res = alloclong(size);
	res.sign = signoff;
    fseek(fp, signoff, SEEK_SET);
    for (i = size - 1; i >= 0; i--) {
        char c;
        c = getc(fp);
        res.d[i] = c - '0';        
    }

    for (i= 0; i < res.size; ++i)
        printf("d[i]=%d\n", res.d[i]);
    
    return res;
}

mylong alloclong(int size) {
	mylong res;
	res.sign = 0;
	res.size = size;
	res.d = calloc(size, 1); 
	if (res.d == NULL) {
		printf("alloc err\n");
		exit(1);
	}
	return res;
}

void freelong(mylong res) {
    free(res.d);
}

void main() {
   mylong t = readl("my");
}
