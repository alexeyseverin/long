typedef struct _mylong {
    int sign;
    char *d;
    int size;
} mylong; 
mylong readl(const char *);
void writel(mylong, const char *);
void freelong(mylong);
mylong alloclong(int);
int comparel(mylong, mylong);
mylong subl(mylong, mylong);
mylong  subl_unsign(mylong, mylong);
mylong addl(mylong, mylong);
mylong addl_unsign(mylong, mylong);
mylong mull(mylong, mylong);
