%module cpplevel

%{
#define SWIG_FILE_WITH_INIT
#include "cpplevel.h"
%}

class Long {
private:
	mylong num;
public:
	Long(const char *);
	~Long();
	Long(const Long &);
	Long &operator =(const Long&); 
	const Long operator +(Long&);
	const Long operator -(Long&);
	Long operator -();
	const Long operator *(Long&);
	const Long operator /(Long&);
	const Long operator %(Long&);
	const Long operator ^(Long&);

	bool operator ==(const Long&);
	bool operator <(const Long&);
	bool operator >(const Long&);
	bool operator <=(const Long&);
	bool operator >=(const Long&);

	Long(const mylong&);
	const mylong& getnum() const;
	void outf(const char *);
	void out();
};
