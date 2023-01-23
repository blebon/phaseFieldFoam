#!/usr/bin/env python3
"""Tests for application."""

from os import sep
from shutil import which


def test_phaseFieldFoam(_app_name="phaseFieldFoam"):
    """Test that `phaseFieldFoam` is in PATH."""
    assert which(_app_name).split(sep)[-1] == _app_name
