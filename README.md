# phaseFieldFoam

## About OpenFOAM

  OpenFOAM is a free, open source computational fluid dynamics (CFD) software
  package released by the OpenFOAM Foundation. It has a large user base across
  most areas of engineering and science, from both commercial and academic
  organisations. OpenFOAM has an extensive range of features to solve anything
  from complex fluid flows involving chemical reactions, turbulence and heat
  transfer, to solid dynamics and electromagnetics.

## Copyright

  OpenFOAM is free software: you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation, either version 3 of the License, or (at your option) any later
  version.  See the file [LICENSE](LICENSE) in this directory or
  [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/), for a description of the GNU General Public
  License terms under which you can copy the files.

## Phase Field Solver Application

  This repository contains developments for solving coupled phase field equations by BCAST, Brunel University London using OpenFOAM.

  This work includes a solver based on the model of:

  James A. Warren, Ryo Kobayashi, Alexander E. Lobkovsky and W. Craig Carter,
  "Extending phase field models of solidification to polycrystalline materials", Acta Materialia **51**(20), 2003, 6035-6058, [doi:10.1016/S1359-6454(03)00388-4](https://doi.org/10.1016/S1359-6454(03)00388-4).

  as well as a tutorial case from:
  
  NIST, "FiPy: A Finite Volume PDE Solver Using Python: examples.phase.anisotropy". [https://www.ctcms.nist.gov/fipy/examples/phase/generated/examples.phase.anisotropy.html](https://www.ctcms.nist.gov/fipy/examples/phase/generated/examples.phase.anisotropy.html).

* [Application](applications/solvers/multiphase/phaseFieldFoam) - Phase field equations solver
* [Tutorial case](tutorials/multiphase/phaseFieldFoam/phase) - Tutorial case corresponding to the above reference
* [Documentation](https://blebon.com/phaseFieldFoam/) - Documentation
