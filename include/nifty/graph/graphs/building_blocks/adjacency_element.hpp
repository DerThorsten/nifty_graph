#pragma once


#include <boost/iterator/counting_iterator.hpp>


namespace nifty{
namespace graph{

    template<class NODE, class EDGE>
    class AdjacencyElement{
    public:

        typedef NODE NodeType;
        typedef EDGE EdgeType;

        AdjacencyElement(const NodeType & node = NodeType(), const EdgeType & edge = EdgeType())
        :   node_(node),
            edge_(edge){
        }

        const NodeType & node()const{
            return node_;
        }

        const EdgeType & edge()const{
            return edge_;
        }

    private:

        NodeType node_;
        EdgeType edge_;
    };


    template<class ADJACENCY_ELEMENT, class NODE_HASH_FUNCTOR>
    class AdjacencyElementHashFunctor{
    public:
        typedef std::size_t result_type;
        typedef ADJACENCY_ELEMENT argument_type;

        result_type operator()(const argument_type & arg)const{
            return node_hash_functor_(arg.node());
        }
    private:
        NODE_HASH_FUNCTOR node_hash_functor_;
    };

    template<class ADJACENCY_ELEMENT, class NODE_COMPARE_FUNCTOR>
    class AdjacencyElementCompareFunctor{
    public:
        typedef std::size_t result_type;
        typedef ADJACENCY_ELEMENT argument_type;

        result_type operator()(const argument_type & arg_a, const argument_type & arg_b)const{
            return node_compare_functor_(arg_a.node(), arg_b.node());
        }
    private:
        NODE_COMPARE_FUNCTOR node_compare_functor_;
    };


    // // example impl
    // template<class SET_IMPL>
    // class AdjacencyElementSet{
    // public:


    //     typedef SET_IMPL SetImplType;
    //     typedef typename SetImplType::iterator SetImplIter;
    //     typedef typename SetImplType::const_iterator SetImplConstIter;
    //     typedef typename SetImplType::value_type NodeEdgePair;
    //     typedef typename NodeEdgePair::first_type NodeType;
    //     typedef typename NodeEdgePair::second_type EdgeType;

    //     void add_edge(const NodeType & node, const EdgeType & edge){
    //         set_.insert(NodeEdgePair(node, edge));
    //     }

    //     SetImplIter find_edge(const NodeType & node){
    //         return set_.find(NodeEdgePair(node, EdgeType()));
    //     }

    //     SetImplIter end(){
    //         return set_.end();
    //     }
    //     SetImplConstIter end()const{
    //         return set_.end();
    //     }

    // private:
    //     SetImplType set_;
    // };


}
}