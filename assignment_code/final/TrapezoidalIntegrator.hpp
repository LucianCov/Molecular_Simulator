#ifndef TRAPEZOIDAL_INTEGRATOR_H_
#define TRAPEZOIDAL_INTEGRATOR_H_

#include "IntegratorBase.hpp"

#include <iostream>

namespace GLOO {
template <class TSystem, class TState>
class TrapezoidalIntegrator : public IntegratorBase<TSystem, TState> {
  TState Integrate(const TSystem& system,
                   const TState& state,
                   float start_time,
                   float dt) const override {
    // TODO: Here we are returning the state at time t (which is NOT what we
    // want). Please replace the line below by the state at time t + dt using
    // forward Euler integration.
    // euler is x + f(X)*dt
    // std::cout << "euler integrator integrating" << std::endl;
    TState deriv1 = system.ComputeTimeDerivative(state, start_time);
    // std::cout << "Derivative successfully calculated" << std::endl;
    
    TState deriv2 = system.ComputeTimeDerivative(state + deriv1 * dt, start_time + dt);

    return state + dt/2*(deriv1 + deriv2);
  }
};
}  // namespace GLOO

#endif
