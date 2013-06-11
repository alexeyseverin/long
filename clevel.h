typedef struct _mylong {
    int sign;
    int *d;
    int size;
} mylong; 
mylong readl(const char *);
void writel(mylong, const char *);
void freelong(mylong);
mylong alloclong(int);
