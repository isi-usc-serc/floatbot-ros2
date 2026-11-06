/*
 * Copyright (c) The acados authors.
 *
 * This file is part of acados.
 *
 * Licensed under the 2-Clause BSD License.
 */

#ifndef floatbot_direct_allocation_MODEL
#define floatbot_direct_allocation_MODEL

#ifdef __cplusplus
extern "C" {
#endif


/* explicit ODE */

// explicit ODE
int floatbot_direct_allocation_expl_ode_fun(const real_t** arg, real_t** res, int* iw, real_t* w, void *mem);
int floatbot_direct_allocation_expl_ode_fun_work(int *, int *, int *, int *);
const int *floatbot_direct_allocation_expl_ode_fun_sparsity_in(int);
const int *floatbot_direct_allocation_expl_ode_fun_sparsity_out(int);
int floatbot_direct_allocation_expl_ode_fun_n_in(void);
int floatbot_direct_allocation_expl_ode_fun_n_out(void);

// explicit forward VDE
int floatbot_direct_allocation_expl_vde_forw(const real_t** arg, real_t** res, int* iw, real_t* w, void *mem);
int floatbot_direct_allocation_expl_vde_forw_work(int *, int *, int *, int *);
const int *floatbot_direct_allocation_expl_vde_forw_sparsity_in(int);
const int *floatbot_direct_allocation_expl_vde_forw_sparsity_out(int);
int floatbot_direct_allocation_expl_vde_forw_n_in(void);
int floatbot_direct_allocation_expl_vde_forw_n_out(void);

// explicit param-direction forward VDE


// explicit adjoint VDE
int floatbot_direct_allocation_expl_vde_adj(const real_t** arg, real_t** res, int* iw, real_t* w, void *mem);
int floatbot_direct_allocation_expl_vde_adj_work(int *, int *, int *, int *);
const int *floatbot_direct_allocation_expl_vde_adj_sparsity_in(int);
const int *floatbot_direct_allocation_expl_vde_adj_sparsity_out(int);
int floatbot_direct_allocation_expl_vde_adj_n_in(void);
int floatbot_direct_allocation_expl_vde_adj_n_out(void);



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  // floatbot_direct_allocation_MODEL
