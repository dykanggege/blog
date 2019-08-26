#include<stdio.h>
#include<string.h>

#define cl 1

void mygetline(char arr[],int n);

int main() {
    char str[10];
    mygetline(str,10);
    printf("%d",strlen(str));
    return 0; //exit(0) 程序正常退出
}

void mygetline(char arr[],int n) {
    char c;
    int i;
    for(i = 0; i < n-1; i++){
        c = getchar();
        if(c == EOF || c == '\n'){
            i++;
            break;
        }
        arr[i] = c;
    }
    arr[i] = '\0';
}