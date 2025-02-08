#include"graph.h"

// 将图枚举类型转换为字符串
std::string gtype2str(GraphCat g_type){
    switch (g_type){
        case UDG: return "undigraph";
        case DG: return "digraph";
        case UDN: return "undigraph net";
        case DN: return "digraph net";
        default: return "invail graph type";
    }
}

// 将字符串转换为图枚举类型
GraphCat str2gtype(std::string g_type){
    if(g_type == "undigraph")
        return GraphCat::UDG;
    else if(g_type == "digraph")
        return GraphCat::DG;
    else if(g_type == "undigraph net")
        return GraphCat::UDN;
    else if(g_type == "digraph net")
        return GraphCat::DN;
    else
        throw "错误的图类型";
}
