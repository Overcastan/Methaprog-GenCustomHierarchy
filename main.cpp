#include <iostream>

class Nulltype {
};

template<typename ...Args>
struct TypeList {
    using Head = Nulltype;
    using Tail = Nulltype;
};

template<typename H, typename ...T>
struct TypeList<H, T...> {
    using Head = H;
    using Tail = TypeList<T...>;
};

template<typename H>
struct TypeList<H> {
    using Head = H;
    using Tail = Nulltype;
};

//...................................................................//

template<typename T>
struct Holder {
    T _obj;
};

//...................................................................//

template<size_t ind, typename T>
struct TypeAt {
    using res = Nulltype;
};

template<size_t ind, typename ...T>
struct TypeAt<ind, TypeList<T...>> {
    using res = typename TypeAt<ind - 1, typename TypeList<T...>::Tail>::res;
};

template<size_t ind>
struct TypeAt<ind, Nulltype> {
    using res = Nulltype;
};

template<typename ...T>
struct TypeAt<0, TypeList<T...>> {
    using res = typename TypeList<T...>::Head;
};

//...................................................................//

template<size_t n>
struct Power {
    static const size_t result = 3 * Power<n - 1>::result;
};

template<>
struct Power<0> {
    static const size_t result = 1;
};

//...................................................................//

template<size_t h>
struct Sum {
    static const size_t result = Power<(h + 1) / 2>::result + Sum<h - 1>::result;
};

template<>
struct Sum<1> {
    static const size_t result = 3;
};

//...................................................................//

template<template<class> class Unit, size_t h, size_t ind, class TList>
struct LinearToScatter;

template<typename T, template<class> class Unit, size_t h, size_t ind, class TList>
struct GenLinearHierarchy;

template<template<class> class Unit, size_t h, size_t ind, class TList>
struct ScatterToLinear;

template<typename T, template<class> class Unit, size_t h, size_t ind, class TList>
struct GenScatterHierarchy;


template<template<class> class Unit, size_t h, size_t ind, class ...TList>
struct LinearToScatter<Unit, h, ind, TypeList<TList...>> :
public GenScatterHierarchy<typename TypeAt<ind, TypeList<TList...>>::res, Unit, h, ind, TypeList<TList...>> {
    using Base = GenScatterHierarchy<typename TypeAt<ind, TypeList<TList...>>::res, Unit, h, ind, TypeList<TList...>>;
};


template<typename T, template<class> class Unit, size_t h, size_t ind, class ...TList>
struct GenLinearHierarchy<T, Unit, h, ind, TypeList<TList...>> :
        public Unit<T>,
        public LinearToScatter<Unit, h + 1, ind + Power<(h + 1) / 2>::result, TypeList<TList...>> {
    using UnitType = Unit<T>;
};


template<template<class> class Unit, size_t h, size_t ind, class ...TList>
struct ScatterToLinear<Unit, h, ind, TypeList<TList...>> :
        public GenLinearHierarchy<typename TypeAt<ind, TypeList<TList...>>::res, Unit, h, ind, TypeList<TList...>>,
        public GenLinearHierarchy<typename TypeAt<ind + 1, TypeList<TList...>>::res, Unit, h, ind + 1, TypeList<TList...>>,
        public GenLinearHierarchy<typename TypeAt<ind + 2, TypeList<TList...>>::res, Unit, h, ind + 2, TypeList<TList...>> {
    using LeftBase = GenLinearHierarchy<typename TypeAt<ind, TypeList<TList...>>::res, Unit, h, ind, TypeList<TList...>>;
    using MiddleBase = GenLinearHierarchy<typename TypeAt<ind + 1, TypeList<TList...>>::res, Unit, h, ind + 1, TypeList<TList...>>;
    using RightBase = GenLinearHierarchy<typename TypeAt<ind + 2, TypeList<TList...>>::res, Unit, h, ind + 2, TypeList<TList...>>;
};


template<typename T, template<class> class Unit, size_t h, size_t ind, class ...TList>
struct GenScatterHierarchy<T, Unit, h, ind, TypeList<TList...>> :
        public Unit<T>,
        public ScatterToLinear<Unit, h + 1, Sum<h>::result + 3 * (ind - Sum<h - 1>::result), TypeList<TList...>> {
    using UnitType = Unit<T>;
};

template<template<class> class Unit, size_t h, size_t ind, class ...TList>
struct GenLinearHierarchy<Nulltype, Unit, h, ind, TypeList<TList...>> {
};

template<template<class> class Unit, size_t h, size_t ind, class ...TList>
struct GenScatterHierarchy<Nulltype, Unit, h, ind, TypeList<TList...>> {
};

template<typename TL, template<class> class Unit>
struct BaseUnit {

};

template<typename ...TList, template<class> class Unit>
struct BaseUnit<TypeList<TList...>, Unit> :
        public ScatterToLinear<Unit, 1, 0, TypeList<TList...>> {

};


int main() {
    BaseUnit<TypeList<int, float, double, char, float>, Holder> g;
//    static_cast<Unit<int>>(g);


}