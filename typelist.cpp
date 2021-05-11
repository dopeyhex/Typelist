#include <iostream>

struct NullType {};

template <typename ...Args>
struct TypeList {
    using Head = NullType;
    using Tail = NullType;
};

template <typename THead>
struct TypeList<THead> {
    using Head = THead;
    using Tail = NullType;
};

template <typename THead, typename ...TTail>
struct TypeList<THead, TTail...> {
    using Head = THead;
    using Tail = TypeList<TTail...>;
};

///////////////////////////////////////

template <typename T1, typename T2>
struct IsEmpty {
    static constexpr bool empty = false;
};

template <typename T>
struct IsEmpty<T, T> {
    static constexpr bool empty = true;
};

///////////////////////////////////////

template <size_t pos, typename ...TTail>
struct TypePosition {
    using value = NullType;
};

template <size_t pos>
struct TypePosition<pos, TypeList<>> {
    using value = NullType;
};

template <size_t pos>
struct TypePosition <pos, NullType> {
    using value = NullType;
};

template <size_t pos, typename ...TTail>
struct TypePosition<pos, TypeList<TTail...>> {
    using value = typename TypePosition<pos - 1, typename TypeList<TTail...>::Tail>::value;
};

template <typename ...TTail>
struct TypePosition<0, TypeList<TTail...>> {
    using value = typename TypeList<TTail...>::Head;
};

///////////////////////////////////////

template <class TypeList, template <class Type, class Base> class Unit,
    size_t left = 0, size_t center = 0, size_t right = 0,
    bool empty = false>
struct GenCustomHierarchy {};

//Every second level of tree is "linear", so it's okay to check only splitting branches
//NextLevelHierarchy basically splits a branch in 3 and checks if the center one isn't empty
/*
template <template <class Type, class Base> class Unit,
    size_t n, size_t left, size_t center, size_t right, class ...TTail>
using NextLevelHierarchy =
        GenCustomHierarchy<TypeList<TTail...>,
                           Unit,
                           right + 1 + (right - left + 1) * 3, //new left branch
                           right + (center - left) * 3 + n + (right - left + 1) * 3, //new center branch
                           right + (right - left + 1) * 6, //new right branch
                           IsEmpty<
                               typename TypePosition<right + (center - left) * 3 + n - 1 + (right - left + 1) * 3, TypeList<TTail...>>::value, //Check if current center branch is not empty
                               NullType
                           >::empty
        >;

Судя по всему, IsEmpty не пересчитывается:(
*/
template <size_t n, template <class Type, class Base> class Unit,
    size_t left, size_t center, size_t right,  class ...TTail>
using NChild =
       Unit<typename TypePosition<right + (center - left) * 3 + n - 1, TypeList<TTail...>>::value, //next level of branch
            Unit<typename TypePosition<right + (center - left) * 3 + n - 1 + (right - left + 1) * 3,
                                       TypeList<TTail...> >::value, //level after next level of branch
                 GenCustomHierarchy<TypeList<TTail...>,
                                    Unit,
                                    right + 1 + (right - left + 1) * 3, //new left branch
                                    right + (center - left) * 3 + n + (right - left + 1) * 3, //new center branch
                                    right + (right - left + 1) * 6, //new right branch
                                    IsEmpty<
                                        typename TypePosition<right + (center - left) * 3 + n - 1 + (right - left + 1) * 3, TypeList<TTail...>>::value, //Check if current center branch is not empty
                                        NullType
                                    >::empty
                 >
        >>;

//splitting branches
template <class ...TTail, template <class Type, class Base> class Unit,
    size_t left, size_t center, size_t right>
struct GenCustomHierarchy<TypeList<TTail...>, Unit, left, center, right, false> :
    NChild<1, Unit, left, center, right, TTail...>,
    NChild<2, Unit, left, center, right, TTail...>,
    NChild<3, Unit, left, center, right, TTail...>
{
    void Draw() {
        std::cout << center << " begin\n"; 
        NChild<1, Unit, left, center, right, TTail...>::Draw();
        NChild<2, Unit, left, center, right, TTail...>::Draw();
        NChild<3, Unit, left, center, right, TTail...>::Draw();
        std::cout<<center<<" end\n";
    }
};

template <class ...TTail, template <class Type, class Base> class Unit,
    size_t left, size_t center, size_t right>
struct GenCustomHierarchy<TypeList<TTail...>, Unit, left, center, right, true> {
    void Draw() {
        std::cout << "hello" << center << "\n";
    }
};

///////////////////////////////////////   
 
template <typename T, class Base>
struct Unit : Base
{
    T _obj;
    void Draw() {
        _obj.Draw();
        Base::Draw();
    }
};

template <typename T>
struct Unit<NullType, T>
{
    void Draw() {
    }
};

///////////////////////////////////////

template <size_t index>
struct Type {
    void Draw() {
        std::cout << "Current type: " << index << "\n";
    }
};

int main() {
    GenCustomHierarchy <
        TypeList <
            Type<1>,
            Type<2>,
            Type<3>,
            Type<4>,
            Type<5>,
            Type<6>,
            Type<7>,
            Type<8>,
            Type<9>,
            Type<10>,
            Type<11>,
            Type<12>,
            Type<13>,
            Type<14>,
            Type<15>,
            Type<16>,
            Type<17>,
            Type<18>,
            Type<19>,
            Type<20>,
            Type<21>,
            Type<22>,
            Type<23>,
            Type<24>,
            Type<25>,
            Type<26>,
            Type<27>,
            Type<28>,
            Type<29>,
            Type<30>,
            Type<31>,
            Type<32>,
            Type<33>,
            Type<34>,
            Type<35>,
            Type<36>,
            Type<37>,
            Type<38>,
            Type<39>,
            Type<40>,
            Type<41>,
            Type<42>,
            Type<43>,
            Type<44>,
            Type<45>,
            Type<46>,
            Type<47>,
            Type<48>,
            Type<49>,
            Type<50>,
            Type<51>>,
        Unit> typeTree;
        typeTree.Draw();
    return 0;
}