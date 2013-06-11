typedef struct _mylong {
    int sign;
    int *d;
    int size;
} mylong; 
mylong readl(const char*);
mylong alloclong(int);
void freelong(mylong);
