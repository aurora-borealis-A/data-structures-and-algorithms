#pragma once
#include<iostream>
#include"vector.h"
#include<vector>
#include<queue>
#include<functional>
#include<yaml-cpp/yaml.h>
#include"common.h"

#define MaxInt 32767  // 最大整数，表示无穷
#define MVnum 100 // 最多图顶点个数
typedef std::string VType;  // 顶点数据类型
typedef int AType;  // 边数据类型

// 图的类型
enum GraphCat{
    UDG,    // 无向图
    DG,     // 有向图
    UDN,    // 无向网
    DN,     // 有向网
};
// 将图枚举类型转换为字符串
std::string gtype2str(GraphCat g_type);
// 将字符串转换为图枚举类型
GraphCat str2gtype(std::string g_type);

// 图 基类
template <typename VerType, typename ArcType>
class Graph{
public:
    Graph(std::string name="图"):name(name){}
    virtual ~Graph() = default;
    virtual Status create(std::string file)=0;  // 从yaml文件中构造图
    virtual Status createUDG_ui()=0;    // 构造无向图
    virtual Status createDG_ui()=0;     // 构造有向图
    virtual Status createUDN_ui()=0;    // 构造无向网
    virtual Status createDN_ui()=0;     // 构造有向网
    virtual void print()=0;  // 打印图结构
    virtual int locate_vertex(VerType v)=0;     // 根据顶点名称找到索引
    virtual VerType& operator[](int v_index)=0; // 根据顶点索引找到名称

    std::string name;   // 图的名称
    GraphCat type;     // 图的类型
    int vertex_num, arc_num;  // 图的顶点数与边数
};
    
// 邻接矩阵图
template <typename VerType, typename ArcType>
class AMGraph: public Graph<VerType, ArcType>{
public:
    // 构造函数
    AMGraph(std::string name="邻接矩阵"):Graph<VerType, ArcType>(name){}

    // 析构函数
    // ~AMGraph(){}

    // 创建图
    Status create(std::string file){
        YAML::Node graph = YAML::LoadFile(file);
        this->type = str2gtype(graph["graph type"].as<std::string>());

        // 输入图的总顶点数与边数
        this->vertex_num = graph["vertex num"].as<int>();
        this->arc_num = graph["arc num"].as<int>();

        // 将各顶点信息存入顶点表中
        YAML::Node vertices = graph["vertices"];
        int v_index = 0;
        for(auto vertex:vertices){
            this->vexes[v_index] = vertex.as<VerType>();
            v_index++;
        }

        // 初始化邻接矩阵
        for(int i=0; i<this->vertex_num; i++)
        for(int j=0; j<this->vertex_num; j++){
            if(this->type == GraphCat::UDG or this->type == GraphCat::DG)
                this->arcs[i][j] = 0;
            else if(this->type == GraphCat::UDN or this->type == GraphCat::DN)
                this->arcs[i][j] = MaxInt;
        }

        // 构造邻接矩阵
        YAML::Node arcs = graph["arcs"];
        for(auto it=arcs.begin(); it!=arcs.end(); it++){
            // 读取边关联的两顶点与权值
            VerType v1 = (*it)["vi"].as<VerType>();		
            VerType v2 = (*it)["vj"].as<VerType>();		
            ArcType arc = (*it)["w"].as<ArcType>();

            // 根据顶点名称找到索引
            int v1_index = locate_vertex(v1);
            int v2_index = locate_vertex(v2);

            // 构造邻接矩阵
            this->arcs[v1_index][v2_index] = arc;
            if(this->type == GraphCat::UDG or this->type == GraphCat::UDN)
                this->arcs[v2_index][v1_index] = arc;
        }
        std::cout << "创建图成功!" << std::endl;
        return OK;
    
    }

    // 用于创建无向图
    Status createUDG_ui() override{
        this->type = GraphCat::UDG;

        // 输入图的总顶点数与边数
        std::cout << "请输入无向图顶点数与边数：" << std::endl;
        std::cin >> this->vertex_num >> this->arc_num;

        // 将各顶点信息存入顶点表中
        for(int i=0; i<this->vertex_num; i++){
            std::cout << "请输入第" << i+1 << "个顶点名称：" << std::endl;
            std::cin >> this->vexes[i];
        }

        // 初始化邻接矩阵
        for(int i=0; i<this->vertex_num; i++)
        for(int j=0; j<this->vertex_num; j++)
            this->arcs[i][j] = 0;

        // 构造邻接矩阵
        for(int i=0; i<this->arc_num; i++){
            VerType v1, v2;		
            std::cout << "请输入第" << i+1 << "条边关联的顶点名称" << std::endl;
            std::cin >> v1 >> v2;
            int v1_index = locate_vertex(v1);
            int v2_index = locate_vertex(v2);
            this->arcs[v1_index][v2_index] = 1;
            this->arcs[v2_index][v1_index] = 1;
        }
        std::cout << "创建无向图成功!" << std::endl;
        return OK;
    }

    // 用于创建有向图
    Status createDG_ui() override{
        this->type = GraphCat::DG;

        // 输入图的总顶点数与边数
        std::cout << "请输入有向图顶点数与边数：" << std::endl;
        std::cin >> this->vertex_num >> this->arc_num;

        // 将各顶点信息存入顶点表中
        for(int i=0; i<this->vertex_num; i++){
            std::cout << "请输入第" << i+1 << "个顶点名称：" << std::endl;
            std::cin >> this->vexes[i];
        }

        // 初始化邻接矩阵
        for(int i=0; i<this->vertex_num; i++)
        for(int j=0; j<this->vertex_num; j++)
            this->arcs[i][j] = 0;

        // 构造邻接矩阵
        for(int i=0; i<this->arc_num; i++){
            VerType v1, v2;		
            std::cout << "请输入第" << i+1 << "条边关联的顶点名称" << std::endl;
            std::cin >> v1 >> v2;
            int v1_index = locate_vertex(v1);
            int v2_index = locate_vertex(v2);
            this->arcs[v1_index][v2_index] = 1;
        }
        std::cout << "创建有向图成功!" << std::endl;
        return OK;
    }

