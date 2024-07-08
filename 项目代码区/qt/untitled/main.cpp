#include "mainwindow.h"

#include <Python.h>
#include <QApplication>
#include <iostream>

using namespace std;

void test_get_price(int time_window) {
    //添加搜索python安装路径
    Py_SetPythonHome(L"D:\\Anaconda\\envs\\pytorch");


    // 初始化Python解释器
    Py_Initialize();

    // 导入Python模块
    PyObject* pModule=PyImport_ImportModule("Untitled-1");// 文件名

    if (pModule) {
        // 获取Python函数
        PyObject* pFunc = PyObject_GetAttrString(pModule, "predict_temperature");//函数名

        if (pFunc && PyCallable_Check(pFunc)) {
            // 创建参数元组
            PyObject* pArgs = PyTuple_Pack(1, PyLong_FromLong(time_window));//传递一个整型参数

            // 调用Python函数
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            if (pValue != nullptr) {
                std::cout << "Result of call: " << PyLong_AsLong(pValue) << std::endl;
                Py_DECREF(pValue);
            } else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                std::cerr << "Call failed" << std::endl;
            }
        } else {
            if (PyErr_Occurred())
                PyErr_Print();
            std::cerr << "Cannot find function 'predict_temperature'" << std::endl;
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cerr << "Failed to load 'predict_temperature'" << std::endl;
    }

    // 关闭Python解释器
    Py_Finalize();
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    test_get_price(24);
    return a.exec();
}
