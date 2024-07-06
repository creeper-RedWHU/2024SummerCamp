# 注意事项
> 请将.pro文件26-30行地址改为你对应python环境的地址
> 所调用的python文件位于qt生成的.exe 程序所在的文件夹
> 请将python文件中数据集的地址改为你的地址


> 如果Python.h 头文件报错，请将python环境include文件夹object.h
```c
typedef struct{
    const char* name;
    int basicsize;
    int itemsize;
    unsigned int flags;
    PyType_Slot *slots; /* terminated by slot==0. */
} PyType_Spec;
```
改为
```c
#undef slots
typedef struct{
    const char* name;
    int basicsize;
    int itemsize;
    unsigned int flags;
    PyType_Slot *slots; /* terminated by slot==0. */
} PyType_Spec;
#define slots Q_SLOTS
```
并保存