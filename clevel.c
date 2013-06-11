#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "clevel.h"

int max(int a, int b) {
    return (a > b ? a : b);
}

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

mylong subl(mylong al1, mylong al2) {
    mylong res;
    //a>0 b<0 = a+b
    if (!al1.sign&&al2.sign) {
        res = addl_unsign(al1,al2);        
    }
    //a<0 b>0 = -(|a|+|b|)
    if (al1.sign&&!al2.sign) {
        res = addl_unsign(al1,al2);        
        res.sign=1-res.sign;
    }
    //a<0 b<0 =  |b|-|a| 
    if (al1.sign&&al2.sign) {
        res =subl_unsign(al2,al1);
        res.sign = 1 - res.sign;
    }
    if (!al1.sign && !al2.sign) {
        res = subl_unsign(al1,al2);    
    }
    return res;
} 

mylong subl_unsign(mylong al1, mylong al2) {
    mylong res = alloclong(max(al1.size, al2.size));
    if (comparel(al1, al2) == -1) {
        mylong tmp = al1;
        al1 = al2;
        al2 = tmp;        
        res.sign = 1;
    }
    int i,c=0;
    for (i=0;i<al1.size;++i) {
        c+=al1.d[i]-al2.d[i]+10;
        res.d[i]=c%10;
        if (c<10) c=-1; else c=0;
    }
    while (res.d[res.size - 1]==0&&res.size>1) res.size--;
    return res;
}


mylong addl(mylong al1, mylong al2) {
    mylong res;
    //a>0 b<0 = |a|-|b| 
    if (!al1.sign&&al2.sign) {
        res = subl_unsign(al1,al2);       
    }
    //a<0 b>0 = |b|-|a|
    if (al1.sign&&!al2.sign) {
        res = subl_unsign(al2,al1);        
    }
    //a<0 b<0 = -(|a|+|b|) 
    if (al1.sign&&al2.sign) {
        res = addl_unsign(al1, al2);
        res.sign = 1;
    }
    if (!al1.sign && !al2.sign) {
        res = addl_unsign(al1,al2);    
    }
    return res;
}

mylong addl_unsign(mylong al1, mylong al2) { 
    mylong res = alloclong(max(al1.size, al2.size));
    int i,c=0,m=max(al1.size,al2.size);
    for (i=0;i<m;++i) {
        c+=al1.d[i]+al2.d[i];
        res.d[i]=c%10;
        c/=10;
    }
    if (c>0) 
        res.d[m]=c;
    res.size=m;
    return res;
}

mylong mull(mylong al1,mylong al2) {
    mylong res = alloclong(al1.size + al2.size);
    res.size = 0;
    int c=0,i,j,k;
    if ((al1.sign&&al2.sign)||(!al1.sign&&!al2.sign)) res.sign=0; else res.sign=1;
    for (i=0;i<al1.size;++i) 
        for (j=0;j<al2.size;++j) {
        k=i+j;
        c=al1.d[i]*al2.d[j];
        while (c) {
            c+=res.d[k];
            res.d[k]=c%10;
            c/=10;
            if (k+1>res.size) 
                res.size=k+1;
            k++;                
        }
    } 
    return res;
}

void main() {
   mylong t = readl("my");
   mylong p = readl("res");
   mylong res = mull(t,p);
writel(res, "file");

}
