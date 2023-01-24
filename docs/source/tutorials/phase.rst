================================
Tutorial: Growth of a 2D crystal
================================

This tutorial describes how to pre-process, run and post-process a simple crystal growth case with phaseFieldFoam.

Pre-processing
==============

Change to the case directory.

.. code-block:: console

  $ cd phaseFieldFoam/tutorials/multiphase/phaseFieldFoam/phase

Mesh generation
---------------

Generate the mesh in the case directory.

.. code-block:: console

  $ blockMesh

Boundary and initial conditions
-------------------------------

The boundary conditions are in the 0 folder.

.. code-block:: console

  $ less 0/fi.orig
  $ less 0/dT

Initialize the fields.

.. code-block:: console

  $ setFields

Physical properties
-------------------

The `physical properties <../solver/phaseFieldFoam.html#nomenclature>`_ are prescribed in the constant/physicalProperties dictionary file.

Example usage:

.. code-block:: C 

  DT              DT     [0 2 -1 0 0 0 0] 2.25;
  tau             tau    [0 0  1 0 0 0 0] 3e-04;
  alpha           alpha  [0 1  0 0 0 0 0] 0.015;
  c               c      [0 0  0 0 0 0 0] 0.02;
  N               N      [0 0  0 0 0 0 0] 6;
  kappa1          kappa1 [0 0  0 0 0 0 0] 0.9;
  kappa2          kappa2 [0 0  0 0 0 0 0] 20;

Run the application
===================

In the case directory, run:

.. code-block:: console

  $ phaseFieldFoam

Post-processing
===============

Contour plots
-------------

The sump profile can be plotted from the VTK files that are saved in the 
postProcessing directory.

.. Figure:: ../images/Crystal.png
  :width: 80%
  :alt: Crystal growth after 0.5 s

  Predicted crystal contour and under-cooling after 0.5 s of growth.

Benchmarking
------------

To check if the numerical predictions are identical to the benchmark, run the following commands in the case directory:

.. code-block:: console

  $ mkdir -p test/build
  $ cd test/build
  $ cmake ..
  $ make
  $ BOOST_TEST_LOG_LEVEL=message ./test_phasefield