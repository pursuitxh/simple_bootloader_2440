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
#include "stdio.h"
#include "uart.h"
#include "type.h"
#include "ctype.h"

#define unlikely(x) __builtin_expect(!!(x), 0)

static const char *digits="0123456789abcdef";
static char numbers[68];
static char print_buf[1024];
static char scanf_buf[1024];

unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (!base) {
		base = 10;
		if (*cp == '0') {
			base = 8;
			cp++;
			if ((toupper(*cp) == 'X') && isxdigit(cp[1])) {
				cp++;
				base = 16;
			}
		}
	} else if (base == 16) {
		if (cp[0] == '0' && toupper(cp[1]) == 'X')
			cp += 2;
	}
	while (isxdigit(*cp) &&
	       (value = isdigit(*cp) ? *cp-'0' : toupper(*cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

long simple_strtol(const char *cp,char **endp,unsigned int base)
{
	if(*cp=='-')
		return -simple_strtoul(cp+1,endp,base);
	return simple_strtoul(cp,endp,base);
}

unsigned long long simple_strtoull(const char *cp,char **endp,unsigned int base)
{
	unsigned long long result = 0,value;

	if (!base) {
		base = 10;
		if (*cp == '0') {
			base = 8;
			cp++;
			if ((toupper(*cp) == 'X') && isxdigit(cp[1])) {
				cp++;
				base = 16;
			}
		}
	} else if (base == 16) {
		if (cp[0] == '0' && toupper(cp[1]) == 'X')
			cp += 2;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

long long simple_strtoll(const char *cp,char **endp,unsigned int base)
{
	if(*cp=='-')
		return -simple_strtoull(cp+1,endp,base);
	return simple_strtoull(cp,endp,base);
}

static int skip_atoi(const char **s)
{
	int i=0;

	while (isdigit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

void * memcpy(void * dest,const void *src,unsigned int count)
{
	char *tmp = (char *) dest, *s = (char *) src;
	while (count--)
		*tmp++ = *s++;
	return dest;
}

char *number(char *str, int num,int base,unsigned int flags){
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

int format_decode(const char *fmt,unsigned int *flags){
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

	uart_queue(print_buf, i);
}

int vsscanf(const char * buf, const char * fmt, va_list args)
{
	const char *str = buf;
	char *next;
	char digit;
	int num = 0;
	int qualifier;
	int base;
	int field_width;
	int is_sign = 0;

	while(*fmt && *str) {
		/* skip any white space in format */
		/* white space in format matchs any amount of
		 * white space, including none, in the input.
		 */
		if (isspace(*fmt)) {
			while (isspace(*fmt))
				++fmt;
			while (isspace(*str))
				++str;
		}

		/* anything that is not a conversion must match exactly */
		if (*fmt != '%' && *fmt) {
			if (*fmt++ != *str++)
				break;
			continue;
		}

		if (!*fmt)
			break;
		++fmt;

		/* skip this conversion.
		 * advance both strings to next white space
		 */
		if (*fmt == '*') {
			while (!isspace(*fmt) && *fmt)
				fmt++;
			while (!isspace(*str) && *str)
				str++;
			continue;
		}

		/* get field width */
		field_width = -1;
		if (isdigit(*fmt))
			field_width = skip_atoi(&fmt);

		/* get conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
		    *fmt == 'Z' || *fmt == 'z') {
			qualifier = *fmt++;
			if (unlikely(qualifier == *fmt)) {
				if (qualifier == 'h') {
					qualifier = 'H';
					fmt++;
				} else if (qualifier == 'l') {
					qualifier = 'L';
					fmt++;
				}
			}
		}
		base = 10;
		is_sign = 0;

		if (!*fmt || !*str)
			break;

		switch(*fmt++) {
		case 'c':
		{
			char *s = (char *) va_arg(args,char*);
			if (field_width == -1)
				field_width = 1;
			do {
				*s++ = *str++;
			} while (--field_width > 0 && *str);
			num++;
		}
		continue;
		case 's':
		{
			char *s = (char *) va_arg(args, char *);
			if(field_width == -1)
				field_width = INT_MAX;
			/* first, skip leading white space in buffer */
			while (isspace(*str))
				str++;

			/* now copy until next white space */
			while (*str && !isspace(*str) && field_width--) {
				*s++ = *str++;
			}
			*s = '\0';
			num++;
		}
		continue;
		case 'n':
			/* return number of characters read so far */
		{
			int *i = (int *)va_arg(args,int*);
			*i = str - buf;
		}
		continue;
		case 'o':
			base = 8;
			break;
		case 'x':
		case 'X':
			base = 16;
			break;
		case 'i':
                        base = 0;
		case 'd':
			is_sign = 1;
		case 'u':
			break;
		case '%':
			/* looking for '%' in str */
			if (*str++ != '%')
				return num;
			continue;
		default:
			/* invalid format; stop here */
			return num;
		}

		/* have some sort of integer conversion.
		 * first, skip white space in buffer.
		 */
		while (isspace(*str))
			str++;

		digit = *str;
		if (is_sign && digit == '-')
			digit = *(str + 1);

		if (!digit
                    || (base == 16 && !isxdigit(digit))
                    || (base == 10 && !isdigit(digit))
                    || (base == 8 && (!isdigit(digit) || digit > '7'))
                    || (base == 0 && !isdigit(digit)))
				break;

		switch(qualifier) {
		case 'H':	/* that's 'hh' in format */
			if (is_sign) {
				signed char *s = (signed char *) va_arg(args,signed char *);
				*s = (signed char) simple_strtol(str,&next,base);
			} else {
				unsigned char *s = (unsigned char *) va_arg(args, unsigned char *);
				*s = (unsigned char) simple_strtoul(str, &next, base);
			}
			break;
		case 'h':
			if (is_sign) {
				short *s = (short *) va_arg(args,short *);
				*s = (short) simple_strtol(str,&next,base);
			} else {
				unsigned short *s = (unsigned short *) va_arg(args, unsigned short *);
				*s = (unsigned short) simple_strtoul(str, &next, base);
			}
			break;
		case 'l':
			if (is_sign) {
				long *l = (long *) va_arg(args,long *);
				*l = simple_strtol(str,&next,base);
			} else {
				unsigned long *l = (unsigned long*) va_arg(args,unsigned long*);
				*l = simple_strtoul(str,&next,base);
			}
			break;
		case 'L':
			if (is_sign) {
				long long *l = (long long*) va_arg(args,long long *);
				*l = simple_strtoll(str,&next,base);
			} else {
				unsigned long long *l = (unsigned long long*) va_arg(args,unsigned long long*);
				*l = simple_strtoull(str,&next,base);
			}
			break;
		case 'Z':
		case 'z':
		{
			u32 *s = (u32*) va_arg(args,u32*);
			*s = (u32) simple_strtoul(str,&next,base);
		}
		break;
		default:
			if (is_sign) {
				int *i = (int *) va_arg(args, int*);
				*i = (int) simple_strtol(str,&next,base);
			} else {
				unsigned int *i = (unsigned int*) va_arg(args, unsigned int*);
				*i = (unsigned int) simple_strtoul(str,&next,base);
			}
			break;
		}
		num++;

		if (!next)
			break;
		str = next;
	}
	return num;
}

int scanf(const char * fmt, ...)
{
	int i = 0;
	unsigned char c;
	va_list args;

	while(1)
	{
		c = getc();
		if((c == 0x0d) || (c == 0x0a))
		{
			scanf_buf[i] = '\0';
			break;
		}
		else
		{
			scanf_buf[i++] = c;
		}
	}

	va_start(args,fmt);
	i = vsscanf((char *)scanf_buf,fmt,args);
	va_end(args);

	return i;
}
