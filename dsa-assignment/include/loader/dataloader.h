/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/*
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "tensor/xtensor_lib.h" // ann/xtensor_lib.h -> loader/xtensor_lib.h
#include "loader/dataset.h"     // ann/dataset.h -> loader/dataset.h

using namespace std;

template <typename DType, typename LType>
class DataLoader
{
public:
    class Iterator;

private:
    Dataset<DType, LType> *ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    int seed;
    xt::xarray<unsigned long> indices;

public:
    // Constructor
    DataLoader(Dataset<DType, LType> *ptr_dataset, int batch_size, bool shuffle = true, bool drop_last = false, int seed = -1)
    {
        this->ptr_dataset = ptr_dataset;
        this->batch_size = batch_size;
        this->shuffle = shuffle;
        this->drop_last = drop_last;
        this->indices = xt::arange<unsigned long>(ptr_dataset->len());
        this->seed = seed;
    }
    virtual ~DataLoader() {}
    Iterator begin()
    {
        if (this->shuffle == true && seed >= 0)
        {
            xt::random::seed(seed);
            xt::random::shuffle(indices);
        }
        else if (shuffle == true)
        {
            xt::random::shuffle(indices);
        }
        return Iterator(this, 0);
    }
    Iterator end()
    {
        if (this->ptr_dataset->len() < batch_size && drop_last == false)
        {
            return Iterator(this, 0);
        }
        return Iterator(this, (this->ptr_dataset->len() / batch_size));
    }
    int get_batch_size()
    {
        return batch_size;
    }
    int get_sample_count()
    {
        return ptr_dataset->len();
    }
    int get_total_batch()
    {
        return int(ptr_dataset->len() / batch_size);
    }
    class Iterator
    {
    private:
        DataLoader *loader;
        unsigned long current;
        Batch<DType, LType> current_batch;

    public:
        Iterator(DataLoader *loader, int current)
        {
            this->loader = loader;
            this->current = current;
        }
        bool operator!=(const Iterator &iterator)
        {
            return this->current != iterator.current;
        }
        Batch<DType, LType> operator*()
        {
            unsigned long start = current * static_cast<unsigned long>(loader->batch_size);
            unsigned long end = std::min(start + static_cast<unsigned long>(loader->batch_size), static_cast<unsigned long>(loader->ptr_dataset->len()));
            // If drop_last is false
            if (loader->drop_last == false && current == (loader->ptr_dataset->len() / loader->batch_size) - 1)
            {
                end = loader->ptr_dataset->len();
            }
            unsigned long batch_size = end - start;
            auto data_shape = loader->ptr_dataset->get_data_shape();
            data_shape[0] = batch_size;
            auto label_shape = loader->ptr_dataset->get_label_shape();
            label_shape[0] = batch_size;
            xt::xarray<DType> data = xt::empty<DType>(data_shape);
            xt::xarray<LType> label = xt::empty<LType>(label_shape);
            for (int i = 0; i < (end - start); i++)
            {
                unsigned long index = loader->indices(i + start);
                DataLabel<DType, LType> item = loader->ptr_dataset->getitem(index);
                if (data.dimension() != 0)
                {
                    xt::view(data, i) = item.getData();
                }
                if (label.dimension() != 0)
                {
                    xt::view(label, i) = item.getLabel();
                }
            }
            this->current_batch = Batch<DType, LType>(data, label);
            if (this->loader->ptr_dataset->get_label().dimension() == 0)
            {
                this->current_batch = Batch<DType, LType>(data, this->loader->ptr_dataset->get_label());
            }
            return current_batch;
        }
        Iterator operator++()
        {
            this->current++;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };

    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};

#endif /* DATALOADER_H */
