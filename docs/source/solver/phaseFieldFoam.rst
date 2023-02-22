============================
Phase field equations solver
============================

.. contents:: Contents:
  :backlinks: none

phaseFieldFoam is based on `laplacianFoam <https://github.com/OpenFOAM/OpenFOAM-10/tree/master/applications/solvers/basic/laplacianFoam>`_, a simple Laplace equation solver. phaseFieldFoam solves the phase field model of Warren, Kobayashi, Lobkovsky and Carter (2003) `doi:10.1016/S1359-6454(03)00388-4 <https://doi.org/10.1016/S1359-6454(03)00388-4>`_ that includes anisotropy.

Definition in file phaseFieldFoam.C

Installation
============

wmake
-----

`wmake <https://doc.cfd.direct/openfoam/user-guide-v10/compiling-applications>`_ is OpenFOAM's own compilation script.

Pre-requisites:  

* A working installation of `OpenFOAM 10 <https://openfoam.org/release/10/>`_.
* All the included libraries:

  * :doc:`../src/fvModels`

After cloning the repository, compile wmake applications:

.. code-block:: console

  $ (cd wmake/src && make ) 

In the phaseFieldFoam/applications/solver/multiphase/phaseFieldFoam directory, run:    

.. code-block:: console
  
  $ wmake

CMake
-----

Pre-requisites:  

* A working installation of `OpenFOAM 10 <https://openfoam.org/release/10/>`_.
* `CMake <https://cmake.org/>`_ (>= 3.17.0)

In the repository root directory (containting the main CMakeLists.txt), run:

.. code-block:: console
  
  $ cmake -DCMAKE_BUILD_TYPE=Release -S . -B build # Build everything in a separate directory
  $ cd build            # Enter build directory
  $ make VERBOSE=1      # Compile and link all libraries and phaseFieldFoam, and copy files to OpenFOAM path

To compile phaseFieldFoam in Debug mode, run:

.. code-block:: console
  
  $ cmake -DCMAKE_BUILD_TYPE=Debug -S . -B debug # Build everything in a separate directory
  $ cd debug            # Enter debug directory
  $ make VERBOSE=1      # Compile and link all libraries and phaseFieldFoam, and copy files to OpenFOAM path

Running the application
=======================

In the case directory, run:

.. code-block:: console
  
  $ phaseFieldFoam

Nomenclature
============

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
  
