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
    fclose(fp);
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

void writel(mylong res, const char *f) {
    FILE *fp = fopen(f, "w+");
    if (fp == NULL) {
        printf("write to file err\n");
        exit(1);
    }
    if (res.sign)
        fprintf(fp, "-");
    int i;
    for (i = res.size - 1; i >= 0; i--)
        fprintf(fp, "%d", res.d[i]);
    fclose(fp);
}

void freelong(mylong res) {
    free(res.d);
}


int comparel(mylong al1, mylong al2) {
        int i, k1 = (al1.sign ? -1 : 1), k2 = ( al2.sign ? 1 : -1);
        /*
        al1>al2 return 1
        al1<al2 return -1
        al1=al2 return 0
        */
        if (al1.sign&&!al2.sign) return -1;
        if (!al1.sign&&al2.sign) return 1;
        if (al1.size<al2.size) return -1;
        if (al1.size>al2.size) return 1;
        for (i=al1.size;i>=0;--i) {
                if (al1.d[i]>al2.d[i]) return k1;
                if (al1.d[i]<al2.d[i]) return k2;
        }
        return 0;
}


void main() {
   mylong t = readl("my");
   mylong p = readl("res");
   printf("%d",comparel(t,p));
}
