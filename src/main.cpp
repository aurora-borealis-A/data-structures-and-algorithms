#include<iostream>
#include"common.h"
#include"graph.h"
#include"vector.h"
#include<yaml-cpp/yaml.h>

#define CallFunc print_vertex
#define GraphFile "../etc/graph/undigraph2.yaml"

void CallFunc(VType v){
    std::cout << "遍历了：" << v << std::endl;
}

// 测试图
template <typename GraType>
void test_graph(){
    GraType G("世界地图");
	G.create(GraphFile);
    G.print();
    
    // 测试迭代器
    std::cout << "测试迭代器：" << std::endl;
    for(int i=0; i<G.vertex_num; i++){
        VType v = G[i];
        std::cout << v << "邻接点为" << std::endl;
        for(auto t=G.vbegin(v); t!=G.vend(v); t++){
            std::cout << *t << ", ";
        }
        std::cout << std::endl;
    }

    // 测试度优先遍历
//     std::cout << "深度优先遍历DFS：" << std::endl;
//     G.DFS("v1", CallFunc);
    std::cout << "深度优先遍历DFS2：" << std::endl;
    G.DFS2("v1", CallFunc);

    // 测试广度优先遍历
    std::cout << "广度优先遍历BFS：" << std::endl;
    G.BFS("v1", CallFunc);
}

// 测试vector
void test_vector(){
    Vector<int> v(3);
    for(int i=0; i<17; i++){
        v.push_back(i);
        std::cout << v[i] << std::endl;
        std::cout << "capasity:" << v.capasity() << std::endl;
    }
//     std::cout << v[0] << std::endl;
//     std::cout << v[1] << std::endl;
}

int main(){
	test_graph<AMLGraph<VType, AType>>();
    // test_vector();
	return 0;
}
