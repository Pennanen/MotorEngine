#ifndef ESASSERT_H_
#define ESASSERT_H_

namespace engine
{
	void yamAssert(const char* expression, const char* file, int line );
}
//#include <assert.h>
//#undef assert
#define assert(_Expression) (void)( (!!(_Expression)) || (yam2d::yamAssert(#_Expression, __FILE__, __LINE__), 0) )




#endif