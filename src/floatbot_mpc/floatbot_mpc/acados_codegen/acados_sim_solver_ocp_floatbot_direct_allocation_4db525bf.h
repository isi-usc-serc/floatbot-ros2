/*
 * Copyright (c) The acados authors.
 *
 * This file is part of acados.
 *
 * Licensed under the 2-Clause BSD License.
 */

#ifndef ACADOS_SIM_ocp_floatbot_direct_allocation_4db525bf_H_
#define ACADOS_SIM_ocp_floatbot_direct_allocation_4db525bf_H_

#include "acados_c/sim_interface.h"
#include "acados_c/external_function_interface.h"

#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NX     7
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NZ     0
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NU     8
#define OCP_FLOATBOT_DIRECT_ALLOCATION_4DB525BF_NP     7

#ifdef __cplusplus
extern "C" {
#endif


// ** capsule for solver data **
typedef struct ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule
{
    // acados objects
    sim_in *acados_sim_in;
    sim_out *acados_sim_out;
    sim_solver *acados_sim_solver;
    sim_opts *acados_sim_opts;
    sim_config *acados_sim_config;
    void *acados_sim_dims;
    void *acados_sim_mem;

    /* external functions */
    // ERK
    external_function_param_casadi * sim_expl_vde_forw;
    external_function_param_casadi * sim_vde_adj_casadi;
    external_function_param_casadi * sim_expl_ode_fun_casadi;
    external_function_param_casadi * sim_expl_ode_hess;
    external_function_param_casadi * sim_expl_vde_forw_p;

    // IRK
    external_function_param_casadi * sim_impl_dae_fun;
    external_function_param_casadi * sim_impl_dae_fun_jac_x_xdot_z;
    external_function_param_casadi * sim_impl_dae_jac_x_xdot_u_z;
    external_function_param_casadi * sim_impl_dae_hess;
    external_function_param_casadi * sim_impl_dae_jac_p;

    // GNSF
    external_function_param_casadi * sim_gnsf_phi_fun;
    external_function_param_casadi * sim_gnsf_phi_fun_jac_y;
    external_function_param_casadi * sim_gnsf_phi_jac_y_uhat;
    external_function_param_casadi * sim_gnsf_f_lo_jac_x1_x1dot_u_z;
    external_function_param_casadi * sim_gnsf_get_matrices_fun;

} ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule;


ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_sim_create(ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule *capsule);
ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_sim_solve(ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule *capsule);

ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_sim_free(ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule *capsule);
ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_sim_update_params(ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule *capsule, double *value, int np);

ACADOS_SYMBOL_EXPORT sim_config * ocp_floatbot_direct_allocation_4db525bf_acados_get_sim_config(ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule *capsule);
ACADOS_SYMBOL_EXPORT sim_in * ocp_floatbot_direct_allocation_4db525bf_acados_get_sim_in(ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule *capsule);
ACADOS_SYMBOL_EXPORT sim_out * ocp_floatbot_direct_allocation_4db525bf_acados_get_sim_out(ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule *capsule);
ACADOS_SYMBOL_EXPORT void * ocp_floatbot_direct_allocation_4db525bf_acados_get_sim_dims(ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule *capsule);
ACADOS_SYMBOL_EXPORT sim_opts * ocp_floatbot_direct_allocation_4db525bf_acados_get_sim_opts(ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule *capsule);
ACADOS_SYMBOL_EXPORT sim_solver * ocp_floatbot_direct_allocation_4db525bf_acados_get_sim_solver(ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule *capsule);
ACADOS_SYMBOL_EXPORT void * ocp_floatbot_direct_allocation_4db525bf_acados_get_sim_mem(ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule *capsule);

ACADOS_SYMBOL_EXPORT ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule * ocp_floatbot_direct_allocation_4db525bf_acados_sim_solver_create_capsule(void);
ACADOS_SYMBOL_EXPORT int ocp_floatbot_direct_allocation_4db525bf_acados_sim_solver_free_capsule(ocp_floatbot_direct_allocation_4db525bf_sim_solver_capsule *capsule);

#ifdef __cplusplus
}
#endif

#endif  // ACADOS_SIM_ocp_floatbot_direct_allocation_4db525bf_H_
