#include<stdio.h>

typedef struct {
float x;
float y;
}point;

int main(int arg, char** argv){
	puts("Hello, World");
	char buf[64];
	point p;
	p.x = 1;
	p.y = 2;
	float length = p.x*p.y;
	if (length>0){
	snprintf(buf, sizeof(buf), "length = %f",length);
	puts(buf);
	}else {
		puts("Value too small");
	}
	while(length > 0.0){
	puts("Loops");
	length = length - 1;
	}
	return 0;

}
