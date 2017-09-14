#pragma once



namespace nifty{
namespace graph{

    template<class DERIVED>
    class AdjacencyBase{
    public:


        auto degree()const{ 
            const auto begin_iter = derived().begin();
            const auto end_iter = derived().end();
            return std::distance(begin_iter, end_iter);
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