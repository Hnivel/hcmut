/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/*
 * File:   AdamParamGroup.cpp
 * Author: ltsach
 *
 * Created on October 8, 2024, 1:43 PM
 */

#include "optim/AdamParamGroup.h"

AdamParamGroup::AdamParamGroup(double beta1, double beta2) : m_beta1(beta1), m_beta2(beta2)
{
    // Create some maps:
    m_pParams = new xmap<string, xt::xarray<double> *>(&stringHash);
    m_pGrads = new xmap<string, xt::xarray<double> *>(&stringHash);
    m_pFirstMomment = new xmap<string, xt::xarray<double> *>(
        &stringHash,
        0.75,
        0,
        xmap<string, xt::xarray<double> *>::freeValue);
    m_pSecondMomment = new xmap<string, xt::xarray<double> *>(
        &stringHash,
        0.75,
        0,
        xmap<string, xt::xarray<double> *>::freeValue);
    //
    m_step_idx = 1;
    m_beta1_t = m_beta1;
    m_beta2_t = m_beta2;
}

AdamParamGroup::AdamParamGroup(const AdamParamGroup &orig) : m_beta1(orig.m_beta1), m_beta2(orig.m_beta2)
{
    m_pParams = new xmap<string, xt::xarray<double> *>(&stringHash);
    m_pGrads = new xmap<string, xt::xarray<double> *>(&stringHash);
    m_pFirstMomment = new xmap<string, xt::xarray<double> *>(
        &stringHash,
        0.75,
        0,
        xmap<string, xt::xarray<double> *>::freeValue);
    m_pSecondMomment = new xmap<string, xt::xarray<double> *>(
        &stringHash,
        0.75,
        0,
        xmap<string, xt::xarray<double> *>::freeValue);
    // copy:
    *m_pParams = *orig.m_pParams;
    *m_pGrads = *orig.m_pGrads;
    *m_pFirstMomment = *orig.m_pFirstMomment;
    *m_pSecondMomment = *orig.m_pSecondMomment;
    //
    m_step_idx = 1;
    m_beta1_t = m_beta1;
    m_beta2_t = m_beta2;
}

AdamParamGroup::~AdamParamGroup()
{
    if (m_pFirstMomment != nullptr)
        delete m_pFirstMomment;
    if (m_pSecondMomment != nullptr)
        delete m_pSecondMomment;
}
// (Undone)
void AdamParamGroup::register_param(string param_name, xt::xarray<double> *ptr_param, xt::xarray<double> *ptr_grad)
{
    m_pParams->put(param_name, ptr_param);
    m_pGrads->put(param_name, ptr_grad);
    m_pFirstMomment->put(param_name, new xt::xarray<double>(xt::zeros_like(*ptr_param)));
    m_pSecondMomment->put(param_name, new xt::xarray<double>(xt::zeros_like(*ptr_param)));
}
void AdamParamGroup::register_sample_count(unsigned long long *pCounter)
{
    m_pCounter = pCounter;
}

// (Undone)
void AdamParamGroup::zero_grad()
{
    DLinkedList<string> keys = m_pGrads->keys();
    for (auto key : keys)
    {
        xt::xarray<double> *grad = m_pGrads->get(key);
        xt::xarray<double> *param = m_pParams->get(key);
        *grad = xt::zeros<double>(param->shape());
    }
}
// (Undone)
void AdamParamGroup::step(double lr)
{
    DLinkedList<string> keys = m_pParams->keys();
    for (auto key : keys)
    {
        xt::xarray<double> *param = m_pParams->get(key);
        xt::xarray<double> *grad = m_pGrads->get(key);
        xt::xarray<double> *m = m_pFirstMomment->get(key);
        xt::xarray<double> *v = m_pSecondMomment->get(key);

        *m = m_beta1 * (*m) + (1 - m_beta1) * (*grad);
        *v = m_beta2 * (*v) + (1 - m_beta2) * xt::pow(*grad, 2);

        xt::xarray<double> m_hat = *m / (1 - std::pow(m_beta1, m_step_idx));
        xt::xarray<double> v_hat = *v / (1 - std::pow(m_beta2, m_step_idx));
        *param -= lr * m_hat / (xt::sqrt(v_hat) + 1e-8);
    }
    // (Done)
    m_step_idx += 1;
    m_beta1_t *= m_beta1;
    m_beta2_t *= m_beta2;
}
