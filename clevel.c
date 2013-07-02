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
	fseek(fp, -1L, SEEK_END);
    int size = ftell(fp) - signoff;
    if (isdigit(getc(fp))) {
        size++;
    }
    res = alloclong(size);
	res.sign = signoff;
    fseek(fp, signoff, SEEK_SET);
    for (i = size - 1; i >= 0; i--) {
        char c;
        c = getc(fp);
        res.d[i] = c - '0';        
    }
    for (i = size - 1; i>=0; --i) {
        if (res.d[i] != 0)
            break;
        res.size--;
    }
    fclose(fp);
    return res;
}
mylong copylong(mylong obj) {
    mylong res = alloclong(obj.size);
    res.sign=obj.sign;
    int i;
    for (i = 0; i<res.size; ++i)
        res.d[i] = obj.d[i];
    return res;
}

mylong negl(mylong a) {
    a.sign = !a.sign;
    return a;
}

mylong alloclong(int size) {
	mylong res;
	res.sign = 0;
	res.size = size;
	res.d = calloc(size, 2); 
	if (res.d == NULL) {
		printf("alloc err\n");
		exit(1);
	}
	return res;
}

void outl(mylong res) {
    if (res.sign)
        printf("-");
    int i;
    for (i = res.size - 1; i >= 0; i--)
        printf("%d", res.d[i]);
    printf("\n");
}

void writel(mylong res, const char *f) {
    FILE *fp = fopen(f, "w+");
    if (fp == NULL) {
        printf("write to file err\n");
        exit(1);
    }
    if (res.size == 0) {
        fprintf(fp, "0");
    } else {
        if (res.sign)
            fprintf(fp, "-");
        int i;
        for (i = res.size - 1; i >= 0; i--)
            fprintf(fp, "%d", res.d[i]);
    }
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
        res = subl_unsign(al2,al1);
        res.sign = res.sign;
    }
    if (!al1.sign && !al2.sign) {
        res = subl_unsign(al1,al2);    
    }
    return res;
} 

mylong subl_unsign(mylong al1, mylong al2) {
    mylong res = alloclong(max(al1.size, al2.size));
    al1.sign = 0;
    al2.sign = 0;
    if (comparel(al1, al2) == -1) {
        mylong tmp = al1;
        al1 = al2;
        al2 = tmp;        
        res.sign = 1;
    }
    int i,c=0,j,k;
    for (i=0;i<al1.size;++i) {
	j=al1.d[i];
	k=al2.d[i];
	if (i>al1.size) j=0;
	if (i>al2.size) k=0;
        c+=j-k+10;
        res.d[i]=c%10;
        if (c<10) c=-1; else c=0;
    }
    while (res.d[res.size - 1]==0&&res.size>=1) res.size--;
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
    mylong res = alloclong(max(al1.size, al2.size)+1);
    int i,c=0,j,k,m=max(al1.size,al2.size);
    for (i=0;i<m;++i) {
	j=al1.d[i];
	k=al2.d[i];
	if (i > al1.size) j = 0;
	if (i > al2.size) k = 0;
        c+=j+k;
        res.d[i]=c%10;
        c/=10;
    }
    if (c>0) {
	m=m+1; 
        res.d[m]=c;
    }
    res.size=m;
    if ((res.d[m-1] == 0) && (m>1)) res.d[m-1] = 1;
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

mylong mulltoint(mylong al1, int al2) {
    if (al2 == 0) {
        mylong zero = alloclong(0);
        return zero;
    }
    mylong res = alloclong(al1.size + (int)log10(al2) + 1);
    int al2sign = al2 < 0;
    if (al2 < 0) {
        al2 = -al2;
    }
    res.size = al1.size;
    int c=0,i,j,k;
    if ((al1.sign&&al2sign)||(!al1.sign&&!al2sign)) 
        res.sign=0; 
    else 
        res.sign=1; 
    long long r = 0;
    for (i=0;i<res.size|r;i++) {
        long long tmp = al2;
        if (i < al1.size)
            tmp *= al1.d[i];
        else
            tmp = 0;
        tmp += r;
        res.d[i] = tmp%10;
        r = tmp / 10;
   
        if (res.d[res.size])
            res.size++;
    }

    return res;
}


mylong divl(mylong al1, mylong al2)
{
    if (al2.size == 0)
        exit(1);
    mylong res = alloclong(al1.size);
    if ((al1.sign&&al2.sign)||(!al1.sign&&!al2.sign)) res.sign=0; else res.sign=1;
    al2.sign = al1.sign = 0; 
    if (comparel(al1,al2) < 0) {
        mylong zero=alloclong(0);
        return zero;
    }
    mylong curvalue  = alloclong(al2.size);
    curvalue.size = 1;
    int i;  
    for (i = al1.size-1; i>=0; i--)  {
        int j; 
        if (curvalue.size > 0) {
            for (j = curvalue.size;j>=1;j--)
                curvalue.d[j] = curvalue.d[j-1];
            if (curvalue.d[curvalue.size])
                curvalue.size++;
        } else
            curvalue.size = 1;
        curvalue.d[0] = al1.d[i];
        int x = 0;
        int l = 0, r = 10;
        while (l <= r) {
            int m = (l + r) >> 1;
            mylong cur = mulltoint(al2, m);
            if (comparel(cur, curvalue) <= 0)  {
                x = m;
                l = m+1;
            } else
                r = m-1;
        }
        res.d[i] = x;
        mylong tmp = mulltoint(al2, x),
            tmp2 = subl_unsign(curvalue, tmp);
        freelong(curvalue);
        curvalue = tmp2;
        freelong(tmp);
    }

    int pos = al1.size;
    while (pos>=0 && !res.d[pos])
        pos--;
    res.size = pos+1;
    freelong(curvalue);
    return res;
}



mylong modl(mylong al1, mylong al2) {
    if (al2.size == 0)
        exit(1);
    if (!al1.sign && !al2.sign)
        if (comparel(al1,al2) < 0) {
            mylong re=copylong(al1);
            return re;
        }
    int al1sign=al1.sign, al2sign=al2.sign;

    mylong curvalue  = alloclong(al2.size + 1);
    al2.sign = al1.sign = 0;
    curvalue.size = 1;
    int i;  
    for (i = al1.size-1; i>=0; i--)  {
        int j; 
        if (curvalue.size > 0) {
            for (j = curvalue.size;j>=1;j--)
                curvalue.d[j] = curvalue.d[j-1];
            if (curvalue.d[curvalue.size])
                curvalue.size++;
        } else
            curvalue.size = 1;
        curvalue.d[0] = al1.d[i];
        int x = 0;
        int l = 0, r = 10;
        while (l <= r) {
            int m = (l + r) >> 1;
            mylong cur = mulltoint(al2, m);
            if (comparel(cur, curvalue) <= 0)  {
                x = m;
                l = m+1;
            } else
                r = m-1;
        }
        mylong tmp = mulltoint(al2, x),
            tmp2 = subl_unsign(curvalue, tmp);
        freelong(curvalue);
        curvalue = tmp2;
        freelong(tmp);
    }
    for (i = curvalue.size-1; i>=0; --i) {
        if (curvalue.d[i] != 0)
            break;
        curvalue.size--;
    }
	if (curvalue.size != 0 && al1sign ^ al2sign) {
		mylong old = curvalue;
		curvalue = subl_unsign(al2, curvalue);
		freelong(old);
	}

	curvalue.sign = al2sign;

    return curvalue;
}

