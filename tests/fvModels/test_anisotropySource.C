/*---------------------------------------------------------------------------*\
Application
    test_anisotropySource

Description
    Unit testing for anisotropySource.

Author
    Bruno Lebon

\*---------------------------------------------------------------------------*/

#define BOOST_TEST_MODULE Check_anisotropySource
#include <boost/test/included/unit_test.hpp>

#include "fvCFD.H"
#include "fvModels.H"
#include "fvModel.H"

namespace utf = boost::unit_test;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

struct F
{
    F()
        : argc(utf::framework::master_test_suite().argc),
          argv(utf::framework::master_test_suite().argv)
    {
        BOOST_TEST_MESSAGE("\nStarting anisotropySource tests\n");
    }

    ~F()
    {
        Info << "\nEnd\n" << endl;
    }

    public:
    int argc;
    char **argv;
};

BOOST_FIXTURE_TEST_SUITE(CheckAnisotropySourceFvModel, F);

    BOOST_AUTO_TEST_CASE(CheckIfAnisotropySourceFvModelHasBeenRead)
    {       
        #include "setRootCaseLists.H"

        #include "createTime.H"
        #include "createMesh.H"
        
        #include "createFields.H"
        
        BOOST_TEST_MESSAGE("-- Checking if a anisotropySource dictionary entry has been read");
        BOOST_WARN_EQUAL(fvModels.PtrListDictionary<fvModel>::size(), 1);

        PtrListDictionary<fvModel>& modelsList(fvModels);

        BOOST_TEST_MESSAGE("-- Checking if a anisotropySource is applied to fi");
        BOOST_ASSERT(modelsList[0].addsSupToField("fi"));

        BOOST_TEST_MESSAGE("-- Checking if anisotropy anisotropySource dictionary has been read");
        BOOST_REQUIRE_EQUAL(modelsList[0].name(), "anisotropy");

        // BOOST_REQUIRE_CLOSE(modelsList[0].coeffs().lookup<scalar>("tau"), 0.0003, 0.1);
        BOOST_REQUIRE_CLOSE(modelsList[0].coeffs().lookup<scalar>("kappa1"), 0.9, 0.1);
        BOOST_REQUIRE_CLOSE(modelsList[0].coeffs().lookup<scalar>("kappa2"), 20.0, 0.1);        
    }

BOOST_AUTO_TEST_SUITE_END();

// ************************************************************************* //
