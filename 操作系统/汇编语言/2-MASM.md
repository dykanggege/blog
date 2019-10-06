现在要正式开始写代码了

# 汇编
有了指令集就可以在之上抽象汇编语法了，微软推出了MASM汇编器及其语法，基于x86架构16bit或32bit，用于window系统

还有其他的例如NASM用于linux，甚至还有FASM同时适用于两个系统，注意MASM和NASM的语法有些差异，我们选择使用主流的MASM

自行解决在linux安装MASM汇编器

segment、ends 定义一个段的开始和结束，操作的地址就是对应段内的地址

```
assume cs:codesg 设置cs寄存器作为codesg的段寄存器

会将代码文本、数据、栈分别放到不同的段内
codesg segment codesg段开始

    mov ax,0123H
    mov bx,0456H
    add ax,bx
    add ax,ax

    mov ax,4c00H
    int 21H

codesg ends 段结束

end 汇编结束
```

将没有注释的这个程序使用MASM.EXE汇编，第一个参数是要汇编的文件，后面的都回车忽略，应该会的到一个 1.obj 目标文件

接着要使用LINK.EXE将1.ojb连接为1.exe，中间关于链接的知识都先忽略

在文件后面加上分号 1; 可以忽略中间文件生成以及链接部分

debug 文件名   开始调试

- a(Assemble)	逐行汇编
- r(Register)	显示和修改寄存器
- t(Trace)	跟踪执行
- q(Quit)	结束
- g 0012 将ip0012前面的指令都执行完
- p 把loop执行完，直至cx==0

# 寄存器

![](img/7.jpeg)

## 通用寄存器：
AX，BX，CX，DX 称作为数据寄存器，可分为AL,AH使用，以DS作为段前缀：

- AX (Accumulator)：累加寄存器，一般用于中转，不可用于[]寻址；
- BX (Base)：基地址寄存器,用于[]内存寻址
- CX (Count)：计数器寄存器，一般用于loop计数
- DX (Data)：数据寄存器，可用于[]寻址；

SP 和 BP 又称作为指针寄存器，一般用于栈操作：

- SP (Stack Pointer)：堆栈指针寄存器；
- BP (Base Pointer)：基指针寄存器，可用于[]内存寻址，默认段前缀SS；

SI 和 DI 又称作为变址寄存器，可只用[]用于内存寻址，或相加寻址：

- SI (Source Index)：源变址寄存器；
- DI (Destination Index)：目的变址寄存器；

控制寄存器：

- IP (Instruction Pointer)：指令指针寄存器，以CS作为段前缀；
- FLAG：标志寄存器；

段寄存器：
- 不能直接被内存赋值，需要寄存器中转
- 不能用于[]内存寻址，只能作为段前缀

- CS (Code Segment)：代码段寄存器；
- DS (Data Segment)：数据段寄存器；
- SS (Stack Segment)：堆栈段寄存器；
- ES (Extra Segment)：附加段寄存器；