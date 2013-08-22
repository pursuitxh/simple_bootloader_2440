/**
 * print.c:
 * Copyright (C) 2009  david leels <davidontech@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "div64.h"
#include "print.h"
#include "uart.h"

static char print_buf[1024];

void * memcpy(void * dest,const void *src,unsigned int count)
{
	char *tmp = (char *) dest, *s = (char *) src;
	while (count--)
		*tmp++ = *s++;
	return dest;
}

static char *number(char *str, int num,int base,unsigned int flags){
	int i=0;
	int sign=0;

	if(FORMAT_SIGNED(flags)&&(signed int)num<0){
		sign=1;
		num=~num+1;
	}

	do{
		numbers[i++]=digits[do_div(num,base)];
	}while(num!=0);


	if(FORMAT_BASE(flags)==FORMAT_BASE_O){
		numbers[i++]='0';
	}else if(FORMAT_BASE(flags)==FORMAT_BASE_X){
		numbers[i++]='x';
		numbers[i++]='0';
	}else if(FORMAT_BASE(flags)==FORMAT_BASE_B){
		numbers[i++]='b';
		numbers[i++]='0';
	}
	if(sign)
		numbers[i++]='-';

	while (i-- > 0)
			*str++ = numbers[i];
	return str;
}

static int format_decode(const char *fmt,unsigned int *flags){
	const char *start = fmt;

	*flags &= ~FORMAT_TYPE_MASK;
	*flags |= FORMAT_TYPE_NONE;
	for (; *fmt ; ++fmt) {
		if (*fmt == '%')
		break;
	}

	if (fmt != start || !*fmt)
		return fmt - start;

	do{
		fmt++;
		switch(*fmt){
			case 'l':
				SET_FORMAT_FLAG(*flags,FORMAT_FLAG_WIDTH);
				break;
			default:
				break;
		}
	}while(0);

	SET_FORMAT_BASE(*flags,FORMAT_BASE_D);
	switch (*fmt) {
		case 'c':
			SET_FORMAT_TYPE(*flags,FORMAT_TYPE_CHAR);
			break;

		case 's':
			SET_FORMAT_TYPE(*flags,FORMAT_TYPE_STR);
			break;

		case 'o':
			SET_FORMAT_BASE(*flags,FORMAT_BASE_O);
			SET_FORMAT_TYPE(*flags,FORMAT_TYPE_UINT);
			break;

		case 'x':
		case 'X':
			SET_FORMAT_BASE(*flags,FORMAT_BASE_X);
			SET_FORMAT_TYPE(*flags,FORMAT_TYPE_UINT);
			break;

		case 'd':
		case 'i':
			SET_FORMAT_TYPE(*flags,FORMAT_TYPE_INT);
			SET_FORMAT_BASE(*flags,FORMAT_BASE_D);
			break;
		case 'u':
			SET_FORMAT_TYPE(*flags,FORMAT_TYPE_UINT);
			SET_FORMAT_BASE(*flags,FORMAT_BASE_D);
			break;

	default:
		break;
	}
	return ++fmt-start;
}

int vsnprintf(char *buf, int size, const char *fmt, va_list args){
	int num;
	char *str, *end, c,*s;
	int read;
	unsigned int spec=0;

	str = buf;
	end = buf + size;

	if (end < buf) {
		end = ((void *)-1);
		size = end - buf;
	}

	while (*fmt) {
		const char *old_fmt = fmt;

		read = format_decode(fmt, &spec);
		fmt += read;

		if((FORMAT_TYPE(spec))==FORMAT_TYPE_NONE){
			int copy = read;
			if (str < end) {
				if (copy > end - str)
					copy = end - str;
					memcpy(str, old_fmt, copy);
			}
			str += read;

		}else if(spec&FORMAT_FLAG_WIDTH){
			//do nothing
		}else if(FORMAT_TYPE(spec)==FORMAT_TYPE_CHAR){
			c = (unsigned char) va_arg(args, int);
			if (str < end)
				*str = c;
			++str;
		}else if(FORMAT_TYPE(spec)==FORMAT_TYPE_STR){
			s = (char *) va_arg(args, char *);
			while(str<end&&*s!='\0'){
				*str++=*s++;
			}
		}else{
			if(FORMAT_TYPE(spec)==FORMAT_TYPE_INT){
				num = va_arg(args, int);
			}else if(FORMAT_TYPE(spec)==FORMAT_TYPE_ULONG){
				num = va_arg(args, unsigned long);
			}else if(FORMAT_TYPE(spec)==FORMAT_TYPE_LONG){
				num = va_arg(args, long);
			}else if(FORMAT_TYPE(spec)==FORMAT_TYPE_SIZE_T){
				num = va_arg(args, int);
			}else if(FORMAT_TYPE(spec)==FORMAT_TYPE_USHORT){
				num = (unsigned short) va_arg(args, int);
			}else if(FORMAT_TYPE(spec)==FORMAT_TYPE_SHORT){
				num = (short) va_arg(args, int);
			}else{
				num = va_arg(args, unsigned int);
			}
				str=number(str,num,spec&FORMAT_BASE_MASK,spec);
		}
	}
	if (size > 0) {
		if (str < end)
			*str = '\0';
		else
			end[-1] = '\0';
	}
	return str-buf;
}

void printf(const char *fmt, ...)
{
	va_list args;
	unsigned int i;

	va_start (args, fmt);
	i = vsnprintf (print_buf, sizeof(print_buf),fmt, args);
	va_end (args);

	put_uart(print_buf, i);
}
