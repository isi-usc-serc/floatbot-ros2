import os
from floatbot_mpc.models.direct_allocation_model import DirectAllocationModel
from floatbot_mpc.controllers.direct_allocation_mpc import DirectAllocationMPC
from acados_solver_plugins import SolverPluginGenerator

# generate Acados model
model = DirectAllocationModel()
mpc = DirectAllocationMPC(model.setup())

# define class
plugin_class_name = 'FloatbotDirectAllocationMpc'
solver_description = 'Acados solver plugin for floatbot MPC with direct thrust allocation'

# define semantic map of OCP variables
x_index_map = {
    'r': [0, 1],
    'q': [2, 3],
    'v': [4, 5],
    'w': [6]
}
z_index_map = {}
p_index_map = {
    'r_cmd': [0, 1],
    'q_cmd': [2, 3],
    'v_cmd': [4, 5],
    'w_cmd': [6]
}
u_index_map = {
'u': [0, 1, 2, 3, 4, 5, 6, 7]
}

# generate plugin
solver_plugin_generator = SolverPluginGenerator()
solver_plugin_generator.generate_solver_plugin(
    mpc.ocp,
    plugin_class_name=plugin_class_name,
    solver_description=solver_description,
    x_index_map=x_index_map,
    z_index_map=z_index_map,
    p_index_map=p_index_map,
    u_index_map=u_index_map
)