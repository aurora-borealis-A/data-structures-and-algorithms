#pragma once
#include<iostream>
#include<string>

// 函数结果状态宏定义
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBILE -1
#define OVERFLOW -2

// Status 是函数的类型，其值是函数返回结果的状态码
typedef int Status;

// 交换两个变量的数值
template <typename T>
void swap(T &v1, T &v2){
    T tmp = v1;
    v1 = v2;
    v2 = tmp;
}

// 数学
namespace math{
    // 矩阵
    class Matrix{
    public:
        // 构造函数
        Matrix(int dim);  // 指定维度，初始化方阵
        Matrix(int rows, int cols);  // 指定行、列数，初始化矩阵
        Matrix(int rows, int cols, int init_num);  // 指定行、列数与初始值来初始化矩阵

        // 重载索引
        int& operator()(int r, int c);

        // 打印矩阵
        void print();
        
        // 析构函数
        ~Matrix();
    private:
        int **data;  // 二级指针，保存矩阵
        int n_row, n_col;
    };
}



