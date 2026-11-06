import os
import numpy as np
from scipy import linalg
from acados_template import AcadosOcp, AcadosOcpSolver

class DirectAllocationMPC():

    def __init__(self, model):

        # solver parameters
        self.model = model
        self.dt = .2
        self.N = 20
        self.u_max = 1.4

        # cost parameters
        self.Q = np.diag([4e1, 4e1, 4e1, 2e2, 2e2, 6e0])
        self.R = 5e0*np.eye(8)
        self.P = self.Q

        # run setup
        self.ocp = self.setup()

    def setup(self) -> AcadosOcp:
        ocp = AcadosOcp()
        ocp.model = self.model

        # set default parameter values
        ocp.parameter_values = np.array([0, 0, 1, 0, 0, 0, 0])

        # define initial cost function 
        ocp.cost.cost_type_0 = 'NONLINEAR_LS'
        ocp.cost.W_0 = linalg.block_diag(self.Q, self.R)
        ocp.cost.yref_0 = np.zeros(ocp.model.cost_y_expr_0.shape[0])

        # define stage cost function 
        ocp.cost.cost_type = 'NONLINEAR_LS'
        ocp.cost.W = linalg.block_diag(self.Q, self.R)
        ocp.cost.yref = np.zeros(ocp.model.cost_y_expr.shape[0])

        # define initial cost function 
        ocp.cost.cost_type_e = 'NONLINEAR_LS'
        ocp.cost.W_e = self.Q
        ocp.cost.yref_e = np.zeros(ocp.model.cost_y_expr_e.shape[0])

        # set constraints
        ocp.constraints.lbu = np.zeros(8)
        ocp.constraints.ubu = self.u_max*np.ones(8)
        ocp.constraints.idxbu = np.arange(8)
        ocp.constraints.x0 = np.array([0, 0, 1, 0, 0, 0, 0])

        # set solver options
        ocp.solver_options.N_horizon = self.N
        ocp.solver_options.tf = self.N*self.dt
        ocp.solver_options.qp_solver = 'PARTIAL_CONDENSING_HPIPM'
        ocp.solver_options.hessian_approximation = 'GAUSS_NEWTON'
        ocp.solver_options.integrator_type = 'ERK'
        ocp.solver_options.nlp_solver_type = 'SQP_RTI'

        # return solver
        return ocp

    def build(self):

        # set directory for code generation
        file_dir = os.path.dirname(os.path.abspath(__file__))
        package_root = os.path.abspath(os.path.join(file_dir, '..'))
        codegen_dir = os.path.join(package_root, 'acados_codegen')
        json_path = os.path.join(codegen_dir, 'acados_ocp.json')
        os.makedirs(codegen_dir, exist_ok=True)
        self.ocp.code_export_directory = codegen_dir

        # create ocp solver
        self.ocp_solver = AcadosOcpSolver(self.ocp, json_file=json_path)

    def solve(self, x0, x_cmd):

        # set initial state
        self.ocp_solver.set(0, 'lbx', x0.flatten())
        self.ocp_solver.set(0, 'ubx', x0.flatten())

        # set state command
        for i in range(self.ocp_solver.N+1):
            # trajectory commands passed as a 2d array with dimensions 7 by N+1
            if x_cmd.ndim > 1:
                p_i = x_cmd[:,i]
            # constant commands passed as a 1d array with dimension 7
            else:
                p_i = x_cmd
            self.ocp_solver.set(i, 'p', p_i)

        # solve ocp
        status = self.ocp_solver.solve()
        if status != 0:
            raise Exception(f'Acados returned status {status}.')

        # return first element of the optimal state trajectory
        return self.ocp_solver.get(0, 'u')