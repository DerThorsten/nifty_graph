#include "doctest.h"

// test related
#include "graph_test.hpp"

// nifty-graph
#include "nifty/graph/graphs/dynamic_ugraph.hpp"

TEST_SUITE_BEGIN("dynamic_undirected_graph");


template<class NODE>
struct NodeHelper{
    static NODE num_to_node(uint8_t val){
        return NODE(val);
    }
};

template<>
struct NodeHelper<std::string>{
    static std::string num_to_node(uint8_t val){
        return std::to_string(val);
    }
};


TEST_CASE_TEMPLATE_DEFINE("test_dynamic_graph_grid_2x2", GraphType, test_dynamic_graph) {
    
    typedef typename GraphType::NodeType NodeType;
    typedef NodeHelper<NodeType> NodeHelperType;

    // construct graph
    // 0 - 1
    // 2 - 3
    GraphType g;

    REQUIRE_EQ(g.n_nodes(), 0);
    REQUIRE_EQ(g.n_edges(), 0);

    // simple 2x2 grid graph

    g.add_node(NodeHelperType::num_to_node(0));
    REQUIRE_EQ(g.n_nodes(), 1);
    REQUIRE_EQ(g.n_edges(), 0);

    g.add_node(NodeHelperType::num_to_node(1));
    REQUIRE_EQ(g.n_nodes(), 2);
    REQUIRE_EQ(g.n_edges(), 0);

    g.add_edge(NodeHelperType::num_to_node(2),NodeHelperType::num_to_node(3));
    REQUIRE_EQ(g.n_nodes(), 4);
    REQUIRE_EQ(g.n_edges(), 1);


    g.add_edge(NodeHelperType::num_to_node(0),NodeHelperType::num_to_node(1));
    REQUIRE_EQ(g.n_nodes(), 4);
    REQUIRE_EQ(g.n_edges(), 2);

    g.add_edge(NodeHelperType::num_to_node(0),NodeHelperType::num_to_node(2));
    REQUIRE_EQ(g.n_nodes(), 4);
    REQUIRE_EQ(g.n_edges(), 3);

    g.add_edge(NodeHelperType::num_to_node(1),NodeHelperType::num_to_node(3));
    REQUIRE_EQ(g.n_nodes(), 4);
    REQUIRE_EQ(g.n_edges(), 4);





    SUBCASE( "nodes" ) {

        auto counter = 0;
        for(auto node : g.nodes()){
            ++counter;
        }
        REQUIRE_EQ(counter, g.n_nodes());
    }

    SUBCASE( "adjacency" ) {


        for(auto u : g.nodes()){
            for(auto adj : g.adjacency(u)){
                auto v = adj.node();
            }
        }
    }

}

typedef doctest::Types<
    nifty::graph::DynamicUGraph<int>,
    nifty::graph::DynamicUGraph<std::string>
> dynamic_graph_types;
TEST_CASE_TEMPLATE_INSTANTIATE(test_dynamic_graph, dynamic_graph_types);






TEST_CASE( "add_node" ) {
    nifty::graph::DynamicUGraph<int> g;

    REQUIRE(g.add_node(0));
    REQUIRE_FALSE(g.add_node(0));
    REQUIRE_EQ(g.n_nodes(), 1);

    REQUIRE(g.add_node(1));
    REQUIRE_FALSE(g.add_node(0));
    REQUIRE_FALSE(g.add_node(1));
    REQUIRE_EQ(g.n_nodes(), 2);

    REQUIRE(g.add_node(2));
    REQUIRE_FALSE(g.add_node(0));
    REQUIRE_FALSE(g.add_node(1));
    REQUIRE_FALSE(g.add_node(2));
    REQUIRE_EQ(g.n_nodes(), 3);
}

TEST_CASE( "add_edge" ) {
    nifty::graph::DynamicUGraph<int> g;
    typedef typename nifty::graph::DynamicUGraph<int>::EdgeType EdgeType;


    EdgeType edge;
    bool added_edge;

    // both nodes are NOT in graph
    std::tie(edge, added_edge) = g.add_edge(0,1);
    REQUIRE(added_edge);
    REQUIRE_EQ(edge, 0);
    REQUIRE_EQ(g.n_edges(), 1);
    REQUIRE_EQ(g.n_nodes(), 2);

    // u  in graph v not
    std::tie(edge, added_edge) = g.add_edge(0,2);
    REQUIRE(added_edge);
    REQUIRE_EQ(edge, 1);
    REQUIRE_EQ(g.n_edges(), 2);
    REQUIRE_EQ(g.n_nodes(), 3);

    // u and v in graph
    g.add_node(3);
    std::tie(edge, added_edge) = g.add_edge(0,3);
    REQUIRE(added_edge);
    REQUIRE_EQ(edge, 2);
    REQUIRE_EQ(g.n_edges(), 3);
    REQUIRE_EQ(g.n_nodes(), 4);


    // existing edge
    std::tie(edge, added_edge) = g.add_edge(0,1);
    REQUIRE_FALSE(added_edge);
    REQUIRE_EQ(edge, 0);
    REQUIRE_EQ(g.n_edges(), 3);
    REQUIRE_EQ(g.n_nodes(), 4);

    // existing edge
    std::tie(edge, added_edge) = g.add_edge(3,0);
    REQUIRE_FALSE(added_edge);
    REQUIRE_EQ(edge, 2);
    REQUIRE_EQ(g.n_edges(), 3);
    REQUIRE_EQ(g.n_nodes(), 4);
}


TEST_CASE( "add_edge" ) {
    nifty::graph::DynamicUGraph<int> g;
    typedef typename nifty::graph::DynamicUGraph<int>::EdgeType EdgeType;

}

TEST_SUITE_END();