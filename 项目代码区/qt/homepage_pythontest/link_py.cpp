#include "link_py.h"
#include <QDebug>
#include <Python.h> // 包含Python头文件
#include <QDebug>

link_py::link_py(QObject *parent)
    : QObject(parent),
    process(new QProcess(this))
{
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &link_py::onProcessFinished);
}

void link_py::fetchData(const QString &ct, int startYear, int startMonth, int endYear, int endMonth)
{
    Py_SetPythonHome(L"D:\\Python\\libs");
    // 初始化Python解释器
    Py_Initialize();
    // 导入Python模块（文件名）
    PyObject* pModule = PyImport_ImportModule("demo.py");
    if (pModule)
    {
        // 从模块中获取函数
        PyObject* demo = PyObject_GetAttrString(pModule, "check_parameters");
        if (demo && PyCallable_Check(demo))
        {
            // 创建包含一个QString参数和四个整型参数的元组
            PyObject* pArgs = PyTuple_Pack(5, PyUnicode_FromString(ct.toStdString().c_str()), PyLong_FromLong(startYear), PyLong_FromLong(startMonth), PyLong_FromLong(endYear), PyLong_FromLong(endMonth));
            // 调用Python函数，并获取返回值
            PyObject* pValue = PyObject_CallObject(demo, pArgs);
            Py_DECREF(pArgs);  // 释放参数对象
            if (pValue != nullptr)
            {
                // 输出函数调用结果
                qDebug() << "Result of call: " << PyUnicode_AsUTF8(pValue);
                Py_DECREF(pValue);  // 释放返回值对象
            }
            else
            {
                Py_DECREF(demo);   // 释放函数对象
                Py_DECREF(pModule); // 释放模块对象
                PyErr_Print();      // 打印错误信息
                qDebug() << "Call failed";
            }
        }
        else
        {
            if (PyErr_Occurred())
                PyErr_Print();  // 打印错误信息
            qDebug() << "Cannot find function 'check_parameters'";
        }
        Py_XDECREF(demo);  // 释放函数对象
        Py_DECREF(pModule); // 释放模块对象
    }
    else
    {
        PyErr_Print();  // 打印错误信息
        qDebug() << "Failed to load 'demo.py'";
    }
    // 关闭Python解释器
    Py_Finalize();
}

// void link_py::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
// {
//     if (exitStatus == QProcess::NormalExit && exitCode == 0) {
//         qDebug() << "Python 脚本执行成功";
//     } else {
//         qDebug() << "Python 脚本执行失败";
//     }
// }
