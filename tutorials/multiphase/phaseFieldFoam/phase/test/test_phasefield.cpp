#define BOOST_TEST_MODULE Checkphase field values
#include <boost/test/included/unit_test.hpp>

#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

using namespace std;

struct PhaseFieldVectors
{
    PhaseFieldVectors(string casename, string end_time = "0.5")
        : casename{casename},
          end_time{end_time}
    {
        expected = read_phasefield("../" + casename + ".xy"); // Expected in test directory when run in build subdir
        numerical = read_phasefield("../../postProcessing/" + casename + "/" + end_time + "/line.xy"); // Numerical result in postProcessing directory
    }

    ~PhaseFieldVectors()
    {

    }

    vector<double> read_phasefield(string filename)
    {
        ifstream datafile(filename);
        vector<double> expected;
        double number;
        vector<string> line_array;
        string line;

        while(getline(datafile, line))
        {
            line_array.clear();
            boost::split(line_array, line, boost::is_any_of("\t "), boost::token_compress_on);
            if ( strcmp(line_array[0].c_str(), "#") == 0 ) continue; // Skip headers and comments
            number = stod(line_array[1]); // Phase field values are in the second column
            expected.push_back(number);
        }

        return expected;
    }

    public:
    //! Expected phase field values
    /*! Vector holding the expected phase field values. */
    vector<double> expected;
    //! Predicted phase field values
    /*! Vector holding the predicted phase field values. */
    vector<double> numerical;

    private:
    //! Case name
    /*! String holding the line along which phase field values are compared*/
    string casename;
    //! Case end time
    /*! String holding the time at which the simulation ends*/
    string end_time;
};

struct F {
  F() { BOOST_TEST_MESSAGE( "Setup fixture" ); }
  ~F() { BOOST_TEST_MESSAGE( "Teardown fixture" ); }
};

BOOST_FIXTURE_TEST_SUITE(CheckIfphasefieldvaluesMatchExpectedValues, F);

    BOOST_AUTO_TEST_CASE(CheckIfCentrelinephasefieldvaluesMatchExpectedValues)
    {
        BOOST_TEST_MESSAGE("Checking centreline phase field values");
        PhaseFieldVectors centreline{ "centreline" };
        BOOST_CHECK_EQUAL_COLLECTIONS(centreline.expected.begin(), centreline.expected.end(),
                                      centreline.numerical.begin(), centreline.numerical.end());
    }

BOOST_AUTO_TEST_SUITE_END();