    // 用于创建无向网UDN(undigraph net)
    Status createUDN_ui() override{
        this->type = GraphCat::UDN;

        // 输入图的总顶点数与边数
        std::cout << "请输入无向网顶点数与边数：" << std::endl;
        std::cin >> this->vertex_num >> this->arc_num;

        // 将各顶点信息存入顶点表中
        for(int i=0; i<this->vertex_num; i++){
            std::cout << "请输入第" << i+1 << "个顶点名称：" << std::endl;
            std::cin >> this->vexes[i];
        }

        // 初始化邻接矩阵
        for(int i=0; i<this->vertex_num; i++)
        for(int j=0; j<this->vertex_num; j++)
            this->arcs[i][j] = MaxInt;

        // 构造邻接矩阵
        for(int i=0; i<this->arc_num; i++){
            VerType v1, v2;		
            ArcType w;
            std::cout << "请输入第" << i+1 << "条边关联的顶点名称和权值" << std::endl;
            std::cin >> v1 >> v2 >> w;
            int v1_index = locate_vertex(v1);
            int v2_index = locate_vertex(v2);
            this->arcs[v1_index][v2_index] = w;
            this->arcs[v2_index][v1_index] = w;
        }
        std::cout << "创建无向网成功!" << std::endl;
        return OK;
    }

    // 用于创建有向网DN
    Status createDN_ui() override{
        this->type = GraphCat::DN;

        // 输入图的总顶点数与边数
        std::cout << "请输入有向网顶点数与边数：" << std::endl;
        std::cin >> this->vertex_num >> this->arc_num;

        // 将各顶点信息存入顶点表中
        for(int i=0; i<this->vertex_num; i++){
            std::cout << "请输入第" << i+1 << "个顶点名称：" << std::endl;
            std::cin >> this->vexes[i];
        }

        // 初始化邻接矩阵
        for(int i=0; i<this->vertex_num; i++)
        for(int j=0; j<this->vertex_num; j++)
            this->arcs[i][j] = MaxInt;

        // 构造邻接矩阵
        for(int i=0; i<this->arc_num; i++){
            VerType v1, v2;		
            ArcType w;
            std::cout << "请输入第" << i+1 << "条边关联的顶点名称和权值" << std::endl;
            std::cin >> v1 >> v2 >> w;
            int v1_index = locate_vertex(v1);
            int v2_index = locate_vertex(v2);
            this->arcs[v1_index][v2_index] = w;
        }
        std::cout << "创建有向网成功!" << std::endl;
        return OK;
    }

    // 打印邻接矩阵
    void print() override{
        char title[100];
        sprintf(title, "[%s]%s的邻接矩阵为：", gtype2str(this->type).c_str(), this->name.c_str());
        std::cout << title << std::endl;
        for(int i=0; i<this->vertex_num; i++){
            for(int j = 0; j<this->vertex_num; j++){
                std::cout << this->arcs[i][j] << "\t";	
            }
            std::cout << std::endl;
        }	
    }

    // 给定顶点名称，找到顶点的索引
    int locate_vertex(VerType v) override{
        for(int i=0; i<this->vertex_num; i++)
            if (this->vexes[i] == v) return i;
        return  -1;
    }

    // 根据顶点索引，返回顶点名称
    VerType& operator[](int v_index) override{
        return vexes[v_index];
    }

    // 深度优先遍历(对外接口)
    void DFS(VerType v, std::function<void(VerType)> func){
        int v_index = locate_vertex(v);
        Vector<bool> visited(this->vertex_num, false);  // 定义辅助数组，用于记录哪些顶点被访问过
        this->_dfs(v_index, func, visited);
    }

    // 深度优先遍历(递归算法)
    void _dfs(int v_index, std::function<void(VerType)> func, Vector<bool> &visited){
        visited[v_index] = true;
        func((*this)[v_index]);
        for(int adj_vindex=0; adj_vindex < this->vertex_num; adj_vindex++){
            if(arcs[v_index][adj_vindex] !=0 and !visited[adj_vindex]) this->_dfs(adj_vindex, func, visited);
        } 
    }

    // 深度优先遍历(对外接口)，迭代器版
    virtual void DFS2(VerType v, std::function<void(VerType)> func){
        Vector<bool> visited(this->vertex_num, false);  // 定义辅助数组，用于记录哪些顶点被访问过
        this->_dfs2(v, func, visited);
    }

    // 深度优先遍历(递归算法)，迭代器版
    virtual void _dfs2(VerType v, std::function<void(VerType)> func, Vector<bool> &visited){
        int v_index = locate_vertex(v);
        if(visited[v_index]) return;

        visited[v_index] = true;
        func(v);
        for(auto t=vbegin(v); t!=vend(v); t++){
            // std::cout << "*t:" << *t << std::endl;
            this->_dfs2(*t, func, visited);
        } 
    }

    // 广度优先遍历
    void BFS(VerType v, std::function<void(VerType)> func){
        // 准备工作
        int v_index = locate_vertex(v);  // 将顶点名称转换为整数类型的索引，提高效率
        std::queue<VerType> worklist;  // 定义栈，用于辅助遍历
        Vector<bool> visited(this->vertex_num, false);  // 定义辅助数组，记录遍历过的顶点

        // 开始遍历
        func(v); 
        worklist.push(v);
        visited[v_index] = true;
        while(!worklist.empty()){
            v = worklist.back();
            worklist.pop();
            for(auto it=this->vbegin(v); it!=this->vend(v); it++){
                v_index = locate_vertex(*it);
                if(!visited[v_index]){
                    func(*it);
                    worklist.push(*it);
                    visited[v_index] = true;
                }
            }
        }
    }

    friend class Iterator;
    class Iterator{ 
    public:
        // 构造函数
        Iterator(AMGraph &G, int v_index, int adj_vindex):
            G(G), v_index(v_index),
            adj_vindex(adj_vindex){}

        // 重载运算符
        // 前自增
        Iterator& operator++(){
            ArcType no_arc = 0;
            if(this->G.type == GraphCat::UDN or this->G.type == GraphCat::DN) no_arc = MaxInt;
            while(this->G.arcs[this->v_index][++adj_vindex] == no_arc and adj_vindex<this->G.vertex_num);
            return *this;
        }

        // 后自增
        Iterator& operator++(int){
            Iterator &temp = *this;
            ArcType no_arc = 0;
            if(this->G.type == GraphCat::UDN or this->G.type == GraphCat::DN) no_arc = MaxInt;
            while(this->G.arcs[this->v_index][++adj_vindex] == no_arc and adj_vindex<this->G.vertex_num);
            return temp;
        }

