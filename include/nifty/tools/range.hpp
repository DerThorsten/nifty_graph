#pragma once

namespace nifty{
namespace tools{

    template<class ITERATOR>
    class Range{
    public:
        typedef ITERATOR iterator;
        Range(iterator begin, iterator end)
        :   begin_(begin),
            end_(end){
        }

        iterator begin()const{
            return begin_;
        }
        iterator end()const{
            return end_;
        }
    private:
        iterator begin_;
        iterator end_;
    };
}
}