#ifndef ALIGNED_MALLOC_H
#define ALIGNED_MALLOC_H

#include <stddef.h>
#include <malloc.h>

namespace CAMP {

static inline void * aligned_malloc( size_t size , size_t alignment = 16)
{	
	// allocate aligned memory...
	# ifdef _MSC_VER
		return ::_aligned_malloc(size, alignment);
	#else
		return ::memalign(alignment, size);
	#endif
}

static inline void aligned_free( void * data_ptr )
{
	if (data_ptr) {
		# ifdef _MSC_VER
				::_aligned_free(data_ptr);
		#else
				free(data_ptr);
		# endif

		data_ptr = NULL;
	}
}

} // end of namespace

#endif
