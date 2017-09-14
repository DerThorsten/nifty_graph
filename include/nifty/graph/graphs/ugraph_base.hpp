#pragma once

// nifty graph
#include <nifty/graph/graphs/graph_base.hpp>

// nifty tools
#include <nifty/tools/range.hpp>

namespace nifty{
namespace graph{

    template<class DERIVED>
    class UGraphBase
    : public GraphBase<UGraphBase<DERIVED>>
    {
        public:

        auto nodes()const{
            typedef typename DERIVED::NodeIterator NodeIterator;
            auto nodes_begin = this->derived().nodes_begin();
            auto nodes_end   = this->derived().nodes_end();
            return nifty::tools::Range<NodeIterator>(nodes_begin, nodes_end);
        }

        auto edges()const{
            typedef typename DERIVED::EdgeIterator EdgeIterator;
            auto edges_begin = this->derived().edges_begin();
            auto edges_end   = this->derived().edges_end();
            return nifty::tools::Range<EdgeIterator>(edges_begin, edges_end);
        }

        template<class NODE>
        auto adjacency(const NODE & node)const{
            typedef typename DERIVED::AdjacencyIterator AdjacencyIterator;
            auto adjacency_begin = this->derived().adjacency_begin(node);
            auto adjacency_end   = this->derived().adjacency_end(node);
            return nifty::tools::Range<AdjacencyIterator>(adjacency_begin, adjacency_end);
        }

        // cast
        const DERIVED & derived()const{
            return static_cast<const DERIVED &>(*this);
        }
        DERIVED & derived(){
            return static_cast<DERIVED &>(*this);
        }
    };

}
}