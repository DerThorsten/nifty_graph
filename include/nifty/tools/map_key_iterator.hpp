#pragma once


#include <boost/iterator/transform_iterator.hpp>


namespace nifty {
namespace tools {

    template<class MAP>
    struct GetKey{
        typedef typename MAP::value_type map_value_type;
        typedef typename MAP::key_type map_key_type;
        const map_key_type & operator()(const map_value_type & kv)const{
            return kv.first;
        } 
    };



    template<class MAP>
    class MapKeyIterator
    : public boost::transform_iterator<
        GetKey<MAP>, 
        typename MAP::const_iterator
    > 
    {
    public:
        typedef MAP MapType;
        typedef typename MapType::const_iterator MapIterator;
        typedef GetKey<MapType> GetKeyFunctor;
        typedef boost::transform_iterator<GetKeyFunctor, MapIterator> BaseType;

        MapKeyIterator(const MapIterator & iter)
        :   BaseType(iter, GetKeyFunctor()){
            
        }
    };
}
}