        // 解引用
        VerType& operator*(){
            return this->G.vexes[adj_vindex];
        }

        // 相等
        bool operator==(const Iterator& other){
            if(this->v_index == other.v_index &&
                this->adj_vindex == other.adj_vindex)
                return true;
            return false;
        }

        // 不相等
        bool operator!=(const Iterator& other){
            if(this->v_index == other.v_index &&
                this->adj_vindex == other.adj_vindex)
                return false;
            return true;
        }
    private:
        AMGraph &G;
        int v_index;
        int adj_vindex;
    };

    // 根据顶点v返回迭代起点
    virtual Iterator vbegin(VerType v){
        int v_index = locate_vertex(v);
        int adj_vindex = 0;
        ArcType no_arc = 0;
        if(this->type == GraphCat::UDN or this->type == GraphCat::DN) no_arc = MaxInt;
        while(arcs[v_index][adj_vindex] == no_arc and adj_vindex<this->vertex_num) adj_vindex++;
        return Iterator(*this, v_index, adj_vindex);
    }

    // 根据顶点v返回迭代终点
    virtual Iterator vend(VerType v){
        int v_index = locate_vertex(v);
        return Iterator(*this, v_index, this->vertex_num);
    }
    
protected:
    VerType vexes[MVnum];
	ArcType arcs[MVnum][MVnum];
};

// 邻接表图
template <typename VerType, typename ArcType>
class ALGraph: public Graph<VerType, ArcType>{
public:
    // 边结点
    struct ArcNode{
        int v_index;  // 保存表头顶点指向的顶点的数组索引
        ArcType arc;  // 保存边的权重（用于网）
        ArcNode* next_arc;  // 指向下一个邻接的边结点
    };

    // 顶点结点
    struct VNode{
        VerType vertex;  // 保存顶点信息
        ArcNode* first_arc;  // 指向第一个边结点
    };

    // 构造函数
    ALGraph(std::string name="邻接表"):Graph<VerType, ArcType>(name){}

    // 析构函数
    ~ALGraph () override{
        for(int i=0; i<this->vertex_num; i++){
            for(ArcNode* p=vexes[i].first_arc; p!=nullptr;){
                ArcNode *temp = p;
                p = p->next_arc;
                // std::cout << "删除：" << (*this)[temp->v_index] << " at " << temp << std::endl;
                delete temp;
            }
        }
    }

    // 使用十字链表创建图
    Status create(std::string file){
        YAML::Node graph = YAML::LoadFile(file);
        this->type = str2gtype(graph["graph type"].as<std::string>());

        // 输入图的总顶点数与边数
        this->vertex_num = graph["vertex num"].as<int>();
        this->arc_num = graph["arc num"].as<int>();

        // 构造顶点表
        YAML::Node vertices = graph["vertices"];
        int v_index = 0;
        for(auto vertex:vertices){
            this->vexes[v_index].vertex = vertex.as<VerType>();
            this->vexes[v_index].first_arc = nullptr;
            v_index++;
        }

        // 构造十字链表
        YAML::Node arcs = graph["arcs"];
        for(auto arc_info: arcs){
            // 读取弧关联的两顶点与权值
            VerType vi = arc_info["vi"].as<VerType>();		
            VerType vj = arc_info["vj"].as<VerType>();		
            ArcType arc = arc_info["w"].as<ArcType>();
            
            // 找到两个顶点的索引
            int i_vindex = locate_vertex(vi);
            int j_vindex = locate_vertex(vj);

            // 创建顶点并插入
            ArcNode *p = new ArcNode;
            p->arc = arc;
            p->v_index = j_vindex;
            p->next_arc = this->vexes[i_vindex].first_arc;  // 插入顶点
            this->vexes[i_vindex].first_arc = p;

            if(this->type == GraphCat::UDG or this->type == GraphCat::UDN){
                p = new ArcNode;
                p->arc = arc;
                p->v_index = i_vindex;
                p->next_arc = this->vexes[j_vindex].first_arc;  // 插入顶点
                this->vexes[j_vindex].first_arc = p;
            }
        }

        std::cout << "创建图成功！" << std::endl;
        return OK;
    }

    // 构建无向图邻接表
    Status createUDG_ui() override{
        this->type = GraphCat::UDG; 

        // 输入无向图总顶点数与边数
        std::cout << "请输入无向图总顶点数与边数" << std::endl;
        std::cin >> this->vertex_num >> this->arc_num;

        // 构造顶点表
        for(int i=0; i<this->vertex_num; i++){
            std::cout << "请输入第" << i+1 << "个顶点的名称" << std::endl;	
            std::cin >> this->vexes[i].vertex;
            this->vexes[i].first_arc = nullptr;
        }

        // 构造邻接表
        for(int i=0; i<this->arc_num; i++){
            // 读入关联边
            VerType v1, v2;
            std::cout << "请输入第" << i+1 << "条边关联的顶点" << std::endl;
            std::cin >> v1 >> v2;

            // 根据顶点名称找到索引
            int v1_index = locate_vertex(v1);	
            int v2_index = locate_vertex(v2);	
            
            // 插入新结点
            ArcNode* p = new ArcNode;
            p->v_index = v2_index;
            p->arc = 1;
            p->next_arc = this->vexes[v1_index].first_arc;
            this->vexes[v1_index].first_arc = p;
            
            p = new ArcNode;
            p->v_index = v1_index;
            p->arc = 1;
            p->next_arc = this->vexes[v2_index].first_arc;
            this->vexes[v2_index].first_arc = p;
        }

        std::cout << "构造邻接表成功！" << std::endl;
        return OK;
    } 

    // 构建有向图邻接表
    Status createDG_ui() override{
        this->type = GraphCat::DG; 

        // 输入有向图总顶点数与边数
        std::cout << "请输入有向图总顶点数与边数" << std::endl;
        std::cin >> this->vertex_num >> this->arc_num;

        // 构造顶点表
        for(int i=0; i<this->vertex_num; i++){
            std::cout << "请输入第" << i+1 << "个顶点的名称" << std::endl;	
            std::cin >> this->vexes[i].vertex;
            this->vexes[i].first_arc = nullptr;
        }

        // 构造邻接表
        for(int i=0; i<this->arc_num; i++){
            // 读入关联边
            VerType v1, v2;
            std::cout << "请输入第" << i+1 << "条边关联的顶点" << std::endl;
            std::cin >> v1 >> v2;
            
            // 根据顶点名称找到索引
            int v1_index = locate_vertex(v1);	
            int v2_index = locate_vertex(v2);	
            
            // 插入新结点
            ArcNode* p = new ArcNode;
            p->v_index = v2_index;
            p->arc = 1;
            p->next_arc = this->vexes[v1_index].first_arc;
            this->vexes[v1_index].first_arc = p;
        }

        std::cout << "构造邻接表成功！" << std::endl;
        return OK;
    } 

