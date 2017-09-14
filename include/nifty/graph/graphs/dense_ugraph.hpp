#pragma once

#include <xtensor/xtensor.hpp>
#include <boost/iterator/counting_iterator.hpp>


namespace nifty{
    namespace graph{

     
        template<class NODE_INTEGER_TYPE, bool SELF_LOOPS = false>
        class DenseUGraph{
        public:




            typedef NODE_INTEGER_TYPE   NodeType;
            typedef std::pair<NodeType, NodeType> EdgeType;


            typedef NodeType NodeConstRef;
            typedef EdgeType EdgeConstRef;


            
            boost::counting_iterator<NodeType> NodeIter;
            //boost::counting_iterator<EdgeType> EdgeIter;



            DenseUGraph(const NodeType & n_nodes = 0)
            :   n_nodes_(n_nodes),
                n_edges_(0),
                edge_matrix_({n_nodes, n_nodes})
            {
                edge_matrix_ = false;
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


            std::pair<EdgeType, bool> add_edge(const NodeType & u, const NodeType & v){
                const auto edge = EdgeType(std::min(u,v), std::max(u,v));
                if(!edges_(u, v)){
                    edges_(u,v) = true;
                    edges_(v,u) = true;
                    return std::make_pair(edge, true);
                }
                else{
                    return std::make_pair(edge, false);
                }
            }

            std::pair<EdgeType, bool> find_edge(const NodeType & u, const NodeType & v){
                if(!edges_(u, v)){
                    return std::make_pair(EdgeType(), true);
                }
                else{
                    const auto edge = EdgeType(std::min(u,v), std::max(u,v));
                    return std::make_pair(edge, false);
                }
            }

            bool contains_node(const NodeType & node)const{
                return node < n_nodes_;
            }
            bool contains_edge(const EdgeType & edge)const{
                return edges_(edge.first, edge.second);
            }


        private:

            NodeType n_nodes_;
            EdgeType n_edges_;

            xtensor::xtensor<2, bool> edges_;


        };


    }
}