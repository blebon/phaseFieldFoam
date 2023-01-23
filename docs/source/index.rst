.. phaseFieldFoam documentation master file, created by
   sphinx-quickstart on Wed Jan 18 10:19:15 2023.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to phaseFieldFoam's documentation!
==========================================

phaseFieldFoam is an OpenFOAM implementation of a finite volume solver for coupled phase field equations using the phase field model of Warren, Kobayashi, Lobkovsky and Carter (2003) `doi:10.1016/S1359-6454(03)00388-4 <https://doi.org/10.1016/S1359-6454(03)00388-4>`_.

You will find the application source code on `Gitea <https://v-sys-bcast.brunel.ac.uk/BCAST/phaseFieldFoam/src/branch/master/applications/solvers/multiphase/phaseFieldFoam>`_.

A tutorial case for the growth of a crystal with 6 fold symmetry is found `here <https://v-sys-bcast.brunel.ac.uk/BCAST/phaseFieldFoam/src/branch/master/tutorials/multiphase/phaseFieldFoam/phase>`_ and documented on :doc:`tutorials/phase`.

.. toctree::
   :maxdepth: 2
   :caption: Contents:
 
   solver/phaseFieldFoam
   theory/theory
   tutorials/phase
   tutorials/modules


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
