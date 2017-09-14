#pragma once

#include "nifty/graph/graphs/building_blocks/stl_graph_map_adaptor.hpp"

namespace nifty {
namespace graph{

    template<class GRAPH, class MAP>
    class StlGraphMapAdaptor{
    public:
        const GRAPH GraphType;
        const MAP MapType;

        StlGraphMapAdaptor(const GraphType & graph, const MapType & map)
        :   graph_(graph),
            map_(map){
        }

    private:
        const GraphType & graph_;
        
    };


}
}