    // 构建无向网邻接表
    Status createUDN_ui() override{
        this->type = GraphCat::UDN; 

        // 输入无向图总顶点数与边数
        std::cout << "请输入无向网总顶点数与边数" << std::endl;
        std::cin >> this->vertex_num >> this->arc_num;

        // 构造顶点表
        for(int i=0; i<this->vertex_num; i++){
            std::cout << "请输入第" << i+1 << "个顶点的名称" << std::endl;	
            std::cin >> this->vexes[i].vertex;
            this->vexes[i].first_arc = nullptr;
        }

        // 构造邻接表
        for(int i=0; i<this->arc_num; i++){
            // 读入关联边
            VerType v1, v2;
            ArcType arc;
            std::cout << "请输入第" << i+1 << "条边关联的顶点与权值：" << std::endl;
            std::cin >> v1 >> v2 >> arc;

            // 根据顶点名称找到索引
            int v1_index = locate_vertex(v1);	
            int v2_index = locate_vertex(v2);	
            
            // 插入新结点
            ArcNode* p = new ArcNode;
            p->v_index = v2_index;
            p->arc = arc;
            p->next_arc = this->vexes[v1_index].first_arc;
            this->vexes[v1_index].first_arc = p;
            
            p = new ArcNode;
            p->v_index = v1_index;
            p->arc = arc;
            p->next_arc = this->vexes[v2_index].first_arc;
            this->vexes[v2_index].first_arc = p;
        }

        std::cout << "构造无向网成功！" << std::endl;
        return OK;
    } 

    // 构建有向网邻接表
    Status createDN_ui() override{
        this->type = GraphCat::DN; 

        // 输入无向图总顶点数与边数
        std::cout << "请输入有向网总顶点数与边数" << std::endl;
        std::cin >> this->vertex_num >> this->arc_num;

        // 构造顶点表
        for(int i=0; i<this->vertex_num; i++){
            std::cout << "请输入第" << i+1 << "个顶点的名称" << std::endl;	
            std::cin >> this->vexes[i].vertex;
            this->vexes[i].first_arc = nullptr;
        }

        // 构造邻接表
        for(int i=0; i<this->arc_num; i++){
            // 读入关联边
            VerType v1, v2;
            ArcType arc;
            std::cout << "请输入第" << i+1 << "条边关联的顶点与权值：" << std::endl;
            std::cin >> v1 >> v2 >> arc;

            // 根据顶点名称找到索引
            int v1_index = locate_vertex(v1);	
            int v2_index = locate_vertex(v2);	
            
            // 插入新结点
            ArcNode* p = new ArcNode;
            p->v_index = v2_index;
            p->arc = arc;
            p->next_arc = this->vexes[v1_index].first_arc;
            this->vexes[v1_index].first_arc = p;
        }

        std::cout << "构造有向网成功！" << std::endl;
        return OK;
    } 

    // 打印邻接表
    void print() override{
        std::cout << "[" << gtype2str(this->type) << "]" << this->name << "的邻接表为：" << std::endl;
        for(int i=0; i<this->vertex_num; i++){
            std::cout << this->vexes[i].vertex << ":";
            ArcNode* p = this->vexes[i].first_arc;
            while(p != nullptr){
                std::cout << "\t->\t" << this->vexes[p->v_index].vertex;	
                p = p->next_arc;
            }
            std::cout << std::endl;
        }
    }

    // 根据顶点名称找到索引
    int locate_vertex(VerType v) override{
        for(int i=0; i<this->vertex_num; i++){
            if(this->vexes[i].vertex == v) return i;
        }
        return -1;
    }

    // 根据顶点索引，返回顶点名称
    VerType& operator[](int v_index) override{
        return vexes[v_index].vertex;
    }

    // 深度优先遍历(对外接口)
    void DFS(VerType v, std::function<void(VerType)> func){
        int v_index = locate_vertex(v);
        Vector<bool> visited(this->vertex_num, false);
        this->_dfs(v_index, func, visited);
    }

    // 深度优先遍历，递归算法
    void _dfs(int v_index, std::function<void(VerType)> func, Vector<bool> &visited){
        func((*this)[v_index]);
        visited[v_index] = true;
        for(ArcNode *adj_vptr=vexes[v_index].first_arc; adj_vptr!=nullptr; adj_vptr = adj_vptr->next_arc){
            if(!visited[adj_vptr->v_index]) this->_dfs(adj_vptr->v_index, func, visited);
        }
    }

    // 深度优先遍历(对外接口)，迭代器版
    void DFS2(VerType v, std::function<void(VerType)> func){
        Vector<bool> visited(this->vertex_num, false);  // 定义辅助数组，用于记录哪些顶点被访问过
        this->_dfs2(v, func, visited);
    }

    // 深度优先遍历(递归算法)，迭代器版
    void _dfs2(VerType v, std::function<void(VerType)> func, Vector<bool> &visited){
        int v_index = locate_vertex(v);
        if(visited[v_index]) return;

        visited[v_index] = true;
        func(v);
        for(auto t=vbegin(v); t!=vend(v); t++){
            // std::cout << "*t:" << *t << std::endl;
            this->_dfs2(*t, func, visited);
        } 
    }

    // 广度优先遍历
    void BFS(VerType v, std::function<void(VerType)> func){
        // 准备工作
        int v_index = locate_vertex(v);  // 将顶点名称转换为整数类型的索引，提高效率
        std::queue<VerType> worklist;  // 定义栈，用于辅助遍历
        Vector<bool> visited(this->vertex_num, false);  // 定义辅助数组，记录遍历过的顶点

        // 开始遍历
        func(v); 
        worklist.push(v);
        visited[v_index] = true;
        while(!worklist.empty()){
            v = worklist.back();
            worklist.pop();
            for(auto it=this->vbegin(v); it!=this->vend(v); it++){
                v_index = locate_vertex(*it);
                if(!visited[v_index]){
                    func(*it);
                    worklist.push(*it);
                    visited[v_index] = true;
                }
            }
        }
    }

