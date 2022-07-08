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

#include "MovingAvgFilter.h"

void MovingAvgFilter::begin (int length)
{
	_length = length;
	
	_filter_buff = (float *) malloc (sizeof (float) * length);
	
	if (_filter_buff != NULL)
	{
		for (int i = 0; i < _length; i++)
		{
			_filter_buff[i] = 0.0;
		}
	}
}

void MovingAvgFilter::initValue (float value)
{	
	if (_filter_buff == NULL)
		return;
		
	for (int i = 0; i < _length; i++)
	{
		_filter_buff[i] = value;
	}
	
	return;
}

void MovingAvgFilter::reset (void)
{
	if (_filter_buff == NULL)
		return;
		
	for (int i = 0; i < _length; i++)
	{
		_filter_buff[i] = 0.0;
	}
	
	_ptr = 0;
	_avg = 0;
	
	return;
}

int MovingAvgFilter::getFilterPos (void)
{
	return _ptr;
}

float MovingAvgFilter::getAvg (void)
{
	return _avg;
}

float MovingAvgFilter::calculateAvg (float value)
{
	float sum = 0;
	
	if (_ptr  >= _length)
		_ptr = 0;
	
	_filter_buff[_ptr] = value;
	_ptr++;
	
	for (int i = 0; i < _length; i++)
	{
		sum += _filter_buff[i];
	}
	
	_avg = sum / _length;
	
	return _avg;
}

void MovingAvgFilter::release (void)
{

	free (_filter_buff);
	_filter_buff = NULL;
	_ptr = 0;
	_avg = 0;
	
	return;
}
