#pragma once
#include <vector>
#include "../printable/Printable.hpp"
#include "../readable/Readable.hpp"
template<template<typename>typename ptr_type, typename t>
class PtrVector : public Printable, public Readable{
private:
    vector<ptr_type<t>> p_vector;
public:
    PtrVector(const std :: vector<ptr_type<t>>& p_vector) : p_vector(p_vector){};
    const std :: vector<ptr_type<t>>& getVector(){}
    void print(std :: ostream& out) override const{
        for (const auto& elem : this->p_vector)
            elem->print(out);
    }
    
    vector<ptr_type<t>> clone(){
        vector<ptr_type<t>> cloned_elem;
        cloned_elem.reserve(this->p_vector.size());

        for (const auto& elem : this->p_vector)
            cloned_elem.push_back(move(elem->clone()));
    }
    ~PtrVector(){};
};
