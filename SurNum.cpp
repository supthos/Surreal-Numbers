// 2025 (c) Reed Axewielder, G.M. Davila
// SurNum.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include<set>
#include<map>
#include<vector>
#include <climits>
#include <string>
#include <concepts>



// Concept: checks if T has isSurreal() member function
template<typename T>
concept Surreal = requires(T a) {
    { a.isSurreal() } -> std::convertible_to<bool>;
};

// Class of Partizan games.
class ParGam {
public:
    
    //tern sign = Z;
    //const ParGam* x = this;

    struct CustComp {
        bool operator()(ParGam a, ParGam b) const {
            return a.operator<( b) ; // Descending order
            //return b > a;
        }
    };
    std::set<ParGam, CustComp> L{};
    std::set<ParGam, CustComp> R{};

    ParGam() = default;
    ParGam(std::set<ParGam, ParGam::CustComp>l, std::set<ParGam, ParGam::CustComp>r) {
        if (!l.empty()) {
            L = l;
        }
        if (!r.empty()) {
            R = r;
        }
    }

    std::string displ() {
        std::string s = "{";
        for (auto i : L) {
            s = s + i.displ();
        }
        s = s + " | ";
        for (auto i : R) {
            s = s + i.displ();
        }
        s = s + "}";
        return s;
    }
    bool operator >= (const ParGam& y) const {
        if (!R.empty())
        for (auto& i : R) {
            if (i <= y) return false;
        }
        if (!y.L.empty())
        for (auto& i : y.L) {
            if (*this <= i) return false;
        }
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
    bool isSurreal() {
        if (!L.empty())
        for (auto& i : L) {
            if (!R.empty())
            for (auto& j : R) {
                if (i >= j) return false;
            }
        }
        return true;
    }

    ParGam operator+(const ParGam& y) const {
        ParGam Sum({},{});
        if (!L.empty())
        for (auto& i: L) Sum.L.insert(i+y);
        if (!y.L.empty())
        for (auto& i : y.L) Sum.L.insert(*this + i);
        if (!R.empty())
        for (auto& i : R) Sum.R.insert(i + y);
        if (!y.R.empty())
        for (auto& i : y.R) Sum.R.insert(*this + i);
        return Sum;
    }

    ParGam operator-()const {
        ParGam InvSum({}, {});
        if (!R.empty())
        for (auto& i: R) InvSum.L.insert(-i);
        if (!L.empty())
        for (auto& i: L) InvSum.R.insert(-i);
        return InvSum;
    }

    ParGam operator-(const ParGam& y) const {
        return (*this + (-y));
    }

    ParGam operator*(const ParGam& y) const {
        ParGam Sum({}, {});
        ParGam Mul({}, {});
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

        return Mul;
    }
};

// SurNum class constrained to types that satisfy SurrealLike
template<Surreal T>
class SurNum : public ParGam {
public:

    SurNum(const T& v) : L(v.L), R(v.R) {
        if (!v.isSurreal()) {
            throw std::invalid_argument("Value is not surreal.");
        }
    }

};

int main()
{

    std::map<long, ParGam> On;

    std::set<ParGam, ParGam::CustComp> empty{};
    
    try {
        On[0] = ParGam(std::set<ParGam, ParGam::CustComp>{}, std::set<ParGam, ParGam::CustComp>{});
        std::cout << "Is O[0] a NUMBER? ";
        if (On[0].isSurreal()) std::cout << "Yes!\n";
        else std::cout << "No.\n";
        std::cout << On[0].displ() << std::endl;

        On[1] = ParGam(std::set<ParGam, ParGam::CustComp>{ On[0] }, std::set<ParGam, ParGam::CustComp>{});
        std::cout << "Is O[1] a NUMBER? ";
        if (On[1].isSurreal()) std::cout << "Yes!\n";
        else std::cout << "No.\n";
        std::cout << On[0].displ() << std::endl;
        std::cout << On[1].displ() << std::endl;

        On[-1] = ParGam(std::set<ParGam, ParGam::CustComp>{}, std::set<ParGam, ParGam::CustComp>{ On[0] });
        std::cout << "Is O[-1] a NUMBER? ";
        if (On[-1].isSurreal()) std::cout << "Yes!\n";
        else std::cout << "No.\n";
       
        for (unsigned i = 2; i<=300; ++i) {
            On[(long)i] = ParGam(std::set<ParGam, ParGam::CustComp>{ On[i - 1] }, std::set<ParGam, ParGam::CustComp>{});
            On[-(long)i] = ParGam(std::set<ParGam, ParGam::CustComp>{}, std::set<ParGam, ParGam::CustComp>{ On[i - 1] });

            //std::cout << "Allocated " << i*2 << " elements\n";
            //std::cout << On[(long)i].displ() << std::endl;
        }
    }
    catch (const std::bad_alloc&) {
        std::cout << "Memory limit reached after " << On.size() << " elements.\n";
    }

    std::cout << "0 = " << On[0].displ() << std::endl;
    std::cout << "1 = " << (On[0] + On[1]).displ() << std::endl;
    std::cout << "2 = " << On[2].displ() << std::endl;
    std::cout << "3 = " << On[3].displ() << std::endl;
    std::cout << "4 = " << On[4].displ() << std::endl;
    std::cout << "5 = " << On[5].displ() << std::endl;
    std::cout << "6 = " << On[6].displ() << std::endl;
    std::cout << "7 = " << On[7].displ() << std::endl;
    std::cout << "8 = " << On[8].displ() << std::endl;
    std::cout << "9 = " << On[9].displ() << std::endl;
    std::cout << "10 = " << On[10].displ() << std::endl;

    ParGam A = On[8] + On[3];
    std::cout << "A = " << A.displ() << std::endl;
    if (A == On[11]) {
        std::cout << "True\n";
    }
    else std::cout << "False\n";


    std::cout << "Hello World!\n";
    
}
