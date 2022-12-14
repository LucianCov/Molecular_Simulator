#ifndef FORWARD_EULER_INTEGRATOR_H_
#define FORWARD_EULER_INTEGRATOR_H_

#include "IntegratorBase.hpp"

#include <iostream>

namespace GLOO {
template <class TSystem, class TState>
class ForwardEulerIntegrator : public IntegratorBase<TSystem, TState> {
  TState Integrate(const TSystem& system,
                   const TState& state,
                   float start_time,
                   float dt) const override {
    // TODO: Here we are returning the state at time t (which is NOT what we
    // want). Please replace the line below by the state at time t + dt using
    // forward Euler integration.
    // euler is x + f(X)*dt
    // std::cout << "euler integrator integrating" << std::endl;
    TState derivative1 = TState(system.ComputeTimeDerivative(state, start_time));
    // std::cout << "Derivative successfully calculated" << std::endl;

    TState output;
    output.velocities = state.velocities;
    output.positions = state.positions;
    for (int i = 0; i < state.positions.size(); i++) {
        output.positions[i] = state.positions[i] + state.velocities[i]*(dt) + (0.5f)*derivative1.velocities[i] * (dt*dt);
    }

    TState derivative2 = TState(system.ComputeTimeDerivative(output, start_time + dt));
    for (int i = 0; i < state.positions.size(); i++) {
        output.velocities[i] = state.velocities[i] + (0.5f)*(derivative1.velocities[i] + derivative2.velocities[i])*(dt);
    }
    return output;
  }
};
}  // namespace GLOO

#endif
