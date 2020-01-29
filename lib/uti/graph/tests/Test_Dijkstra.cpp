#include <criterion/criterion.h>
#include "Graph.hpp"
#include "Dijkstra.hpp"

Test(Dijkstra, small_int_graph)
{
    uti::graph::Graph<int> g;
    g.addVertex(1, 2, uti::graph::BIDIRECT, 100);
    g.addVertex(1, 3, uti::graph::BIDIRECT, 200);
    g.addVertex(1, 10, uti::graph::BIDIRECT, 300);
    g.addVertex(2, 10, uti::graph::BIDIRECT, 50);
    g.addVertex(1, 15, uti::graph::BIDIRECT, 700);
    g.addVertex(3, 15, uti::graph::BIDIRECT, 75);
    g.addVertex(2, 15, uti::graph::BIDIRECT, 5000);
    g.addVertex(1, 4, uti::graph::BIDIRECT, 80);
    g.addVertex(4, 10, uti::graph::BIDIRECT, 350);
    g.addVertex(7, 15, uti::graph::BIDIRECT, 20);
    g.addVertex(2, 7, uti::graph::BIDIRECT, 10);
    g.addVertex(10, 11, uti::graph::BIDIRECT, 60);
    g.addVertex(11, 15, uti::graph::BIDIRECT, 90);

    std::stack<int *> container = g.dijkstraSolving(1, 15);
    std::array<int, 4> expectedArray = {1, 2, 7, 15};
    int i = 0;
    while (!container.empty()) {
        cr_assert(*container.top() == expectedArray.at(i));
        ++i;
        container.pop();
    }
}

class City {
public:
    City(std::string name) : name { std::move(name) } {};
    City(const City &) = delete;
    std::string name;
    bool operator==(const City &other) const { return other.name == name; };
    City &operator=(const City &other) = delete;
    City(City &&) noexcept = default;
    City &operator=(City &&) noexcept = default;
};

Test(Dijkstra, small_city_graph)
{
    uti::graph::Graph<City> g;
    City paris("Paris");
    City tokyo("Tokyo");
    City newYork("NewYork");
    City losangeles("LosAngeles");
    City sydney("sydney");
    City london("london");
    City moscou("Moscou");

    g.addVertex(paris, tokyo, uti::graph::A_TO_B, 10000);
    g.addVertex(paris, moscou, uti::graph::A_TO_B, 5000);
    g.addVertex(paris, newYork, uti::graph::A_TO_B, 5000);
    g.addVertex(newYork, losangeles, uti::graph::A_TO_B, 3000);
    g.addVertex(tokyo, sydney, uti::graph::A_TO_B, 1000);
    g.addVertex(losangeles, sydney, uti::graph::A_TO_B, 250);
    g.addVertex(paris, london, uti::graph::BIDIRECT, 100);

    std::stack<City *> container = g.dijkstraSolving(paris, sydney);
    std::array<City *, 4> expectedArray = {&paris, &newYork, &losangeles, &sydney};
    int i = 0;
    while (!container.empty()) {
        cr_assert(*container.top() == *expectedArray.at(i));
        ++i;
        container.pop();
    }
}