    // 定义迭代器
    class Iterator{
    public:
        // 构造函数
        Iterator(ALGraph &G, int v_index, ArcNode *adj_vptr):
                G(G), v_index(v_index), adj_vptr(adj_vptr){}

        // 重载运算符
        // 前自增
        Iterator& operator++(){
            adj_vptr = adj_vptr->next_arc;
            return *this;
        }

        // 后自增
        Iterator operator++(int){
            Iterator temp = *this;
            adj_vptr = adj_vptr->next_arc;
            return temp;
        }
        
        // 引用
        VerType operator*(){
            return G[adj_vptr->v_index];
        }

        // 相等
        bool operator==(Iterator other){
            if( this->v_index == other.v_index &&
                this->adj_vptr == other.adj_vptr)
                return true;
            return false;
        }

        // 不相等
        bool operator!=(Iterator other){
            if( this->v_index == other.v_index &&
                this->adj_vptr == other.adj_vptr)
                return false;
            return true;
        }

    private:
        ALGraph &G;
        int v_index;
        ArcNode *adj_vptr;
    };
    friend class Iterator;

    // 根据顶点v返回迭代起点
    Iterator vbegin(VerType v){
        int v_index = locate_vertex(v);
        return Iterator(*this, v_index, vexes[v_index].first_arc);
    }

    // 根据顶点v返回迭代起点
    Iterator vend(VerType v){
        int v_index = locate_vertex(v);
        return Iterator(*this, v_index, nullptr);
    }

private:
    VNode vexes[MVnum];
};

// 十字链表图
template <typename VerType, typename ArcType>
class OLGraph: public Graph<VerType, ArcType>{
public:
    // 十字链表的弧结点
    struct ArcNode{
        int tail_vindex;  // 弧尾顶点
        int head_vindex;  // 弧头顶点
        ArcNode *hlink;  // 下一个同弧头的弧，即入度边
        ArcNode *tlink;  // 下一个同弧尾的弧，即出度边
        ArcType arc;  // 边的权值
    };

    // 十字链表的顶点结点
    struct VNode{
        VerType vertex;  // 记录顶点信息
        ArcNode *first_out;  // 第一个出度边
        ArcNode *first_in;  // 第一个入度边
    };

    // 十字链表构造函数
    OLGraph(std::string name="有向图十字链表"):Graph<VerType, ArcType>(name){}

    // 析构函数
    ~OLGraph(){
        for(int i=0; i<this->vertex_num; i++){
            for(ArcNode *p=vexes[i].first_out; p!=nullptr;){
                ArcNode *temp = p;
                p = p->tlink;
                // std::cout << "删除：" << (*this)[temp->tail_vindex] << " at " << temp << std::endl;
                delete temp;
            }
        }
    }

    // 使用十字链表创建图
    Status create(std::string file){
        YAML::Node graph = YAML::LoadFile(file);
        GraphCat type = str2gtype(graph["graph type"].as<std::string>());
        if(type == GraphCat::UDG or type == GraphCat::UDN)
            throw "错误的图类型";
        this->type = type;

        // 输入图的总顶点数与边数
        this->vertex_num = graph["vertex num"].as<int>();
        this->arc_num = graph["arc num"].as<int>();

        // 构造顶点表
        YAML::Node vertices = graph["vertices"];
        int v_index = 0;
        for(auto vertex:vertices){
            this->vexes[v_index].vertex = vertex.as<VerType>();
            this->vexes[v_index].first_in = nullptr;
            this->vexes[v_index].first_out = nullptr;
            v_index++;
        }

        // 构造十字链表
        YAML::Node arcs = graph["arcs"];
        for(auto arc_info: arcs){
            // 读取弧关联的两顶点与权值
            VerType v_tail = arc_info["vi"].as<VerType>();		
            VerType v_head = arc_info["vj"].as<VerType>();		
            ArcType arc = arc_info["w"].as<ArcType>();
            
            // 找到两个顶点的索引
            int vt_index = locate_vertex(v_tail);
            int vh_index = locate_vertex(v_head);

            // 创建顶点并插入
            ArcNode *p = new ArcNode;
            p->arc = arc;
            p->tail_vindex = vt_index;
            p->head_vindex = vh_index;

            p->tlink = this->vexes[vt_index].first_out;  // 插入顶点
            this->vexes[vt_index].first_out = p;
            p->hlink = this->vexes[vh_index].first_in;  // 插入顶点
            this->vexes[vh_index].first_in = p;
        }

        std::cout << "创建图成功！" << std::endl;
        return OK;
    }

    // 无法创建无向图
    Status createUDG_ui() override{
        throw "无法用十字链表创建无向图";
    }

    // 使用十字链表创建有向图
    Status createDG_ui() override{
        this->type = GraphCat::DG;

        // 读取有向图的总顶点数与边数
        std::cout << "请输入有向图的总顶点数与边数" << std::endl;
        std::cin >> this->vertex_num >> this->arc_num;

        // 构造顶点表
        for(int i=0; i<this->vertex_num; i++){
            std::cout << "请输入第" << i+1 << "个顶点的名称" << std::endl;
            std::cin >> this->vexes[i].vertex;
            this->vexes[i].first_in = nullptr;
            this->vexes[i].first_out = nullptr;
        }

        // 构造十字链表
        for(int i=0; i<this->arc_num; i++){
            // 读入弧关联的顶点
            std::cout << "请输入第" << i+1 << "条弧关联的顶点的名称" << std::endl;
            VerType v_tail, v_head;
            std::cin >> v_tail >> v_head; 
            
            // 找到两个顶点的索引
            int vt_index = locate_vertex(v_tail);
            int vh_index = locate_vertex(v_head);

            // 创建顶点并插入
            ArcNode *p = new ArcNode;
            p->arc = 1;
            p->tail_vindex = vt_index;
            p->head_vindex = vh_index;
            p->tlink = this->vexes[vt_index].first_out;  // 插入顶点
            this->vexes[vt_index].first_out = p;
            p->hlink = this->vexes[vh_index].first_in;  // 插入顶点
            this->vexes[vh_index].first_in = p;
        }

        std::cout << "创建有向图成功！" << std::endl;
        return OK;
    }

