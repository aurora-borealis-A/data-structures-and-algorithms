#pragma once
#include<iostream>

template <typename data_type>
class Vector{
    int _size;  // 存储的元素个数
    int _capasity;  // 容量
    data_type *_data;
public:
    // 构造函数
    Vector(int size=0, data_type init_value=data_type()):_size(size){
        this->_capasity = size*2 + 2;
        this->_data = new data_type[_capasity];
        for(int i=0; i<size; i++){
            _data[i] = init_value;
        }
    }
    
    // 析构
    ~Vector(){
        delete[] this->_data;
    }

    // 重载索引
    data_type& operator[](int index){
        if(index < 0 or index >= this->_size) throw "下标越界";
        return _data[index];
    }

    // 尾部插入元素
    void push_back(data_type new_data){
        // 如果容量不够了就扩容
        if(this->_size == this->_capasity){
            // 保存旧空间的信息
            int old_capasity = _capasity;
            data_type *old_data = _data; 

            // 分配新空间，并将旧空间中的元素拷贝到新空间
            _capasity = 2*_capasity;
            this->_data = new data_type[_capasity];
            for(int i=0; i<old_capasity; i++){
                _data[i] = old_data[i];
            }
            
            // 删除旧空间
            delete[] old_data;
        }

        // 插入新元素
        this->_data[_size] = new_data;
        (this->_size)++;
    }
    
    // 尾部删除元素
    void pop_back(){
        // 判断是否无元素了
        if(_size == 0) throw "溢出";
        // 插入新元素
        _size--;
    }
    
    // 返回尾部元素
    data_type& back(){
        return *(_data+_size-1);
    }

    // 返回当前容器容量
    int capasity(){
        return this->_capasity;
    }

    // 返回当前容器容量
    int size(){
        return this->_size;
    }

    // 迭代器
    class Iterator{
        data_type *ptr;
    public:
        // 构造函数
        Iterator(data_type *ptr):ptr(ptr){}

        // 前自增
        Iterator& operator++() {
            ptr++;
            return *this;
        }

        // 后自增
        Iterator& operator++(int){
            int &temp = *this;
            ptr++;
            return temp;
        }

        data_type& operator*(){
            return *(this->ptr);
        }

        // 相等
        bool operator==(Iterator other){
            if(ptr == other.ptr)
                return true;
            return false;
        }

        // 不相等
        bool operator!=(Iterator other){
            if(ptr == other.ptr)
                return false;
            return true;
        }
    };

    // 迭代起点
    Iterator begin(){
        return Iterator(_data);
    }

    // 迭代终点
    Iterator end(){
        return Iterator(_data+_size);
    }


};
