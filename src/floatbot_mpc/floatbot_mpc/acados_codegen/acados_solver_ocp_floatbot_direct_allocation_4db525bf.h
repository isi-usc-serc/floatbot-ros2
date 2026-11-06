/*
 * Copyright (c) The acados authors.
 *
 * This file is part of acados.
 *
 * Licensed under the 2-Clause BSD License.
 */

#ifndef ACADOS_SOLVER_ocp_floatbot_direct_allocation_4db525bf_H_
#define ACADOS_SOLVER_ocp_floatbot_direct_allocation_4db525bf_H_

#include "acados/utils/types.h"

#include "acados_c/ocp_nlp_interface.h"
#include "acados_c/external_function_interface.h"

#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NX     7
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NZ     0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NU     8
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NP     7
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NP_GLOBAL     0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NBX    0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NBX0   7
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NBU    8
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NSBX   0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NSBU   0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NSH    0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NSH0   0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NSG    0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NSPHI  0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NSHN   0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NSGN   0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NSPHIN 0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NSPHI0 0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NSBXN  0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NS     0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NS0    0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NSN    0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NG     0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NBXN   0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NGN    0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NY0    14
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NY     14
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NYN    6
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_N      20
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NH     0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NHN    0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NH0    0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NPHI0  0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NPHI   0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NPHIN  0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NR     0

#ifdef __cplusplus
extern "C" {
#endif


// ** capsule for solver data **
typedef struct ocp_floatbot_direct_allocation_4db525bf_solver_capsule
{
    // acados objects
    ocp_nlp_in *nlp_in;
    ocp_nlp_out *nlp_out;
    ocp_nlp_out *sens_out;
    ocp_nlp_solver *nlp_solver;
    void *nlp_opts;
    ocp_nlp_plan_t *nlp_solver_plan;
    ocp_nlp_config *nlp_config;
    ocp_nlp_dims *nlp_dims;

    // number of expected runtime parameters
    unsigned int nlp_np;

    /* external functions */

    // dynamics

    external_function_external_param_casadi *expl_vde_forw;
    external_function_external_param_casadi *expl_vde_forw_p;
    external_function_external_param_casadi *expl_ode_fun;
    external_function_external_param_casadi *expl_vde_adj;




    // cost

    external_function_external_param_casadi *cost_y_fun;
    external_function_external_param_casadi *cost_y_fun_jac_ut_xt;



    external_function_external_param_casadi cost_y_0_fun;
    external_function_external_param_casadi cost_y_0_fun_jac_ut_xt;



    external_function_external_param_casadi cost_y_e_fun;
    external_function_external_param_casadi cost_y_e_fun_jac_ut_xt;


    // constraints







} ocp_floatbot_direct_allocation_4db525bf_solver_capsule;

ACADOS_SYMBOL_EXPORT ocp_floatbot_direct_allocation_4db525bf_solver_capsule * ocp_floatbot_direct_allocation_4db525bf_acados_create_capsule(void);
ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_free_capsule(ocp_floatbot_direct_allocation_4db525bf_solver_capsule *capsule);

ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_create(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule);

ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_reset(ocp_floatbot_direct_allocation_4db525bf_solver_capsule* capsule, int reset_qp_solver_mem, int reset_numerical_values, int reset_solver_options, int reset_x_to_x0_bar);

/**
 * Generic version of ocp_floatbot_direct_allocation_4db525bf_acados_create which allows to use a different number of shooting intervals than
 * the number used for code generation. If new_time_steps=NULL and n_time_steps matches the number used for code
 * generation, the time-steps from code generation is used.
 */
ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_create_with_discretization(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule, int n_time_steps, double* new_time_steps);
/**
 * Update the time step vector. Number N must be identical to the currently set number of shooting nodes in the
 * nlp_solver_plan. Returns 0 if no error occurred and a otherwise a value other than 0.
 */
ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_update_time_steps(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule, int N, double* new_time_steps);
/**
 * This function is used for updating an already initialized solver with a different number of qp_cond_N.
 */
ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_update_qp_solver_cond_N(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule, int qp_solver_cond_N);
ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_update_params(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule, int stage, double *value, int np);
ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_update_params_sparse(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule, int stage, int *idx, double *p, int n_update);
ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_set_p_global_and_precompute_dependencies(ocp_floatbot_direct_allocation_4db525bf_solver_capsule* capsule, double* data, int data_len);

ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_solve(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule);
ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_setup_qp_matrices_and_factorize(ocp_floatbot_direct_allocation_4db525bf_solver_capsule* capsule);



ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_free(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule);
ACADOS_SYMBOL_EXPORT void ocp_floatbot_direct_allocation_4db525bf_acados_print_stats(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule);
ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_custom_update(ocp_floatbot_direct_allocation_4db525bf_solver_capsule* capsule, double* data, int data_len);

ACADOS_SYMBOL_EXPORT ocp_nlp_in *ocp_floatbot_direct_allocation_4db525bf_acados_get_nlp_in(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule);
ACADOS_SYMBOL_EXPORT ocp_nlp_out *ocp_floatbot_direct_allocation_4db525bf_acados_get_nlp_out(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule);
ACADOS_SYMBOL_EXPORT ocp_nlp_out *ocp_floatbot_direct_allocation_4db525bf_acados_get_sens_out(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule);
ACADOS_SYMBOL_EXPORT ocp_nlp_solver *ocp_floatbot_direct_allocation_4db525bf_acados_get_nlp_solver(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule);
ACADOS_SYMBOL_EXPORT ocp_nlp_config *ocp_floatbot_direct_allocation_4db525bf_acados_get_nlp_config(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule);
ACADOS_SYMBOL_EXPORT void *ocp_floatbot_direct_allocation_4db525bf_acados_get_nlp_opts(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule);
ACADOS_SYMBOL_EXPORT ocp_nlp_dims *ocp_floatbot_direct_allocation_4db525bf_acados_get_nlp_dims(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule);
ACADOS_SYMBOL_EXPORT ocp_nlp_plan_t *ocp_floatbot_direct_allocation_4db525bf_acados_get_nlp_plan(ocp_floatbot_direct_allocation_4db525bf_solver_capsule * capsule);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  // ACADOS_SOLVER_ocp_floatbot_direct_allocation_4db525bf_H_
