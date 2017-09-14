#pragma once


#include <boost/iterator/counting_iterator.hpp>


namespace nifty{
    namespace graph{

        ///\cond
        namespace detail_complete_graph{
            template<class NODE_TYPE>
            uint64_t n_complete_dgraph_edges(const NODE_TYPE && n_nodes, const bool self_loops){
                if(n_nodes == 0){
                    return 0;
                }
                else{
                    const auto from_self_loop = static_cast<NODE_TYPE>(self_loops) * n_nodes;
                    return ((n_nodes*(n_nodes-1))/2) + from_self_loop;
                }
            }
        }
        ///\endcond

        template<class NODE_INTEGER_TYPE, bool SELF_LOOPS = false>
        class CompleteGraph{
        public:




            typedef NODE_INTEGER_TYPE   NodeType;
            typedef uint64_t            EdgeType;

            typedef NodeType NodeConstRef;
            typedef EdgeType EdgeConstRef;



            boost::counting_iterator<NodeType> NodeIter;
            boost::counting_iterator<EdgeType> EdgeIter;



            CompleteGraph(const NodeType & n_nodes = 0)
            :   n_nodes_(n_nodes),
                n_edges_(detail_complete_graph::n_complete_dgraph_edges(n_nodes, SELF_LOOPS))
            {
            }


            NodeType n_edges()const{
                return n_edges_;
            }
            NodeType n_nodes()const{
                return n_nodes_;
            }

            NodeIter nodes_begin()const{
                return NodeIter(NodeType(0));
            }
            NodeIter nodes_end()const{
                return NodeIter(NodeType(n_nodes_));
            }
            EdgeIter edges_begin()const{
                return EdgeIter(EdgeType(0));
            }
            EdgeIter edges_end()const{
                return EdgeIter(EdgeType(n_edges_));
            }




        private:
            NodeType n_nodes_;
            EdgeType n_edges_;


        };


    }
}