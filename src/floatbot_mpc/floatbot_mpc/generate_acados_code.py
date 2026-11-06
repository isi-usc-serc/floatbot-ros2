from models.direct_allocation_model import DirectAllocationModel
from controllers.direct_allocation_mpc import DirectAllocationMPC

# set up model
model = DirectAllocationModel()

# set up ocp and generate c code
mpc = DirectAllocationMPC(model)
mpc.build()