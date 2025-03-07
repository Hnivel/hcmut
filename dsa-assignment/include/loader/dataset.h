/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/*
 * File:   dataset.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 3:59 PM
 */

#ifndef DATASET_H
#define DATASET_H
#include "tensor/xtensor_lib.h"
using namespace std;

template <typename DType, typename LType>
class DataLabel
{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;

public:
    DataLabel(xt::xarray<DType> data, xt::xarray<LType> label) : data(data), label(label) {}
    xt::xarray<DType> getData() const
    {
        return this->data;
    }
    xt::xarray<LType> getLabel() const
    {
        return this->label;
    }
};

template <typename DType, typename LType>
class Batch
{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;

public:
    Batch() : data(xt::xarray<DType>{}), label(xt::xarray<LType>{}) {}
    Batch(xt::xarray<DType> data, xt::xarray<LType> label) : data(data), label(label) {}
    virtual ~Batch() {}
    xt::xarray<DType> &getData()
    {
        return this->data;
    }
    xt::xarray<LType> &getLabel()
    {
        return this->label;
    }
};

template <typename DType, typename LType>
class Dataset
{
private:
public:
    Dataset() {};
    virtual ~Dataset() {};

    virtual int len() = 0;
    virtual DataLabel<DType, LType> getitem(int index) = 0;
    virtual xt::svector<unsigned long> get_data_shape() = 0;
    virtual xt::svector<unsigned long> get_label_shape() = 0;
    //
    virtual xt::xarray<DType> get_data() = 0;
    virtual xt::xarray<LType> get_label() = 0;
};

//////////////////////////////////////////////////////////////////////
template <typename DType, typename LType>
class TensorDataset : public Dataset<DType, LType>
{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
    xt::svector<unsigned long> data_shape, label_shape;

public:
    // Constructor (Done)
    TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label)
    {
        this->data = data;
        this->label = label;
        this->data_shape = xt::svector<unsigned long>(data.shape().begin(), data.shape().end());
        this->label_shape = xt::svector<unsigned long>(label.shape().begin(), label.shape().end());
    }
    // Length (Done)
    int len()
    {
        return data.shape()[0];
    }
    // Get Item (Done)
    DataLabel<DType, LType> getitem(int index)
    {
        if (index < 0 || index >= this->len())
        {
            throw std::out_of_range("Index is out of range!");
        }
        if (label.shape().size() == 0 && label.dimension() == 0)
        {
            DataLabel<DType, LType> item(xt::view(data, index), label);
            return item;
        }
        else
        {
            DataLabel<DType, LType> item(xt::view(data, index, xt::all()), xt::view(label, index, xt::all()));
            return item;
        }
    }
    // Data Shape (Done)
    xt::svector<unsigned long> get_data_shape()
    {
        return this->data_shape;
    }
    // Label Shape (Done)
    xt::svector<unsigned long> get_label_shape()
    {
        return this->label_shape;
    }
    //
    xt::xarray<DType> get_data()
    {
        return this->data;
    }

    //
    xt::xarray<LType> get_label()
    {
        return this->label;
    }
};

#endif /* DATASET_H */
