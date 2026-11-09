import casadi as cs

def quat2rot(q):
    '''Z axis rotation matrix from 2D quaternion'''
    return cs.vertcat(
        cs.horzcat(q[0]**2-q[1]**2, -2*q[0]*q[1]),
        cs.horzcat(2*q[0]*q[1], q[0]**2+q[1]**2)
    )

def q_inv(q):
    '''Inverse (conjugate) of 2D quaternion'''
    return cs.vertcat(q[0], -q[1])

def q_mult(p, q):
    '''2D version of the Hamilton product of quaternions'''
    return cs.vertcat(
        p[0]*q[0] - p[1]*q[1],
        p[0]*q[1] + p[1]*q[0]
    )

def skew(v):
    '''2D version of the cross-product operator'''
    return cs.vertcat(v[1], -v[0])