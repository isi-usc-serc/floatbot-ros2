import numpy as np
import casadi as cs
from models import utils
from acados_template import AcadosModel

class DirectAllocationModel():

    def __init__(self):
        self.name = 'floatbot_direct_allocation'

        # mass properties
        self.m = 17.8 # total mass (kg)
        self.J = .315 # total inertia (kg*m^2)
        self.cg = np.array([0, 0]) # cg location (m)
        self.radius = 0.12 # thruster radius (m)

        # thruster matrix
        F = np.array([
            [1, -1, 0, 0, -1, 1, 0, 0],
            [0, 0, 1, -1, 0, 0, -1, 1]
        ])
        T = self.radius*np.array([
            [-1, 1, -1, 1, -1, 1, -1, 1]
        ])
        self.B = np.vstack([
            F,
            T - self.cg[0]*F[1,:] - self.cg[1]*F[0,:]
        ])

    def get_acados_model(self):
        model = AcadosModel()
        model.name = self.name

        # states
        r = cs.MX.sym('r', 2) # position (m)
        q = cs.MX.sym('q', 2) # quaternion orientation
        v = cs.MX.sym('v', 2) # velocity (m)
        w = cs.MX.sym('w', 1) # angular velocity (rad/s)
        x = cs.vertcat(r, q, v, w)

        # state derivatives
        r_dot = cs.MX.sym('r_dot', 2)
        q_dot = cs.MX.sym('q_dot', 2)
        v_dot = cs.MX.sym('v_dot', 2)
        w_dot = cs.MX.sym('w_dot', 1)
        x_dot = cs.vertcat(r_dot, q_dot, v_dot, w_dot)

        # state commands
        r_cmd = cs.MX.sym('r_cmd', 2)
        q_cmd = cs.MX.sym('q_cmd', 2)
        v_cmd = cs.MX.sym('v_cmd', 2)
        w_cmd = cs.MX.sym('w_cmd', 1)
        x_cmd = cs.vertcat(r_cmd, q_cmd, v_cmd, w_cmd)

        # inputs
        u = cs.MX.sym('u', 8)

        # dynamics
        q_norm = q / cs.norm_2(q)
        R = utils.quat2rot(q)
        cg_X = utils.skew(self.cg)
        M = cs.vertcat(
            cs.horzcat(self.m*cs.DM.eye(2), -self.m*R@cg_X),
            cs.horzcat(-self.m*cg_X.T@R.T, self.J)
        )
        C = cs.vertcat(
            -self.m*R@self.cg*w**2,
            0
        )
        accel = cs.inv(M)@(cs.diagcat(R, cs.MX(1))@self.B@u - C)

        # set state derivative function
        f = cs.vertcat(
            v,
            .5*utils.q_mult(q_norm, cs.vertcat(0, w)),
            accel
        )

        # state error
        r_e = r - r_cmd
        q_e = utils.q_mult(utils.q_inv(q_cmd), q)
        v_e = v - v_cmd
        w_e = w - w_cmd
        x_e = cs.vertcat(r_e, q_e[1], v_e, w_e)

        # define model
        model.f_expl_expr = f # explicit dynamics equation
        model.f_impl_expr = x_dot - f # implicit dynamics equation
        model.x = x # state
        model.xdot = x_dot # state derivative
        model.u = u # inputs
        model.p = x_cmd # parameters
        model.cost_y_expr_0 = cs.vertcat(x_e, u) # initial cost
        model.cost_y_expr = cs.vertcat(x_e, u) # stage cost
        model.cost_y_expr_e = x_e # terminal cost
        return model