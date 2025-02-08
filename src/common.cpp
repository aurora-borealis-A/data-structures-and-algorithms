#include"common.h"

// 指定维度，初始化方阵
math::Matrix::Matrix(int dim){
    data = new int*[dim];
    for(int i=0; i<dim; i++){
        this->data[i] = new int[dim]();
    }
    this->n_row = dim;
    this->n_col = dim;
}
// 指定行、列数，初始化矩阵
math::Matrix::Matrix(int rows, int cols){
    data = new int*[rows];
    for(int i=0; i<rows; i++){
        this->data[i] = new int[cols]();
    }
    this->n_row = rows;
    this->n_col = cols;
}

// 指定行、列数与初始值来初始化矩阵
math::Matrix::Matrix(int rows, int cols, int init_num){
    Matrix(rows, cols);
    for(int i=0; i<rows; i++)
    for(int j=0; j<rows; j++)
        this->data[i][j] = init_num;
}

// 重载索引
int& math::Matrix::operator()(int r, int c){
    if(r < 0 || r >= this->n_row || c < 0 || c >= this->n_col) return this->data[0][0];
    return this->data[r][c];
}

// 打印矩阵
void math::Matrix::print(){
    for(int i=0; i<this->n_row; i++){
        for(int j=0; j<this->n_col; j++){
            std::cout << this->data[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

// 析构函数
math::Matrix::~Matrix(){
    for (int i = 0; i < this->n_row; ++i) {
        delete[] this->data[i]; // 释放每一行的内存
    }
    delete[] this->data;
}

