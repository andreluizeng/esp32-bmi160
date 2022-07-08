/* Copyright (C) 2022 Andre Silva. All rights reserved.

 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").

 Contact information
 -------------------

 Andre Silva
 Web      :  http://github.com/andreluizeng
 e-mail   :  andreluizeng@yahoo.com.br
 */

#ifndef _MovingAvgFilter_h_
#define _MovingAvgFilter_h_

#include <malloc.h>

class MovingAvgFilter 
{
	public:
        MovingAvgFilter(void)
			: _length{0}, _ptr{0}, _avg{0}, _filter_buff {NULL} {}
            
		void begin (int length);
		void initValue (float value);
		float calculateAvg (float value);
		float getAvg (void);
		int getFilterPos (void);
		void reset (void); 
		void release (void); //free filter's memory

	private:
		int _length;			// size of the filter
		int _ptr;				// index pointer
		float _avg;				// output data
		float *_filter_buff; 	// filter buffer

};

#endif
