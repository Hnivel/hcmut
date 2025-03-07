/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"
// #include "stacknqueue/PriorityQueue.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str) {}
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
        // Harmony
        if (from_node->equals(to_node))
        {
            from_node->connect(to_node, weight);
        }
        else
        {
            from_node->connect(to_node, weight);
            to_node->connect(from_node, weight);
        }
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
        if (from_node->equals(to_node))
        {
            typename AbstractGraph<T>::Edge *edge = from_node->getEdge(to_node);
            if (edge == nullptr)
            {
                string edge_string = "E(" + this->vertex2str(from) + "," + this->vertex2str(to) + ")";
                throw EdgeNotFoundException(edge_string);
            }
            from_node->removeTo(to_node);
        }
        else
        {
            typename AbstractGraph<T>::Edge *edge_1 = from_node->getEdge(to_node);
            if (edge_1 == nullptr)
            {
                string edge_string = "E(" + this->vertex2str(from) + "," + this->vertex2str(to) + ")";
                throw EdgeNotFoundException(edge_string);
            }
            typename AbstractGraph<T>::Edge *edge_2 = to_node->getEdge(from_node);
            if (edge_2 == nullptr)
            {
                string edge_string = "E(" + this->vertex2str(to) + "," + this->vertex2str(from) + ")";
                throw EdgeNotFoundException(edge_string);
            }
            from_node->removeTo(to_node);
            to_node->removeTo(from_node);
        }
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
    static UGraphModel<T> *create(T *vertices, int nvertices, Edge<T> *edges, int nedges, bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
    {
        UGraphModel<T> *graph = new UGraphModel<T>(vertexEQ, vertex2str);
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

#endif /* UGRAPHMODEL_H */
