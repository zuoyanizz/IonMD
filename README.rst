=====
IonMD
=====

Overview
========

IonMD is a molecular dynamics (MD) simulation of ions in a linear Paul
trap. The implmentation uses the leapfrog integration technique and
Newtonian equations of motion to clasically simulate the forces on
individual ions. Methods are largely based on the MD simulation
described in [1]_ and [2]_.

Model Description
-----------------

IonMD simulates ions in a linear Paul trap by solving each ion's
classical equation of motion :math:`\ddot{\vec{x}}_i = \vec{F}_i/m_i`
where the force on each ion is the sum of forces due to the trapping
potential, cooling lasers, Coulomb repulsion, external secular
excitation and stochastic processes such as background gas
collisions. In other words,

.. math::
   
   \vec{F}_i = \vec{F}_i^{(trap)} + \vec{F}_i^{(laser)} + \vec{F}_i^{(Coulomb)} + \vec{F}_i^{(secular)} + \vec{F}_i^{(stochastic)}.

License
=======

IonMD is freely distributable under the terms of the GNU GPL version 3
(see LICENSE for details).

System Requirements
===================

IonMD uses Python as a frontend for accessing compiled C++
routines. IonMD is built and tested on Linux (Debian Wheezy), but
should work on any platform that can meet the following dependencies:

* Python requirements
  - Python 2.6 or 2.7
  - Numpy
  - Scipy
  - MayaVI (optional; for displaying ions in 3D)
  - Matplotlib (optional; for plotting/simulating CCD images)
  - PIL (optional; for simulating CCD images)
* C++ requirements
  - `GNU Scientific Library <https://www.gnu.org/software/gsl/>`_ (for
    random number generation and 2D binning for simulated CCD images)

Usage
=====

The simulation at present is run through the runsim.py
file. Simulation parameters may be entered in the main() function, or
any other valid Python code can be written by the user to interface
with the C++ library.

If modifying the C++ code, the included Makefile should be sufficient
if using a standard Linux environment with g++.

Data Output
===========

Large data files containing, e.g., trajectories are written in low
level binary format. This means that they may not be portable to
another computer, but within a machine, there should be no
issues. Data can be easily read with numpy.fromfile. See the simCCD
function in runsim.py for an example.

Simulated CCD data gives mass in amu and position data in
microns. Trajectory data gives time in microseconds and position in
mm. In both cases, floats are used rather than the internal doubles in
order to reduce file size (precision should not be an issue in this
case). A "final positions" file gives x y z positions of all ions at
the end of the simulation in the plaintext xyz format (a common
chemical file format).

Future Features
===============

Some things that are planned for future releases:

* Optimization!
* Better stability -> less random initial conditions could help this
* A simple GUI for running simulations

Authors
=======

IonMD is principally written by Michael V. DePalatis <mvd@gatech.edu>
with some optimization enhancements by Ben Land.

References
==========

.. [1] C.B. Zhang *et al.*, Phys. Rev. A **76**, 012719 (2007).
.. [2] C.B. Zhang, *Production and Sympathetic Cooling of Complex
       Molecular Ions*, PhD thesis, Heinrich-Heine-Universität
       Düsseldorf (2008).