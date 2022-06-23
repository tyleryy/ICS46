// Digraph.hpp
//
// ICS 46 Spring 2022
// Project #5: Rock and Roll Stops the Traffic
//
// This header file declares a class template called Digraph, which is
// intended to implement a generic directed graph.  The implementation
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//
// In general, directed graphs are all the same, except in the senses
// that they store different kinds of information about each vertex and
// about each edge; these two types are the type parameters to the
// Digraph class template.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <exception>
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <limits>
#include <queue>





// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}

// class VertexInfo
// {
// public:
//     VertexInfo(std::string name){m_name = name;}
//     std::string get_name(){return m_name;}
//     void set_name(std::string new_str){m_name = new_str;}
// private:
//     std::string m_name;
// };

// class EdgeInfo
// {

// public:
//     EdgeInfo(int distance, double speed_input)
//     {
//         dist = distance;
//         speed = speed_input;
//     }
//     int get_dist(){return dist;}
//     double get_speed(){return speed;}
//     void set_speed(double new_speed){speed = new_speed;}
//     void set_dist(int new_dist){dist = new_dist;}
//     int get_time(){return dist/speed;}

// private:
//     int dist;
//     double speed;
// };




// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a struct template.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a struct template.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
    bool visited;

    bool shortest_path_found;
    int previous_vertex;
    double shortest_path;
};



// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
// You'll need to implement the member functions declared here; each has a
// comment detailing how it is intended to work.
//
// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one (i.e., any change to the copy will not affect the
    // original).
    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d) noexcept;

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph() noexcept;

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d) noexcept;

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const noexcept;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const noexcept;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the predecessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;


private:
    // Add whatever member variables you think you need here.  One
    // possibility is a std::map where the keys are vertex numbers
    // and the values are DigraphVertex<VertexInfo, EdgeInfo> objects.


    // You can also feel free to add any additional member functions
    // you'd like (public or private), so long as you don't remove or
    // change the signatures of the ones that already exist.

    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> adj_list;
    
    void check_vertex_existence(int vertex) const;
    int count_reachable_vertices(std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>& copy_adj_list, int vertex) const;
    void set_up_copy_map(std::map<int, DigraphVertex<VertexInfo,EdgeInfo>>& copy_map, int startVertex) const;


};



template<typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::set_up_copy_map(std::map<int, DigraphVertex<VertexInfo,EdgeInfo>>& copy_map, int startVertex) const
{
    
    for(auto& [key, value]: copy_map)
    {
        if(key == startVertex)
        {
            //value.previous_vertex = nullptr;
            value.shortest_path = 0;
            value.previous_vertex = std::numeric_limits<int>::min(); //previous vertex of start vertex is None, represented by -infinity
        }
        else
        {
            value.shortest_path = std::numeric_limits<double>::max(); //shortest path intially set to infinity
            value.previous_vertex = -1; //prev vertex not found yet
        }
        value.shortest_path_found = false;
    }
}




template<typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::check_vertex_existence(int vertex) const
{
    bool found = false;
    for(int v: vertices())
    {
        if(v == vertex)
        {
            found = true;
            break;
        }
    }
    if(not found)
    {
        throw DigraphException("toVertex not found");
    }
}

template<typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::count_reachable_vertices(std::map<int, DigraphVertex<VertexInfo, EdgeInfo>>& copy_adj_list, int vertex) const
{
    copy_adj_list.at(vertex).visited = true;
    int counter = 0;
    for(auto edges: copy_adj_list.at(vertex).edges)
    {
        if(copy_adj_list.at(edges.toVertex).visited == false)
            counter += count_reachable_vertices(copy_adj_list, edges.toVertex);
    }
    return 1 + counter;

}



// You'll need to implement the member functions below.  There's enough
// code in place to make them compile, but they'll all need to do the
// correct thing instead.

