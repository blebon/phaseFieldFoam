Phase field equations solver
============================

phaseFieldFoam is based on `laplacianFoam <https://github.com/OpenFOAM/OpenFOAM-10/tree/master/applications/solvers/basic/laplacianFoam>`_, a simple Laplace equation solver. phaseFieldFoam solves the phase field model of Warren, Kobayashi, Lobkovsky and Carter (2003) `doi:10.1016/S1359-6454(03)00388-4 <https://doi.org/10.1016/S1359-6454(03)00388-4>`_ that includes anisotropy.

Definition in file phaseFieldFoam.C

Installation
------------

Pre-requisites:  

* A working installation of `OpenFOAM 10 <https://openfoam.org/release/10/>`_.

In the phaseFieldFoam/applications/solver/multiphase/phaseFieldFoam directory, run:    

.. code-block:: console
  
  $ wmake

Running the application
-----------------------

In the case directory, run:

.. code-block:: console
  
  $ phaseFieldFoam

Nomenclature
------------

.. table:: Variables used in phase field simulations.
  :widths: auto

  +----------+------------------------------------------+
  | Variable | Description                              |
  +==========+==========================================+
  | DT       | Heat diffusion coefficient               |
  +----------+------------------------------------------+
  | tau      | Phase field transient coefficient        |
  +----------+------------------------------------------+
  | alpha    | Anisotropic diffusion tensor coefficient |
  +----------+------------------------------------------+
  | c        | Anisotropic diffusion tensor coefficient |
  +----------+------------------------------------------+
  | N        | Crystal symmetry                         |
  +----------+------------------------------------------+
  | kappa1   | Anisotropy coefficient                   |
  +----------+------------------------------------------+
  | kappa2   | Anisotropy coefficient                   |
  +----------+------------------------------------------+
  