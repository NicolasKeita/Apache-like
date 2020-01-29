#ifndef DIJKSTRA_TRAINING_GRAPH_HPP
#   define DIJKSTRA_TRAINING_GRAPH_HPP

#include <algorithm>
#include <deque>
#include <stack>

namespace uti::graph {
    template<typename T>
    class Graph;
}
#include "Dijkstra.hpp"

/*
 * My graph implementation is a container of vertices.
 * Each vertex also owns a container of edges and each vertex also owns one element.
 * I only used std::deque as container
 * The elements you add to the graph MUST live outside the class. I only retrive references.
 * API public functions :
 *          addVertex(vertex_ID) to create the graph or a vertex.
 *                  "vertex_ID" is usually a unique number or string, to distinguish between each vertex
 *          addVertex(vertex_1_ID, vertex_2_ID, BIDIRECT) to link two vertices
 *          getVertex(vertex_ID)
 *          dijkstraSolving(const T &begin, const T&end)
 *                  Returns each ID in order, from begin to end using the fastest way possible
 */
namespace uti::graph {
    class CannotFindVertex : public std::exception {};
    enum LinkType {
        A_TO_B = 0,
        B_TO_A,
        BIDIRECT
    };

    template<typename T>
    class Graph {
    public:
        class Vertex;
        class Edge;
    public:
        std::deque<Vertex> _vertices;
    public:
        std::stack<T *>         dijkstraSolving(const T &begin, const T&end)
        {
            getVertex(begin);
            getVertex(end);
            return uti::graph::Dijkstra<T>().getShortestPath(*this, begin, end);
        }
        Vertex &                addVertex(const T &element) noexcept
        {
            auto it = std::find(_vertices.begin(), _vertices.end(), element);
            if (it != _vertices.end()) {
                return *it;
            }
            _vertices.push_back(Vertex(element));
            return *_vertices.end();
        };
        void                    addVertex(const T &first_elem, const T &second_elem,
                                          LinkType link = A_TO_B, long double distance = 1) noexcept
        {
            addVertex(first_elem);
            addVertex(second_elem);
            Vertex &vertex1 = getVertex(first_elem);
            Vertex &vertex2 = getVertex(second_elem);
            switch (link) {
                case A_TO_B : {
                    vertex1.link(vertex2, distance);
                    break;
                }
                case B_TO_A : {
                    vertex2.link(vertex1, distance);
                    break;
                }
                case BIDIRECT : {
                    vertex1.link(vertex2, distance);
                    vertex2.link(vertex1, distance);
                    break;
                }
                default:
                    break;
            }
        }
        Vertex &                    getVertex(const T &elem) {
            auto it = std::find(_vertices.begin(), _vertices.end(), elem);
            if (it == _vertices.end())
                throw CannotFindVertex();
            return *it;
        }
        std::deque<Vertex> &        getVertices() noexcept { return _vertices; };

    public:
        class Vertex {
        public:
            explicit Vertex(const T &element) : element{ const_cast<T &>(element) } {}
            explicit Vertex(T &element) : element{ element } {}

            T &element;
            std::deque<Edge> edges;

            Vertex(const Vertex &) = delete;
            Vertex &operator=(const Vertex &) = delete;
            Vertex(Vertex &&) = default;
            Vertex &operator=(Vertex &&) noexcept = default;

            std::deque<T *> getNeighbour() const
            {
                std::deque<T *> list;
                for (const Edge &edge : edges) {
                    list.push_back(&edge.end.element);
                }
                return list;
            }

            bool operator==(const T &elem) const { return elem == this->element; };
            bool operator==(T &elem) const { return elem == element; };
            bool operator==(const Vertex &elem) const { return elem.element == element; };

            void link(Vertex &other, long double distance) { edges.push_back(Edge(*this, other, distance)); }; // TODO : check if already linked
        };
    public:
        class Edge {
        public:
            Edge(Vertex &begin, Vertex &end, long double distance)
                : weight{distance},
                  begin{begin},
                  end{end} {}

            Edge(const Edge &) = delete;
            Edge &operator=(const Edge &) = delete;
            Edge(Edge &&) noexcept = default;
            Edge &operator=(Edge &&) noexcept = default;

            long double weight;
            Vertex &begin;
            Vertex &end;

            bool operator==(const Edge &other) { return other.weight == weight; };
        };
    };
}

#endif //DIJKSTRA_TRAINING_GRAPH_HPP
