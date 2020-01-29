#ifndef DIJKSTRA_TRAINING_DIJKSTRA_HPP
#   define DIJKSTRA_TRAINING_DIJKSTRA_HPP

#include <deque>
#include <stack>
#include <iostream>
#include <float.h>
#include <algorithm>
#include "Dijkstra.hpp"
#include "Graph.hpp"

/*
 * If you create a graph composed of your custom class, e.g : a graph of Cities,
 * then this Class must implement de comparaison operator (==)
 */

namespace uti::graph {
    class AlgorithmFinished : public std::exception {};
    template <class T> // TODO : find a way to remove this template? see if I can move the Dijkstra class inside the Graph class.
    class Dijkstra {
    private:
        static constexpr long double INFINITY = LDBL_MAX;
        class VertexWrapper {
        public:

            typename Graph<T>::Vertex   &vertex;
            long double                 distance;
            VertexWrapper               *previous;
            bool                        already_visited;

            VertexWrapper(typename Graph<T>::Vertex &vertex)
                    : vertex { vertex },
                      distance { INFINITY },
                      previous { nullptr },
                      already_visited { false } {};
            VertexWrapper(const VertexWrapper &) = delete;
            VertexWrapper &operator=(const VertexWrapper &) = delete;
            VertexWrapper(VertexWrapper &&) noexcept = default;
            VertexWrapper &operator=(VertexWrapper &&) noexcept = default;
            bool operator==(const T &elem) const { return elem == vertex.element; };
            bool operator==(const typename Graph<T>::Vertex &vertex) const { return vertex == this->vertex; };
        };
        std::deque<VertexWrapper> _vertices;
    public:
        std::stack<T *> getShortestPath(Graph<T> &graph, const T &begin, const T &end)
        {
            for (auto &vertex : graph.getVertices())
                _vertices.push_back(VertexWrapper(vertex));
            VertexWrapper &vertex_begin = *std::find(_vertices.begin(), _vertices.end(), begin);
            vertex_begin.distance = 0;
            vertex_begin.already_visited = true;
            VertexWrapper *start = &vertex_begin;
            while (true) {
                _updateNeighborsDistance(*start);
                try {
                    VertexWrapper &next_vertex = _getCurrentShortestPath();
                    next_vertex.already_visited = true;
                    start = &next_vertex;
                }
                catch (AlgorithmFinished &e){
                    break;
                }
            }
            VertexWrapper &vertex_end = *std::find(_vertices.begin(), _vertices.end(), end);
            std::stack<T *> path;
            for (VertexWrapper *v = &vertex_end; v != nullptr; v = v->previous)
                path.emplace(&v->vertex.element);
            _vertices.clear();
            return path;
        }

    private:
        void _updateNeighborsDistance(VertexWrapper &vertex)
        {
            if (vertex.vertex.edges.empty())
                return;
            for (auto &edge : vertex.vertex.edges) {
                auto it = std::find(_vertices.begin(), _vertices.end(), edge.end);
                if (it->distance > edge.weight + vertex.distance) {
                    it->previous = &vertex;
                    it->distance = edge.weight + vertex.distance;
                }
            }
        }

    private:
        VertexWrapper &_getCurrentShortestPath() const
        {
            const VertexWrapper *shortestPath = nullptr;
            for (const VertexWrapper &vertex : _vertices) {
                if (!vertex.already_visited) {
                    shortestPath = &vertex;
                    break;
                }
            }
            for (const VertexWrapper &vertex : _vertices) {
                if (vertex.already_visited)
                    continue;
                if (vertex.distance < shortestPath->distance) {
                    shortestPath = &vertex;
                }
            }
            if (shortestPath == nullptr)
                throw AlgorithmFinished();
            return const_cast<VertexWrapper &>(*shortestPath);
        }
    };
}

#endif //DIJKSTRA_TRAINING_DIJKSTRA_HPP
