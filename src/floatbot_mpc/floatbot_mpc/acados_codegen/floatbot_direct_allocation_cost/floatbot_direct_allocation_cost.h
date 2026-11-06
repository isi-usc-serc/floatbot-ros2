/*
 * Copyright (c) The acados authors.
 *
 * This file is part of acados.
 *
 * Licensed under the 2-Clause BSD License.
 */


#ifndef floatbot_direct_allocation_COST
#define floatbot_direct_allocation_COST

#ifdef __cplusplus
extern "C" {
#endif


// Cost at initial shooting node

int floatbot_direct_allocation_cost_y_0_fun(const real_t** arg, real_t** res, int* iw, real_t* w, void *mem);
int floatbot_direct_allocation_cost_y_0_fun_work(int *, int *, int *, int *);
const int *floatbot_direct_allocation_cost_y_0_fun_sparsity_in(int);
const int *floatbot_direct_allocation_cost_y_0_fun_sparsity_out(int);
int floatbot_direct_allocation_cost_y_0_fun_n_in(void);
int floatbot_direct_allocation_cost_y_0_fun_n_out(void);

int floatbot_direct_allocation_cost_y_0_fun_jac_ut_xt(const real_t** arg, real_t** res, int* iw, real_t* w, void *mem);
int floatbot_direct_allocation_cost_y_0_fun_jac_ut_xt_work(int *, int *, int *, int *);
const int *floatbot_direct_allocation_cost_y_0_fun_jac_ut_xt_sparsity_in(int);
const int *floatbot_direct_allocation_cost_y_0_fun_jac_ut_xt_sparsity_out(int);
int floatbot_direct_allocation_cost_y_0_fun_jac_ut_xt_n_in(void);
int floatbot_direct_allocation_cost_y_0_fun_jac_ut_xt_n_out(void);



// Cost at path shooting node

int floatbot_direct_allocation_cost_y_fun(const real_t** arg, real_t** res, int* iw, real_t* w, void *mem);
int floatbot_direct_allocation_cost_y_fun_work(int *, int *, int *, int *);
const int *floatbot_direct_allocation_cost_y_fun_sparsity_in(int);
const int *floatbot_direct_allocation_cost_y_fun_sparsity_out(int);
int floatbot_direct_allocation_cost_y_fun_n_in(void);
int floatbot_direct_allocation_cost_y_fun_n_out(void);

int floatbot_direct_allocation_cost_y_fun_jac_ut_xt(const real_t** arg, real_t** res, int* iw, real_t* w, void *mem);
int floatbot_direct_allocation_cost_y_fun_jac_ut_xt_work(int *, int *, int *, int *);
const int *floatbot_direct_allocation_cost_y_fun_jac_ut_xt_sparsity_in(int);
const int *floatbot_direct_allocation_cost_y_fun_jac_ut_xt_sparsity_out(int);
int floatbot_direct_allocation_cost_y_fun_jac_ut_xt_n_in(void);
int floatbot_direct_allocation_cost_y_fun_jac_ut_xt_n_out(void);



// Cost at terminal shooting node

int floatbot_direct_allocation_cost_y_e_fun(const real_t** arg, real_t** res, int* iw, real_t* w, void *mem);
int floatbot_direct_allocation_cost_y_e_fun_work(int *, int *, int *, int *);
const int *floatbot_direct_allocation_cost_y_e_fun_sparsity_in(int);
const int *floatbot_direct_allocation_cost_y_e_fun_sparsity_out(int);
int floatbot_direct_allocation_cost_y_e_fun_n_in(void);
int floatbot_direct_allocation_cost_y_e_fun_n_out(void);

int floatbot_direct_allocation_cost_y_e_fun_jac_ut_xt(const real_t** arg, real_t** res, int* iw, real_t* w, void *mem);
int floatbot_direct_allocation_cost_y_e_fun_jac_ut_xt_work(int *, int *, int *, int *);
const int *floatbot_direct_allocation_cost_y_e_fun_jac_ut_xt_sparsity_in(int);
const int *floatbot_direct_allocation_cost_y_e_fun_jac_ut_xt_sparsity_out(int);
int floatbot_direct_allocation_cost_y_e_fun_jac_ut_xt_n_in(void);
int floatbot_direct_allocation_cost_y_e_fun_jac_ut_xt_n_out(void);




#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  // floatbot_direct_allocation_COST