    // 使用十字链表创建无向网
    Status createUDN_ui() override{
        throw "无法用十字链表创建无向网";
    }

    // 使用十字链表创建有向网
    Status createDN_ui() override{
        this->type = GraphCat::DN;

        // 读取有向图的总顶点数与边数
        std::cout << "请输入有向图的总顶点数与边数" << std::endl;
        std::cin >> this->vertex_num >> this->arc_num;

        // 构造顶点表
        for(int i=0; i<this->vertex_num; i++){
            std::cout << "请输入第" << i+1 << "个顶点的名称" << std::endl;
            std::cin >> this->vexes[i].vertex;
            this->vexes[i].first_in = nullptr;
            this->vexes[i].first_out = nullptr;
        }

        // 构造十字链表
        for(int i=0; i<this->arc_num; i++){
            // 读入弧关联的顶点
            std::cout << "请输入第" << i+1 << "条弧关联的顶点的名称与弧的权值：" << std::endl;
            VerType v_tail, v_head;
            int arc;
            std::cin >> v_tail >> v_head >> arc; 
            
            // 找到两个顶点的索引
            int vt_index = locate_vertex(v_tail);
            int vh_index = locate_vertex(v_head);

            // 创建顶点并插入
            ArcNode *p = new ArcNode;
            p->arc = arc;
            p->tail_vindex = vt_index;
            p->head_vindex = vh_index;
            p->tlink = this->vexes[vt_index].first_out;  // 插入顶点
            this->vexes[vt_index].first_out = p;
            p->hlink = this->vexes[vh_index].first_in;  // 插入顶点
            this->vexes[vh_index].first_in = p;
        }

        std::cout << "创建有向网成功！" << std::endl;
        return OK;
    }

    // 打印有向图的十字链表
    void print() override{
        math::Matrix am_out(this->vertex_num);
        math::Matrix am_in(this->vertex_num);

        std::cout << "[" << gtype2str(this->type) << "]" << this->name << ":" << std::endl;
        for(int i=0; i<this->vertex_num; i++){
            std::cout << this->vexes[i].vertex << ":\n";

            std::cout << "    出边：";
            ArcNode *p = this->vexes[i].first_out;
            while(p !=nullptr){
                am_out(p->tail_vindex, p->head_vindex) = 1; 
                std::cout << this->vexes[p->head_vindex].vertex << ", ";
                p = p->tlink; 
            }
            std::cout << std::endl;
            
            std::cout << "    入边：";
            p = this->vexes[i].first_in;
            while(p !=nullptr){
                am_in(p->tail_vindex, p->head_vindex) = 1; 
                std::cout << this->vexes[p->tail_vindex].vertex << ", ";
                p = p->hlink; 
            }
            std::cout << std::endl;
        }
        std::cout << "出边转换的邻接矩阵：" << std::endl;
        am_out.print();
        std::cout << "进边转换的邻接矩阵：" << std::endl;
        am_in.print();
    }

    // 根据顶点名称找到索引
    int locate_vertex(VerType v) override{
        for(int i=0; i<this->vertex_num; i++){
            if(this->vexes[i].vertex == v) return i;
        }
        return -1;
    }

    // 根据顶点索引，返回顶点名称
    VerType& operator[](int v_index) override{
        return vexes[v_index].vertex;
    }

    // 深度优先遍历(对外接口)，迭代器版
    void DFS2(VerType v, std::function<void(VerType)> func){
        Vector<bool> visited(this->vertex_num, false);  // 定义辅助数组，用于记录哪些顶点被访问过
        this->_dfs2(v, func, visited);
    }

    // 深度优先遍历(递归算法)，迭代器版
    void _dfs2(VerType v, std::function<void(VerType)> func, Vector<bool> &visited){
        int v_index = locate_vertex(v);
        if(visited[v_index]) return;

        visited[v_index] = true;
        func(v);
        for(auto t=vbegin(v); t!=vend(v); t++){
            // std::cout << "*t:" << *t << std::endl;
            this->_dfs2(*t, func, visited);
        } 
    }

    // 广度优先遍历
    void BFS(VerType v, std::function<void(VerType)> func){
        // 准备工作
        int v_index = locate_vertex(v);  // 将顶点名称转换为整数类型的索引，提高效率
        std::queue<VerType> worklist;  // 定义栈，用于辅助遍历
        Vector<bool> visited(this->vertex_num, false);  // 定义辅助数组，记录遍历过的顶点

        // 开始遍历
        func(v); 
        worklist.push(v);
        visited[v_index] = true;
        while(!worklist.empty()){
            v = worklist.front();
            worklist.pop();
            for(auto it=this->vbegin(v); it!=this->vend(v); it++){
                v_index = locate_vertex(*it);
                if(!visited[v_index]){
                    func(*it);
                    worklist.push(*it);
                    visited[v_index] = true;
                }
            }
        }
    }
    
    // 迭代器
    class Iterator{
    public:
        // 构造函数
        Iterator(OLGraph &G, int v_index, std::string adj_type, ArcNode *adj_vptr):
        G(G), v_index(v_index), adj_type(adj_type), adj_vptr(adj_vptr){
            if(adj_type == "all"){
                if(G.vexes[v_index].first_out == nullptr) 
                    this->_adj_type= 2;
                else
                    this->_adj_type = 0;
            }
            else if(adj_type == "out")
                this->_adj_type = 1;
            else if(adj_type == "in")
                this->_adj_type = 2;
        }
    
        // 重载运算符
        // 前自增
        Iterator& operator++(){
            if(_adj_type == 0 ){
                if(adj_vptr->tlink == nullptr){
                    adj_vptr = G.vexes[v_index].first_in;
                    _adj_type = 2;
                }
                else{
                    adj_vptr = adj_vptr->tlink;
                }
            }
            else if(_adj_type == 1){
                adj_vptr = adj_vptr->tlink;
            }
            else if(_adj_type == 2){
                adj_vptr = adj_vptr->hlink;
            }
            return *this;
        }
    
        // 后自增
        Iterator operator++(int){
            Iterator tmp = *this;
            ++(*this);
            return tmp; 
        }
        
