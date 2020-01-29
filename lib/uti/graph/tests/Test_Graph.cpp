#include <criterion/criterion.h>
#include <string>
#include "Graph.hpp"

Test(Graph, comparison_operator)
{
    uti::graph::Graph<std::string> g;
    g.addVertex("Paris");
    g.addVertex("Tokyo");
    cr_assert(g.getVertex("Paris") == "Paris");
    cr_assert(g.getVertex("Tokyo") == "Tokyo");
}

Test(Graph, getVertex_method)
{
    uti::graph::Graph<std::string> g;
    g.addVertex("Paris");
    cr_assert_throw(g.getVertex("Tokyo"), uti::graph::CannotFindVertex);
    cr_assert_no_throw(g.getVertex("Paris"), uti::graph::CannotFindVertex);
}

Test(Graph, test_linkage_two_vertices_string)
{
    uti::graph::Graph<std::string> g;
    g.addVertex("Paris");
    g.addVertex("Marseille");
    g.addVertex("Paris", "Marseille", uti::graph::BIDIRECT, 4531109.4542);
    cr_assert(g.getVertex("Paris").edges.size() == 1);
    cr_assert(g.getVertex("Paris").edges.at(0).begin == "Paris");
    cr_assert(g.getVertex("Paris").edges.at(0).end == "Marseille");
    cr_assert(g.getVertex("Paris").edges.at(0).weight == 4531109.4542);
    cr_assert(g.getVertex("Marseille").edges.size() == 1);
    cr_assert(g.getVertex("Marseille").edges.at(0).begin == "Marseille");
    cr_assert(g.getVertex("Marseille").edges.at(0).end == "Paris");
    cr_assert(g.getVertex("Marseille").edges.at(0).weight == 4531109.4542);
}

Test(Graph, test_linkage_two_vertices_int)
{
    uti::graph::Graph<int> g2;
    g2.addVertex(1, 2, uti::graph::BIDIRECT);
    g2.addVertex(1, 3, uti::graph::BIDIRECT);
    g2.addVertex(1, 10, uti::graph::BIDIRECT);
    cr_assert(g2.getVertex(1).edges.size() == 3);
    cr_assert(g2.getVertex(1).edges.at(0).begin == 1);
    cr_assert(g2.getVertex(1).edges.at(0).end == 2);
    cr_assert(g2.getVertex(1).edges.at(1).begin == 1);
    cr_assert(g2.getVertex(1).edges.at(1).end == 3);
    cr_assert(g2.getVertex(1).edges.at(2).begin == 1);
    cr_assert(g2.getVertex(1).edges.at(2).end == 10);
}

class City {
public:
    City(const std::string &name) : name { name } {};
    City(const City &other) = delete;
    std::string name;
    bool operator==(const City &other) const { return other.name == name; };
    City &operator=(const City &other) = delete;
};

Test(Graph, objects_in_graph)
{
    uti::graph::Graph<City> g;
    City paris("Paris");
    City tokyo("Tokyo");
    g.addVertex(paris, tokyo, uti::graph::A_TO_B, 42421);
    cr_assert(g.getVertex(paris).edges.size() == 1);
    cr_assert(g.getVertex(paris).element.name == "Paris");
    g.getVertex(paris).element.name = "Berlin";
    cr_assert(g.getVertex(paris).element.name == "Berlin");
    cr_assert(paris.name == "Berlin");
    paris.name = "Paris";
    cr_assert(g.getVertex(paris).element.name == "Paris");
}