template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph()
    : adj_list{}
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
{
    adj_list = d.adj_list;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(Digraph&& d) noexcept
{
    std::swap(adj_list, d.adj_list);
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph() noexcept
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
    adj_list = d.adj_list;
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(Digraph&& d) noexcept
{
    std::swap(adj_list, d.adj_list);
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{
    std::vector<int> numbers;
    for(const auto& [key, value]: adj_list)
    {
        numbers.push_back(key);
    }
    return numbers;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
    std::vector<std::pair<int, int>> edges_vec;
    for(const auto& [key, value]: adj_list)
    {
        for(DigraphEdge<EdgeInfo> e: value.edges)
        {
            std::pair<int, int> edge_nums = std::make_pair(e.fromVertex, e.toVertex);
            edges_vec.push_back(edge_nums);
        }
    }
    return edges_vec;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
    check_vertex_existence(vertex);
    std::vector<std::pair<int, int>> edges_vec;
    for(DigraphEdge<EdgeInfo> e: adj_list.at(vertex).edges)
    {
        std::pair<int, int> edge_nums = std::make_pair(e.fromVertex, e.toVertex);
        edges_vec.push_back(edge_nums);
    }

    return edges_vec;
}


template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
    check_vertex_existence(vertex);
    return adj_list.at(vertex).vinfo;
}

template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
    check_vertex_existence(fromVertex);
    check_vertex_existence(toVertex);
 
    for(DigraphEdge<EdgeInfo> v: adj_list.at(fromVertex).edges)
    {
        if((v.fromVertex == fromVertex) && (v.toVertex == toVertex))
        {
            return v.einfo;
        }
    }
    throw DigraphException("Edge not found");
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
    for(const int& d: vertices())
    {
        if(d == vertex)
        {
            throw DigraphException("Vertex number already in Digraph");
        }
    }
    DigraphVertex<VertexInfo, EdgeInfo> dv;
    dv.vinfo = vinfo;
    adj_list[vertex] = dv;
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
{
    check_vertex_existence(fromVertex);
    check_vertex_existence(toVertex);
    for(DigraphEdge<EdgeInfo> v: adj_list.at(fromVertex).edges)
    {
        if((v.fromVertex == fromVertex) && (v.toVertex == toVertex))
        {
            throw DigraphException("Edge exists already");
        }
    }   
    DigraphEdge<EdgeInfo> new_edge{fromVertex, toVertex, einfo};
    adj_list.at(fromVertex).edges.push_back(new_edge);
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
    check_vertex_existence(vertex);
    adj_list.erase(vertex);
    std::vector<typename std::list<DigraphEdge<EdgeInfo>>::iterator> erase_vec;
    for(auto& [key, value]: adj_list)
    {
        erase_vec.clear();
        std::list<DigraphEdge<EdgeInfo>>& edge_list = value.edges;
        for(typename std::list<DigraphEdge<EdgeInfo>>::iterator list_ptr = edge_list.begin(); list_ptr != edge_list.end(); list_ptr++)
        {
            if(list_ptr->toVertex == vertex)
            {
                typename std::list<DigraphEdge<EdgeInfo>>::iterator copy_ptr = list_ptr;
                erase_vec.push_back(copy_ptr);
            }
        }
        for(const auto& v: erase_vec)
        {
            edge_list.erase(v);
        }
    }

}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
    check_vertex_existence(fromVertex);
    check_vertex_existence(toVertex);
    std::list<DigraphEdge<EdgeInfo>>& edge_list = adj_list.at(fromVertex).edges;
    bool found = false;
    typename std::list<DigraphEdge<EdgeInfo>>::iterator saved_ptr;
    for(typename std::list<DigraphEdge<EdgeInfo>>::iterator list_ptr = edge_list.begin(); list_ptr != edge_list.end(); list_ptr++)
    {
        if((list_ptr->fromVertex == fromVertex) && (list_ptr->toVertex == toVertex))
        {
            found = true;
            saved_ptr = list_ptr;
            break;
        }
    }
    if (found)
        edge_list.erase(saved_ptr);
    else
        throw DigraphException("Edge not found");
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const noexcept
{
    return adj_list.size();
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const noexcept
{
    return edges().size();
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
    return edges(vertex).size();
}


template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> copy = adj_list;
    int counter = 0;
    for(auto [key, value]: copy)
    {
        counter++;
        for(auto& [k, v]: copy)
        {
            v.visited = false;
        }
        if(count_reachable_vertices(copy, key) != vertexCount())
        {
            return false;
        }
    }
    return true;
}


template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
    int startVertex,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
    std::map<int, int> results{};
    auto cmp_func = [](std::pair<int, DigraphVertex<VertexInfo, EdgeInfo>> v, std::pair<int, DigraphVertex<VertexInfo, EdgeInfo>> w) 
    {return (v.second.shortest_path) > (w.second.shortest_path); };
    std::priority_queue<std::pair<int, DigraphVertex<VertexInfo, EdgeInfo>>, std::vector<std::pair<int, DigraphVertex<VertexInfo, EdgeInfo>>>, decltype(cmp_func)> pq(cmp_func);
    std::map<int, DigraphVertex<VertexInfo,EdgeInfo>> copy_map = adj_list;
    set_up_copy_map(copy_map, startVertex);
    pq.push(std::make_pair(startVertex, copy_map.at(startVertex)));

    
    while(not pq.empty())
    {
        std::pair<int, DigraphVertex<VertexInfo, EdgeInfo>> curr_pair = pq.top();
        DigraphVertex<VertexInfo, EdgeInfo> curr_v = curr_pair.second;
        int key = curr_pair.first;
        pq.pop();
    
        if(curr_v.shortest_path_found == false)
        {
            copy_map[key].shortest_path_found = true;
            for(DigraphEdge<EdgeInfo> e : curr_v.edges)
            {
                if(copy_map.at(e.toVertex).shortest_path > curr_v.shortest_path + edgeWeightFunc(e.einfo))
                {                 
                    copy_map[e.toVertex].shortest_path = curr_v.shortest_path + edgeWeightFunc(e.einfo);
                    copy_map[e.toVertex].previous_vertex = key;
                    pq.push(std::make_pair(e.toVertex, copy_map[e.toVertex]));
                }
            }
        }
    }
    for(const auto& [key, value]: copy_map)
    {
        if((value.previous_vertex == std::numeric_limits<int>::min()) || (value.previous_vertex == -1))
            results[key] = key;
        else
            results[key] = value.previous_vertex;
    }
    return results;
}



#endif

