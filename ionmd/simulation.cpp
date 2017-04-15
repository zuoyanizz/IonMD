#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
// #include <ctime>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "simulation.hpp"

namespace ionmd {

using arma::vec;
using arma::mat;

namespace logging = boost::log;


Simulation::Simulation() {
    status = SimStatus::IDLE;
}


Simulation::Simulation(SimParams p, Trap trap)
    : Simulation()
{
    this->p = std::make_shared<SimParams>(p);
    this->trap = std::make_shared<Trap>(trap);
}


Simulation::Simulation(SimParams p, Trap trap, std::vector<Ion> ions)
    : Simulation(p, trap)
{
    for (auto ion: ions) {
        this->ions.push_back(ion);
    }
    BOOST_LOG_TRIVIAL(debug) << "Number of ions: " << this->ions.size();
}


mat Simulation::precompute_coulomb() {
    int i = 0;
    mat Flist(3, ions.size());

    #pragma omp parallel for
    for (unsigned int i = 0; i < ions.size(); i++) {
        // FIXME
        // Flist.col(i) = ion.coulomb(ions);
        i++;
    }
    return Flist;
}


void Simulation::set_params(SimParams new_params) {
    if (status != SimStatus::RUNNING) {
        p = std::make_shared<SimParams>(new_params);
    }
    else {
        BOOST_LOG_TRIVIAL(error) << "Can't change parameters while simulation is running!";
    }
}



void Simulation::set_trap(Trap new_trap) {
    if (status != SimStatus::RUNNING) {
        trap = std::make_shared<Trap>(new_trap);
    }
    else {
        BOOST_LOG_TRIVIAL(error) << "Can't set a new trap while simulation is running!";
    }
}


Ion Simulation::make_ion(const double &m, const double &Z,
                         const std::vector<double> &x0)
{
    return Ion(p, trap, m, Z, x0);
}


void Simulation::set_ions(std::vector<Ion> ions) {
    if (status != SimStatus::RUNNING) {
        ions.clear();

        for (auto ion: ions) {
            ions.push_back(ion);
        }
    }
    else {
        BOOST_LOG_TRIVIAL(error) << "Can't set new ions while simulation is running!";
    }
}


void Simulation::run() {
    // Setup logging.
    // TODO: maybe just leave this up to implementation?
    auto severity = p->verbosity > 0 ? logging::trivial::debug : logging::trivial::info;
    logging::core::get()->set_filter(logging::trivial::severity >= severity);

    // Initialize RNG
    // std::mersenne_twister_engine<double> rng;

    // Storage of pre-computed Coulomb force data
    mat coulomb_forces(3, p->num_ions);
    coulomb_forces.zeros();

    // FIXME: Initialization should happen before here
    // TODO: Initialize CCD
    // TODO: Initialize lasers

    // Recording initialization
    // TODO: HDF5, store all data
    // char buff[256];
    // const auto filename = std::string(std::strftime(buff, sizeof(buff), "%F-%T.bin"));
    // auto outfile = std::ostream(filename, std::ios::out | std::ios::binary);

    // Run simulation
    int iteration = 0;
    int t_10 = (int)(p->t_max/p->dt) / 10;
    BOOST_LOG_TRIVIAL(info) << "Simulating...";
    status = SimStatus::RUNNING;

    for (double t = 0; t < p->t_max; t += p->dt) {
        // Calculate Coulomb forces
        if (p->coulomb_enabled) {
            coulomb_forces = precompute_coulomb();
        }

        // Progress update
        if (iteration % t_10 == 0 && p->verbosity != 0) {
            BOOST_LOG_TRIVIAL(info) << int(10*iteration/t_10) << "% complete; "
                                    << "t = " << t;
        }

        // Update each ion
        // TODO: update to use Boost.compute
        #pragma omp parallel for
        for (unsigned int i = 0; i < ions.size(); i++) {
            auto ion = &ions[i];
            ion->update(t, coulomb_forces);

            // TODO: Record data
            // TODO: Check bounds
        }

        iteration++;
    }

    status = SimStatus::FINISHED;
}

}  // namespace ionmd
