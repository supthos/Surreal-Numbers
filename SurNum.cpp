// 2025 (c) Reed Axewielder, G.M. Davila
// All rights reserved.
// SurNum.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <climits>
#include <string>
#include <concepts>


template<typename T>
concept Ordinal = requires(T a) {
    { a.isOrdinal() == true } -> std::convertible_to<bool>;
};


template<typename T>  
concept isNum = requires(T a) {
    { a.isNumber() } -> std::convertible_to<bool>;
};

template<typename T>
concept Relations = requires(T x, T y) {
    { x >= y } -> std::same_as<bool>;
    { x <= y } -> std::same_as<bool>;
    { x == y } -> std::same_as<bool>;
    { x < y } -> std::same_as<bool>;
    { x > y } -> std::same_as<bool>;
    { x + y } -> std::same_as<T>;
    { -y } -> std::same_as<T>;
    { x * y } -> std::same_as<T>;
};


template<typename T>  
concept Surreal = (isNum<T> && Relations<T>);
//concept Surreal = ((isNum<T> && Relations<T>) || std::is_arithmetic_v<T> );

template<Surreal T>
class SurNum {
public:
    T value;
    SurNum(const T& v) : value(v) {
        if (!v.isNumber()) {
            throw std::invalid_argument("Value is not surreal.");
        }
    }
    SurNum<T> operator+(const SurNum<T>& rhs) {
        return SurNum<T>(value + rhs.value);
    }
    SurNum<T> operator-(const SurNum<T>& rhs) {
        return SurNum<T>(value - rhs.value);
    }
    SurNum<T> operator-() {
        return SurNum<T>(-value);
    }
    SurNum<T> operator*(const SurNum<T>& rhs) {
        return SurNum<T>(value * rhs.value);
    }
    bool operator<(const SurNum<T>&rhs) {
        return (value < rhs.value);
    }
    bool operator>(const SurNum<T>& rhs) {
        return (value > rhs.value);
    }
    bool operator<=(const SurNum<T>& rhs) {
        return (value <= rhs.value);
    }
    bool operator>=(const SurNum<T>& rhs) {
        return (value >= rhs.value);
    }
    bool operator==(const SurNum<T>& rhs) {
        return (value == rhs.value);
    }
    bool operator!=(const SurNum<T>& rhs) {
        return (value != rhs.value);
    }

};


// Class of Partizan games.
class ParGam {
public:

    //tern sign = Z;
    //const ParGam* x = this;

    struct CustComp {
        bool operator()(const ParGam& a, const ParGam& b) const {
            return a.operator<(b); // Descending order
            //return b > a;
        }
    };

    typedef std::set<ParGam, ParGam::CustComp> options;
    

    options L{};
    options R{};

    ParGam() = default;

    ParGam( options l,  options r) {
        if (!l.empty()) {
            L = l;
        }
        if (!r.empty()) {
            R = r;
        }
    }

    std::string displ() {
        std::string s = "{";
        if (!L.empty())
        for (auto i : L) {
            s = s + i.displ();
        }
        
        s = s + "|";

        if (!R.empty())
        for (auto i : R) {
            s = s + i.displ();
        }
        s = s + "}";
        return s;
    }

    bool Ident(ParGam x, ParGam y) {
        if (x.L == y.L && x.R == y.R) return true;
        else return false;
    }

    ParGam Infimum(const  options  S) const {
        if (!S.empty())
            return *std::min_element(S.begin(), S.end(), ParGam::CustComp());
        else return ParGam( options {},  options {});
    }
    ParGam Supremum(const  options  S) const {
        if (!S.empty())
            return *std::max_element(S.begin(), S.end(), ParGam::CustComp());
        else return ParGam( options {},  options {});
    }

    bool operator >= (const ParGam& y) const {
        if (!R.empty() && Infimum(R) <= y) return false;

        if (!y.L.empty() && *this <= Supremum(y.L)) return false;

        return true;
    }
    bool operator <= (const ParGam& y) const {
        return (y >= *this);
    }

    bool operator==(const ParGam& y) const {
        return ((*this >= y) && (y >= *this));
    }

    bool operator>(const ParGam& y) const {
        return ((*this >= y) && (!(y >= *this)));
    }

    bool operator<(const ParGam& y) const {
        return (y > *this);
    }
    bool isNumber() const {
        if (!L.empty() && !R.empty()) {
            if (std::all_of(L.begin(), L.end(), [](const ParGam& i) -> bool { return i.isNumber(); }))
            if (Supremum(L) >= Infimum(R)) return false;
        }
        return true;
    }