        // 解引用
        VerType& operator*(){
            if(_adj_type == 1 or _adj_type == 0)
                return G[adj_vptr->head_vindex];
            else if(_adj_type == 2)
                return G[adj_vptr->tail_vindex];
            else
                throw "要遍历的邻接点类型错误";
        }

        // 相等
        bool operator==(Iterator other){
            if(this->v_index == other.v_index &&
                this->adj_type == other.adj_type &&
                this->adj_vptr == other.adj_vptr)
                return true;
            return false;
        }

        // 不相等
        bool operator!=(Iterator other){
            if(this->v_index == other.v_index &&
                this->adj_type == other.adj_type &&
                this->adj_vptr == other.adj_vptr)
                return false;
            return true;
        }
    private:
        OLGraph<VerType, ArcType> &G;
        int v_index;
        std::string adj_type; 
        int _adj_type;  // 邻接点类型：0、任何；1、出度邻接点；2、入度邻接点
        ArcNode *adj_vptr;
    };
    friend class Iterator;

    // 根据顶点v返回迭代起点
    Iterator vbegin(VerType v, std::string adj_type="all"){
        int v_index = locate_vertex(v);
        ArcNode *adj_vptr;
        if(adj_type == "all"){
            if(vexes[v_index].first_out != nullptr)
                adj_vptr = vexes[v_index].first_out;
            else
                adj_vptr = vexes[v_index].first_in;
        }
        else if(adj_type == "out"){
            adj_vptr = vexes[v_index].first_out;
        }
        else if(adj_type == "in"){
            adj_vptr = vexes[v_index].first_in;
        }
        else{
            throw "要遍历的邻接点类型错误";
        }
        return Iterator(*this, v_index, adj_type, adj_vptr);
    }
        
    // 根据顶点v返回迭代终点
    Iterator vend(VerType v, std::string adj_type="all"){
        int v_index = locate_vertex(v);
        return Iterator(*this, v_index, adj_type, nullptr);
    }
private:
    VNode vexes[MVnum];
};

// 邻接多重表图
template <typename VerType, typename ArcType>
class AMLGraph: public Graph<VerType, ArcType>{
public:
    // 边结点
    struct ArcNode{
        int i_vindex, j_vindex;  // 保存一条边关联的两个顶点
        ArcNode *i_link, *j_link;  // i_vertex的下个邻接点
        ArcType arc;
    };

    // 顶点结点
    struct VNode{
        VerType vertex;
        ArcNode* first_edge;  // 指向第一条边
    };

    // 构造函数
    AMLGraph(std::string name="无向图-邻接多重表"):Graph<VerType, ArcType>(name){}

    // 析构函数
    ~AMLGraph(){
        for(ArcNode *arc:arc_list){
            // std::cout << "删除：" << (*this)[arc->i_vindex] << "--" << (*this)[arc->j_vindex] 
            //           << " at " << arc << std::endl;
            delete arc; 
        }
    }

    // 使用十字链表创建图
    Status create(std::string file){
        YAML::Node graph = YAML::LoadFile(file);
        GraphCat type = str2gtype(graph["graph type"].as<std::string>());
        if(type == GraphCat::DG or type == GraphCat::DN)
            throw "错误的图类型";
        this->type = type;

        // 输入图的总顶点数与边数
        this->vertex_num = graph["vertex num"].as<int>();
        this->arc_num = graph["arc num"].as<int>();

        // 构造顶点表
        YAML::Node vertices = graph["vertices"];
        int v_index = 0;
        for(auto vertex:vertices){
            this->vexes[v_index].vertex = vertex.as<VerType>();
            this->vexes[v_index].first_edge = nullptr;
            v_index++;
        }

        // 构造十字链表
        YAML::Node arcs = graph["arcs"];
        for(auto arc_info: arcs){
            // 读取弧关联的两顶点与权值
            VerType vi = arc_info["vi"].as<VerType>();		
            VerType vj = arc_info["vj"].as<VerType>();		
            ArcType arc = arc_info["w"].as<ArcType>();
            
            // 找到两个顶点的索引
            int i_vindex = locate_vertex(vi);
            int j_vindex = locate_vertex(vj);

            // 创建顶点并插入
            ArcNode *p = new ArcNode;
            arc_list.push_back(p);
            p->arc = arc;
            p->i_vindex = i_vindex;
            p->j_vindex = j_vindex;

            p->i_link = this->vexes[i_vindex].first_edge;  // 插入顶点
            this->vexes[i_vindex].first_edge= p;
            p->j_link = this->vexes[j_vindex].first_edge;  // 插入顶点
            this->vexes[j_vindex].first_edge = p;
        }

        std::cout << "创建图成功！" << std::endl;
        return OK;
    }

    // 使用邻接多重表构造无向图
    Status createUDG_ui() override{
        this->type = GraphCat::UDG;

        // 输入总顶点数与边数
        std::cout << "请输入无向图的总顶点数与边数：" << std::endl;
        std::cin >> this->vertex_num >> this->arc_num;

        // 构造顶点表
        for(int i=0; i<this->vertex_num; i++){
            std::cout << "请输入第" << i+1 << "个顶点的名称：" << std::endl;
            std::cin >> vexes[i].vertex;
            vexes[i].first_edge=nullptr;
        }

        // 构造邻接多重表
        for(int i=0; i<this->arc_num; i++){
            // 读入边关联的顶点
            std::cout << "请输入第" << i+1 << "条边关联的两顶点与边：" << std::endl;
            VerType v_i, v_j;
            std::cin >> v_i >> v_j;

            // 找到顶点的索引
            int i_vindex = locate_vertex(v_i);
            int j_vindex = locate_vertex(v_j);

            // 创建新的边结点并插入
            ArcNode *p = new ArcNode;
            arc_list.push_back(p);
            p->i_vindex = i_vindex;
            p->j_vindex = j_vindex;
            p->arc = 1;
            
            p->i_link = vexes[i_vindex].first_edge;
            vexes[i_vindex].first_edge = p;
            p->j_link = vexes[j_vindex].first_edge;
            vexes[j_vindex].first_edge = p;
        }
        std::cout << "创建无向图成功！" << std::endl;
        return OK;
    }
    
    // 使用邻接多重表构造有向图
    Status createDG_ui() override{
        throw "无法用邻接多重表构造有向图";
    }

