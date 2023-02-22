=================
Anisotropy source
=================

.. contents:: Contents:
  :backlinks: none

Installation
============

Pre-requisites:  

* A working installation of `OpenFOAM 10 <https://openfoam.org/release/10/>`_.

In the phaseFieldFoam/src/fvModels directory, run:

.. code-block:: console
  
  $ wmake libso

C++ Classes
===========

.. doxygenclass:: Foam::fv::anisotropySource
  :members:

Testing
=======

The unit tests for the fvModels libraries require the Boost libraries (>= 1.69.0). The tests are run in the tests/fvModels/case folder:

.. code-block:: console
  
  $ wmake .. # Build the test executable
  $ blockMesh # Create the mesh for the test case
  $ ./test_anisotropySource --log_level=all # Run the tests