    ParGam operator+(const ParGam& y) const {
        ParGam Sum( options {},  options {});
        if (!L.empty())
            for (auto& i : L) Sum.L.insert(i + y);
        if (!y.L.empty())
            for (auto& i : y.L) Sum.L.insert(*this + i);
        if (!R.empty())
            for (auto& i : R) Sum.R.insert(i + y);
        if (!y.R.empty())
            for (auto& i : y.R) Sum.R.insert(*this + i);
        return Sum;
    }

    ParGam operator-()const {
        ParGam InvSum( options {},  options {});
        if (!R.empty())
            for (auto& i : R) InvSum.L.insert(-i);

        if (!L.empty())
            for (auto& i : L) InvSum.R.insert(-i);

        return InvSum;
    }

    ParGam operator-(const ParGam& y) const {
        return (*this + (-y));
    }

    ParGam operator*(const ParGam& y) const {
        ParGam Sum( options {},  options {});
        ParGam Mul( options {},  options {});
        ParGam x = *this;

        if (!x.L.empty())
            for (auto& i : x.L) {
                if (!y.L.empty())
                    for (auto& j : y.L) {
                        Sum = (i * y) + (x * j) - (i * j);
                        Mul.L.insert(Sum);
                    }
            }
        if (!x.R.empty())
            for (auto& i : x.R) {
                if (!y.R.empty())
                    for (auto& j : y.R) {
                        Sum = (i * y) + (x * j) - (i * j);
                        Mul.L.insert(Sum);
                    }
            }
        if (!x.L.empty())
            for (auto& i : x.L) {
                if (!y.R.empty())
                    for (auto& j : y.R) {
                        Sum = (i * y) + (x * j) - (i * j);
                        Mul.R.insert(Sum);
                    }
            }
        if (!x.R.empty())
            for (auto& i : x.R) {
                if (!y.L.empty())
                    for (auto& j : y.L) {
                        Sum = (i * y) + (x * j) - (i * j);
                        Mul.R.insert(Sum);
                    }
            }
        //Mul.reduce();
        return Mul;
    }
    bool isOrdinal() {
        if (isNumber() && R.empty()) return true;
        else return false;
    }


};


int main()
{

    ParGam On1(ParGam::options{}, ParGam::options{});


    std::map<long, std::string> Num;

    ParGam::options  empty{};
    unsigned i = 0;
    try {
        std::cout << "Is O[0] a NUMBER? ";
        if (On1.isNumber()) {
            std::cout << "Yes!\n";
            Num[0] = On1.displ();
        }
        else std::cout << "No.\n";
        std::cout << On1.displ() << std::endl;
        
        On1 = ParGam(ParGam::options{On1}, ParGam::options{});
        std::cout << "Is O[1] a NUMBER? ";
        if (On1.isNumber()) {
            std::cout << "Yes!\n";
            Num[1] = On1.displ();
        }
        else std::cout << "No.\n";
        std::cout << On1.displ() << std::endl;

        On1 = -On1;

        std::cout << "Is O[-1] a NUMBER? ";
        if (On1.isNumber()) {
            std::cout << "Yes!\n";
            Num[-1] = On1.displ();
        }
        else std::cout << "No.\n";
        std::cout << On1.displ() << std::endl;
       
        for (i = 2; i<=500; i++) {
            On1 = ParGam(ParGam::options{ On1 }, ParGam::options{});
            Num[(long)i] = On1.displ();

            On1 = -On1;
            Num[-(long)i] = On1.displ();

            //std::cout << "Allocated " << i*2 << " elements\n";
            //std::cout << On[(long)i].displ() << std::endl;
        }
    }
    catch (const std::bad_alloc&) {
        std::cout << "Memory limit reached after " << i << " pairs of pairs.\n";
    }

    std::cout << "0 = " << Num[0] << std::endl;
    std::cout << "1 = " << Num[1] << std::endl;
    std::cout << "2 = " << Num[2] << std::endl;
    std::cout << "3 = " << Num[3] << std::endl;
    std::cout << "4 = " << Num[4] << std::endl;
    std::cout << "5 = " << Num[5] << std::endl;
    std::cout << "6 = " << Num[6] << std::endl;
    std::cout << "7 = " << Num[7] << std::endl;
    std::cout << "8 = " << Num[8] << std::endl;
    std::cout << "9 = " << Num[9] << std::endl;

    std::cout << std::endl;

    std::cout << "0 = " << Num[0] << std::endl;
    std::cout << "-1 = " << Num[-1] << std::endl;
    std::cout << "-2 = " << Num[-2] << std::endl;
    std::cout << "-3 = " << Num[-3] << std::endl;
    std::cout << "-4 = " << Num[-4] << std::endl;
    std::cout << "-5 = " << Num[-5] << std::endl;
    std::cout << "-6 = " << Num[-6] << std::endl;
    std::cout << "-7 = " << Num[-7] << std::endl;
    std::cout << "-8 = " << Num[-8] << std::endl;
    std::cout << "-9 = " << Num[-9] << std::endl;

    
}