    // 使用邻接多重表构造无向网
    Status createUDN_ui() override{
        this->type = GraphCat::UDN;

        // 输入总顶点数与边数
        for(int i=0; i<this->arc_num; i++){
            std::cout << "请输入第" << i+1 << "个顶点的名称：" << std::endl;
            std::cin >> vexes[i].vertex;
            vexes[i].first_edge=nullptr;
        }

        // 构造邻接多重表
        for(int i=0; i<this->arc_num; i++){
            // 读入边关联的顶点
            std::cout << "请输入第" << i+1 << "条边关联的两顶点与边的权值：" << std::endl;
            VerType v_i, v_j;
            ArcType arc;
            std::cin >> v_i >> v_j >> arc;

            // 找到顶点的索引
            int i_vindex = locate_vertex(v_i);
            int j_vindex = locate_vertex(v_j);

            // 创建新的边结点并插入
            ArcNode *p = new ArcNode;
            arc_list.push_back(p);
            p->i_vindex = i_vindex;
            p->j_vindex = j_vindex;
            p->arc = arc;
            
            p->i_link = vexes[i_vindex].first_edge;
            vexes[i_vindex].first_edge = p;
            p->j_link = vexes[j_vindex].first_edge;
            vexes[j_vindex].first_edge = p;
        }
        std::cout << "创建无向图成功！" << std::endl;
        return OK;
    }
    
    // 使用邻接多重表构造有向网
    Status createDN_ui() override{
        throw "无法用邻接多重表构造有向网";
    }

    // 根据顶点名称找到索引
    int locate_vertex(VerType v) override{
        for(int i=0; i<this->vertex_num; i++){
            if(vexes[i].vertex == v) return i;
        }
        return -1;
    }

    // 打印邻接多重表
    void print() override{
        math::Matrix m(this->vertex_num);  // 定义一个矩阵，用转换邻接表
        for(int i=0; i<this->vertex_num; i++){
            VNode i_vnode = vexes[i];
            std::cout << i_vnode.vertex << "的邻接点：" << std::endl;
            for(ArcNode *p=vexes[i].first_edge; p!=nullptr;){
                m(p->i_vindex, p->j_vindex) = 1;
                m(p->j_vindex, p->i_vindex) = 1;
                if(p->i_vindex == i){
                    std::cout << vexes[p->j_vindex].vertex << "\t";
                    p = p->i_link;
                }
                else{
                    std::cout << vexes[p->i_vindex].vertex << "\t";
                    p = p->j_link;
                }
            }
            std::cout << std::endl;
        }
        std::cout << "[" << gtype2str(this->type) << "]" << this->name << "的等效邻接矩阵为：" << std::endl;
        m.print();
    }

    // 重载索引
    VerType& operator[](int v_index){
        return vexes[v_index].vertex;
    }

    // 深度优先遍历(对外接口)，迭代器版
    void DFS2(VerType v, std::function<void(VerType)> func){
        Vector<bool> visited(this->vertex_num, false);  // 定义辅助数组，用于记录哪些顶点被访问过
        this->_dfs2(v, func, visited);
    }

    // 深度优先遍历(递归算法)，迭代器版
    void _dfs2(VerType v, std::function<void(VerType)> func, Vector<bool> &visited){
        int v_index = locate_vertex(v);
        if(visited[v_index]) return;

        visited[v_index] = true;
        func(v);
        for(auto t=vbegin(v); t!=vend(v); t++){
            // std::cout << "*t:" << *t << std::endl;
            this->_dfs2(*t, func, visited);
        } 
    }
    
    // 广度优先遍历
    void BFS(VerType v, std::function<void(VerType)> func){
        // 准备工作
        int v_index = locate_vertex(v);  // 将顶点名称转换为整数类型的索引，提高效率
        std::queue<VerType> worklist;  // 定义栈，用于辅助遍历
        Vector<bool> visited(this->vertex_num, false);  // 定义辅助数组，记录遍历过的顶点

        // 开始遍历
        func(v); 
        worklist.push(v);
        visited[v_index] = true;
        while(!worklist.empty()){
            v = worklist.front();
            worklist.pop();
            // std::cout << "访问" << v << "的邻接点：" << std::endl;
            for(auto it=this->vbegin(v); it!=this->vend(v); it++){
                // std::cout << "访问到" << *it << ", ";
                v_index = locate_vertex(*it);
                if(!visited[v_index]){
                    func(*it);
                    worklist.push(*it);
                    visited[v_index] = true;
                }
                // std::cout << std::endl;
            }
        }
    }

    // 迭代器
    class Iterator{
    public:
        // 构造函数
        Iterator(AMLGraph& G, int v_index, ArcNode *adj_vptr):G(G), v_index(v_index), adj_vptr(adj_vptr){}
    
        // 重载运算符
        // 前自增
        Iterator& operator++(){
            if(adj_vptr->i_vindex == this->v_index)
                adj_vptr = adj_vptr->i_link;
            else
                adj_vptr = adj_vptr->j_link;
            return *this;
        }
    
        // 后自增
        Iterator& operator++(int){
            Iterator &tmp = *this;
            if(adj_vptr->i_vindex == this->v_index)
                adj_vptr = adj_vptr->i_link;
            else
                adj_vptr = adj_vptr->j_link;
            return tmp; 
        }
        
        // 解引用
        VerType& operator*(){
            if(adj_vptr->i_vindex == this->v_index)
                return G[adj_vptr->j_vindex];
            else
                return G[adj_vptr->i_vindex];
        }

        // 相等
        bool operator==(const Iterator& other){
            if(this->v_index == other.v_index &&
                this->adj_vptr == other.adj_vptr)
                return true;
            return false;
        }

        // 不相等
        bool operator!=(const Iterator& other){
            if(this->v_index == other.v_index &&
                this->adj_vptr == other.adj_vptr)
                return false;
            return true;
        }

    private:
        AMLGraph& G;
        int v_index;
        ArcNode *adj_vptr;
    };
    friend class Iterator;

    // 返回顶点v迭代器起点
    Iterator vbegin(VerType v){
        int v_index = locate_vertex(v);
        return Iterator(*this, v_index, this->vexes[v_index].first_edge);
    }

    // 返回顶点v迭代器终点
    Iterator vend(VerType v){
        int v_index = locate_vertex(v);
        return Iterator(*this, v_index, nullptr);
    }
private:
    VNode vexes[MVnum];
    Vector<ArcNode*> arc_list;
};



