#pragma once

// std
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

// boost
#include "boost/container/flat_set.hpp"

// nifty graph
#include "nifty/graph/graphs/ugraph_base.hpp"
#include "nifty/graph/graphs/building_blocks/adjacency_element.hpp"

// nifty tools
#include "nifty/tools/map_key_iterator.hpp"
#include "nifty/tools/range.hpp"




namespace nifty {
namespace graph{




    

    ///\cond
    namespace detail_dynamic_graph{

    }
    ///\endcond




    // a networkx-ish  graph


    template<
        class NODE_TYPE,
        bool HASH_NODE_ADJACENCY,
        template<typename _KEY, typename _VAL, typename _HASH_OR_COMP, class ... ARGS> 
        class NODE_MAP,
        template<typename _KEY, typename _VAL,class ... ARGS> 
        class EDGE_MAP,
        template<typename _KEY, typename _HASH_OR_COMP,class ... ARGS> 
        class ADJACENCY_SET,
        class NODE_HASH_OR_COMPARE_FUNCTOR,
        class NODE_ADJACENCY_HASH_OR_COMPARE_FUNCTOR
    >
    class DynamicUGraphImpl
    : public UGraphBase<
        DynamicUGraphImpl<NODE_TYPE, HASH_NODE_ADJACENCY, NODE_MAP, EDGE_MAP, 
            ADJACENCY_SET, NODE_HASH_OR_COMPARE_FUNCTOR,NODE_ADJACENCY_HASH_OR_COMPARE_FUNCTOR>>
    {

    public:
        typedef NODE_TYPE NodeType;
        typedef uint64_t  EdgeType;

        typedef const NodeType & NodeConstRef;
        typedef EdgeType         EdgeConstRef;


    private:
        typedef NODE_HASH_OR_COMPARE_FUNCTOR NodeHashOrCompareFunctor;
        typedef NODE_ADJACENCY_HASH_OR_COMPARE_FUNCTOR NodeAdjacencyElementHashOrCompareFunctor;
        typedef AdjacencyElement<NodeType, EdgeType>       AdjacencyElementType;
        typedef std::pair<NodeType, NodeType>       NodePair;

        typedef typename std::conditional<
            HASH_NODE_ADJACENCY, 
            AdjacencyElementHashFunctor<AdjacencyElementType,      NodeAdjacencyElementHashOrCompareFunctor>,
            AdjacencyElementCompareFunctor<AdjacencyElementType,   NodeAdjacencyElementHashOrCompareFunctor>
        >::type AdjacencyElementTypeHashOrCompareFunctor;

        typedef ADJACENCY_SET<AdjacencyElementType, AdjacencyElementTypeHashOrCompareFunctor>   AdjacencyElementSet;
        typedef NODE_MAP<NodeType, AdjacencyElementSet, NodeHashOrCompareFunctor>      NodeStorageMap;
        typedef EDGE_MAP<EdgeType, NodePair>                                    EdgeStorageMap;

    public:

        // iterators WICH MUST BE PROVIDED
        typedef tools::MapKeyIterator<NodeStorageMap> NodeIterator;
        typedef tools::MapKeyIterator<EdgeStorageMap> EdgeIterator;
        typedef typename AdjacencyElementSet::const_iterator AdjacencyIterator;




        DynamicUGraphImpl()
        :   node_storage_map_(),
            edge_storage_map_(),
            next_edge_id_(0)
        {

        }

        auto n_nodes()const{
            return node_storage_map_.size();
        }

        auto n_edges()const{
            return edge_storage_map_.size();
        }


        std::pair<EdgeType, bool> find_edge(const NodeType & u, const NodeType & v){
            auto u_iter = this->node_storage_map_.find(u);
            if(u_iter == this->node_storage_map_.end() && this->contains_node(v)){
                const auto & u_storage = u_iter->second;
                // check if we already have this edge
                const auto adj_iter = u_storage.find(AdjacencyElementType(v,EdgeType()));
                if(adj_iter != u_storage.end()){
                    const auto & node_edge_pair = *adj_iter;
                    return std::make_pair(node_edge_pair.second, true);
                }
            }
            return std::make_pair(EdgeType(), false);
        }

        NodeIterator nodes_begin()const{
            return NodeIterator(node_storage_map_.begin());
        }
        NodeIterator nodes_end()const{
            return NodeIterator(node_storage_map_.end());
        }

        EdgeIterator edges_begin()const{
            return EdgeIterator(edge_storage_map_.begin());
        }
        EdgeIterator edges_end()const{
            return EdgeIterator(edge_storage_map_.end());
        }

        AdjacencyIterator adjacency_begin(const NodeType & node)const{
            const auto map_iter = node_storage_map_.find(node);
            return map_iter->second.begin();
        }

        AdjacencyIterator adjacency_end(const NodeType & node)const{
            const auto map_iter = node_storage_map_.find(node);
            return map_iter->second.end();
        }

        bool contains_node(const NodeType & node)const{
            return this->node_storage_map_.find(node) != this->node_storage_map_.end();
        }
        bool contains_edge(const EdgeType & edge)const{
            return this->edge_storage_map_.find(edge) != this->edge_storage_map_.end();
        }

        bool add_node(const NodeType & node){
            auto it = node_storage_map_.find(node);
            if(it == node_storage_map_.end()){
                const auto kv = std::make_pair(node,AdjacencyElementSet());
                node_storage_map_.insert(kv);
                return true;
            }
            return false;
        }       

        std::pair<EdgeType, bool> add_edge(const NodeType & u, const NodeType & v){
            
            // make sure node exists
            auto ensure_node_adjacency_set = [this](const NodeType & node)-> AdjacencyElementSet & {
                auto node_iter = this->node_storage_map_.find(node);
                if(node_iter == this->node_storage_map_.end()){
                    const auto kv = std::make_pair(node,AdjacencyElementSet());
                    node_iter = this->node_storage_map_.insert(kv).first;
                }
                return  node_iter->second;
            }; 

            auto & u_storage = ensure_node_adjacency_set(u);
            auto & v_storage = ensure_node_adjacency_set(v);

            // check if we already have this edge
            auto adj_iter = u_storage.find(AdjacencyElementType(v,EdgeType()));
            
            //  the edge exists already
            if(adj_iter != u_storage.end()){
                const auto & node_edge_pair = *adj_iter;
                return std::make_pair(node_edge_pair.edge(), false);
            }

            // insert fresh edge
            const EdgeType edge_id = next_edge_id_;
            ++next_edge_id_;

            edge_storage_map_.insert(std::make_pair(edge_id, NodePair(u, v)));
            u_storage.insert(AdjacencyElementType(v, edge_id));
            v_storage.insert(AdjacencyElementType(u, edge_id));

            return std::make_pair(edge_id, true);
        }   
    private:
         


        NodeStorageMap node_storage_map_;
        EdgeStorageMap edge_storage_map_;
        EdgeType next_edge_id_;

    };




    template<class NODE_TYPE, class NODE_HASH_FUNCTOR = std::hash<NODE_TYPE> >
    using DynamicUGraphHashMapImpl = DynamicUGraphImpl<
        NODE_TYPE,
        true,
        std::unordered_map,std::unordered_map,std::unordered_set, 
        NODE_HASH_FUNCTOR,
        NODE_HASH_FUNCTOR
    >;


    template<class NODE_TYPE, class COMPARE_FUNCTOR = std::less<NODE_TYPE> >
    using DynamicUGraphMapImpl = DynamicUGraphImpl<
        NODE_TYPE,
        false,
        std::map,std::map, boost::container::flat_set, 
        COMPARE_FUNCTOR,
        COMPARE_FUNCTOR
    >;

    template<
        class NODE_TYPE = int64_t, 
        class HASH_FUNCTOR = std::hash<NODE_TYPE> ,
        class COMPARE_FUNCTOR = std::less<NODE_TYPE>
    >
    using DynamicUGraph = DynamicUGraphImpl<
        NODE_TYPE,
        false,
        std::unordered_map,std::unordered_map,boost::container::flat_set, 
        HASH_FUNCTOR,
        COMPARE_FUNCTOR
    >;



} // end namespace nifty::graph
} // end namespace nifty

