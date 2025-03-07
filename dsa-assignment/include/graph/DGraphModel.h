/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/xMap.h"
// #include "stacknqueue/PriorityQueue.h"
#include "sorting/DLinkedListSE.h"

//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class DGraphModel : public AbstractGraph<T>
{
private:
public:
    DGraphModel(bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str) {}
    // Connect two vertices with a weight (Finished)
    void connect(T from, T to, float weight = 0)
    {
        typename AbstractGraph<T>::VertexNode *from_node = this->getVertexNode(from);
        if (from_node == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        typename AbstractGraph<T>::VertexNode *to_node = this->getVertexNode(to);
        if (to_node == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(to));
        }
        from_node->connect(to_node, weight);
    }
    // Disconnect two vertices (Finished)
    void disconnect(T from, T to)
    {
        typename AbstractGraph<T>::VertexNode *from_node = this->getVertexNode(from);
        if (from_node == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(from));
        }
        typename AbstractGraph<T>::VertexNode *to_node = this->getVertexNode(to);
        if (to_node == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(to));
        }
        typename AbstractGraph<T>::Edge *edge = from_node->getEdge(to_node);
        if (edge == nullptr)
        {
            stringstream os;
            os << "E(" << this->vertex2str(from) << "," << this->vertex2str(to) << ")";
            throw EdgeNotFoundException(os.str());
        }
        from_node->removeTo(to_node);
    }
    // Remove a vertex from the graph (Finished)
    void remove(T vertex)
    {
        typename AbstractGraph<T>::VertexNode *node = this->getVertexNode(vertex);
        if (node == nullptr)
        {
            throw VertexNotFoundException(this->vertex2str(vertex));
        }
        for (typename AbstractGraph<T>::VertexNode *current_node : this->nodeList)
        {
            current_node->removeTo(node);
            node->removeTo(current_node);
        }
        this->nodeList.removeItem(node);
    }
    // Create a graph from an array of vertices and an array of edges (Finished)
    static DGraphModel<T> *create(T *vertices, int nvertices, Edge<T> *edges, int nedges, bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
    {
        DGraphModel<T> *graph = new DGraphModel<T>(vertexEQ, vertex2str);
        for (int i = 0; i < nvertices; i++)
        {
            graph->add(vertices[i]);
        }
        for (int i = 0; i < nedges; i++)
        {
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);
        }
        return graph;
    }
};

#endif /* DGRAPHMODEL_H */
