# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

from datetime import datetime

# -- Project information -----------------------------------------------------

project = 'phaseFieldFoam'
year = datetime.now().year
copyright = f"{year}, Bruno Lebon"
author = 'Bruno Lebon'

# The full version, including alpha/beta/rc tags
release = 'OF10.0.1'

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
from os import path, sep
import sys

tutorials_path_list = ["tutorials", "multiphase", project]
tutorials_path = path.abspath(f"..{sep}..{sep}{sep.join(tutorials_path_list)}{sep}")
sys.path.insert(0, tutorials_path)

# Breathe
import subprocess

subprocess.call(f"cd ..; mkdir -p build{sep}xml; doxygen Doxyfile", shell=True)

breathe_projects = {project: f"..{sep}build{sep}xml"}
breathe_default_project = project


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.autosectionlabel",
    "breathe",
    "sphinx.ext.mathjax",
]
autosectionlabel_prefix_document = True

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ["build", "Thumbs.db", ".DS_Store"]


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'alabaster'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

html_theme_options = {
    "github_user": "BCAST",
    "github_repo": "phaseFieldFoam",
    "github_banner": False,
    "github_button": False,
    "sidebar_width": "300px",
}