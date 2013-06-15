#include <string>
#include <cstdio>
#include "cpplevel.h"


Long::Long(const char* s) {
	num = readl(s);
}


const mylong &Long::getnum() const {
	return num;
}

Long::~Long() {
	if (num.d)
		freelong(num);
}

Long::Long(const mylong &a) {
	num = a;
}

Long::Long(const Long &a) {
	num = copylong(a.getnum());
}

Long &Long::operator =(const Long &a) {
	if (this == &a)
		return *this;
	freelong(num);
	num = copylong(a.getnum());
	return *this;
}


const Long Long::operator +(Long &a) {
	return Long(addl(num, a.getnum()));
}

const Long Long::operator -(Long &a) {
	return Long(subl(num, a.getnum()));
} 
Long Long::operator -() {
	return Long(negl(copylong(num)));
}

const Long Long::operator *(Long &a) {
	return Long(mull(num, a.getnum()));
}

const Long Long::operator /(Long &a) {
	return Long(divl(num, a.getnum()));
}

const Long Long::operator %(Long &a) {
	return Long(modl(num, a.getnum()));
}


bool Long::operator ==(const Long &b) {
	if (comparel(b.getnum(), num) == 0) 
		return true;
	else
		return false;
}

bool Long::operator >(const Long &b) {
	if (comparel(num, b.getnum()) == 1) 
		return true;
	else
		return false;
}

bool Long::operator <(const Long &b) {
	if (comparel(num, b.getnum()) == -1) 
		return true;
	else
		return false;
}
bool Long::operator <=(const Long &b) {
	if (comparel(num, b.getnum()) <= 0) 
		return true;
	else
		return false;
}
bool Long::operator >=(const Long &b) {
	if (comparel(num, b.getnum()) >= 0) 
		return true;
	else
		return false;
}

void Long::outf(const char * s) {
	writel(num, s);
}

void Long::out() {
	outl(num);
}
