/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/*
 * File:   CrossEntropy.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"
#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction) : ILossLayer(reduction)
{
}

CrossEntropy::CrossEntropy(const CrossEntropy &orig) : ILossLayer(orig)
{
}

CrossEntropy::~CrossEntropy()
{
}
// (Done)
double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t)
{
    this->m_aCached_Ypred = X;
    this->m_aYtarget = t;
    return cross_entropy(X, t, this->m_eReduction == REDUCE_MEAN);
}
// (Undone)
xt::xarray<double> CrossEntropy::backward()
{
    double nsamples = this->m_aCached_Ypred.shape()[0];
    xt::xarray<double> DY = (-1.0 * (this->m_aYtarget / (this->m_aCached_Ypred + 1e-7))); // Avoid division by 0
    if (this->m_eReduction == REDUCE_MEAN)
    {
        DY /= nsamples;
    }
    return DY;
}