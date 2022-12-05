#ifndef RK4_INTEGRATOR_H_
#define RK4_INTEGRATOR_H_

#include "IntegratorBase.hpp"

#include <iostream>

namespace GLOO {
template <class TSystem, class TState>
class RK4Integrator : public IntegratorBase<TSystem, TState> {
  TState Integrate(const TSystem& system,
                   const TState& state,
                   float start_time,
                   float dt) const override {
    // TODO: Here we are returning the state at time t (which is NOT what we
    // want). Please replace the line below by the state at time t + dt using
    // forward Euler integration.
    // euler is x + f(X)*dt
    // std::cout << "euler integrator integrating" << std::endl;
    TState k1 = system.ComputeTimeDerivative(state, start_time);
    TState k2 = system.ComputeTimeDerivative(state + k1 * (dt/2.f), start_time + dt/2.f);
    TState k3 = system.ComputeTimeDerivative(state + k2 * (dt/2.f), start_time + dt/2.f);
    TState k4 = system.ComputeTimeDerivative(state + k3 * dt, start_time + dt);

    TState combination = k1 + 2.f*k2 + 2.f*k3 + k4;

    return state + (combination * (dt/6.f));
  }
};
}  // namespace GLOO

#endif
