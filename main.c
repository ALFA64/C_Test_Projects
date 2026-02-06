<<<<<<< HEAD
#ifdef __linux__
#include <stdarg.h>

typedef unsigned long size_t;
typedef long ssize_t;

#endif

#ifdef _WIN32
#include <windows.h>
#include <stdarg.h>
typedef long ssize_t;
typedef unsigned long size_t;
#endif

#ifdef __linux__

=======
#include <stdarg.h>
#include <stddef.h>

#ifdef __linux__
#include <sys/types.h>
>>>>>>> 36dc2fd (Update program)
#define SYS_READ 0
#define SYS_WRITE 1

ssize_t sys_read(int fd, void *buf, size_t count){
	ssize_t ret;
	__asm__ volatile(
		"syscall"
		: "=a"(ret)
		: "a"(SYS_READ), "D"(fd), "S"(buf), "d"(count)
		: "rcx", "r11", "memory"
	);
	return ret;
}


ssize_t sys_write(int fd,const void *buf, size_t count){
	ssize_t ret;
	__asm__ volatile(
		"syscall"
		: "=a"(ret)
		: "a"(SYS_WRITE), "D"(fd), "S"(buf), "d"(count)
		: "rcx", "r11", "memory"
	);
	return ret;
}

#endif

#ifdef _WIN32
<<<<<<< HEAD
=======
#include <windows.h>
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
>>>>>>> 36dc2fd (Update program)

static HANDLE get_handle(int fd){
	if (fd == 0) return GetStdHandle(STD_INPUT_HANDLE);
	if (fd == 1) return GetStdHandle(STD_OUTPUT_HANDLE);
	if (fd == 2) return GetStdHandle(STD_ERROR_HANDLE);
	return INVALID_HANDLE_VALUE;
<<<<<<< HEAD
=======
}
>>>>>>> 36dc2fd (Update program)

ssize_t sys_read(int fd, void *buf, size_t count){
	HANDLE h = get_handle(fd);
	DWORD read_bytes = 0;

	if(!ReadFile(h, buf, (DWORD)count, &read_bytes, NULL))
		return -1;

	return (ssize_t)read_bytes;
}

ssize_t sys_write(int fd, const void *buf, size_t count){
	HANDLE h = get_handle(fd);
	DWORD written = 0;

	if(!WriteFile(h, buf, (DWORD)count, &written, NULL))
		return -1;

	return (ssize_t)written;
}

#endif

void skip_spaces(char **p){
	while(**p == ' ' || **p == '\n' || **p == '\t')
		(*p)++;
}

void print_str(const char *s) {
	const char *p = s;
	while (*p) p++;
	sys_write(1,s,p-s);
}

int my_atoi(char **p){
	int sign = 1, val = 0;
	if (**p == '-'){
		sign = -1;
		(*p)++;
	}

	while (**p >= '0' && **p <= '9'){
		val = val * 10 + (**p - '0');
		(*p)++;
	}

	return val * sign;
}

void my_string(char **p, char *dest){
	while(**p && **p != ' ' && **p != '\n'){
		*dest++ = **p;
		(*p)++;
	}
	*dest = 0;
}

int my_scanf(const char *fmt, ...){
	char buffer[256];
	ssize_t n = sys_read(0,buffer, 255);
	buffer[n] = 0;

	char *p = buffer;

	va_list args;
	va_start(args, fmt);

	int assigned = 0;

	while (*fmt){
		if(*fmt == '%'){
			fmt++;
			skip_spaces(&p);

			if (*fmt == 'd'){
				int *ip = va_arg(args, int *);
				*ip = my_atoi(&p);
				assigned++;
			}
			else if (*fmt == 's') {
				char *sp = va_arg(args, char *);
				my_string(&p, sp);
				assigned++;
			}
		}
		fmt++;
	}

	va_end(args);
	return assigned;
}

void print_int(int x){
	char buf[16];
	int i = 0;

	if(x == 0){
		sys_write(1,"0",1);
		return;
	}

	if(x<0){
		sys_write(1,"-",1);
		x= -x;
	}

	while(x){
		buf[i++] = (x%10) + '0';
		x/=10;
	}

	while (i--)
		sys_write(1,&buf[i],1);
}


struct Person{
	int age;
	char name[32];
};

int main(){

	struct Person people[3] = {0};

	print_str("DB Student: \n");
	for(int i=0; i<3; i++){
		print_str("Introdu varsta: ");
		my_scanf("%d", &people[i].age);
		print_str("Introdu numele: ");
		my_scanf("%s", &people[i].name);
	}

	for(int i=0; i<3; i++){
		print_str("\nStudent: ");
		print_int(i);
		print_str("\nVarsta = ");
		print_int(people[i].age);
		print_str("\nNume = ");
		print_str(people[i].name);
		print_str("\n");
	}
	return 0;
}
