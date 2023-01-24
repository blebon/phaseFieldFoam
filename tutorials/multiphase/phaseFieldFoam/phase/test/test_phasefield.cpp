#define BOOST_TEST_MODULE CheckVariableValues
#include <boost/test/included/unit_test.hpp>

#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

using namespace std;
namespace utf = boost::unit_test;

struct VariableVectors
{
    VariableVectors(string casename, size_t column = 1, string end_time = "0.5")
        : casename{casename},
          column{column},
          end_time{end_time}
    {
        expected = read_variable("../" + casename + ".xy", column); // Expected in test directory when run in build subdir
        numerical = read_variable("../../postProcessing/" + casename + "/" + end_time + "/line.xy", column); // Numerical result in postProcessing directory
    }

    ~VariableVectors()
    {

    }

    vector<double> read_variable(string filename, size_t column)
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
            number = stod(line_array[column]); // Variable values in column
            // printf("%f %f\n", stod(line_array[1]), number);
            expected.push_back(number);
        }

        return expected;
    }

    public:
    //! Expected variable values
    /*! Vector holding the expected variable values. */
    vector<double> expected;
    //! Predicted variable values
    /*! Vector holding the predicted variable values. */
    vector<double> numerical;

    private:
    //! Case name
    /*! String holding the line along which variable values are compared*/
    string casename;
    //! Column of variable to compare
    /*! 1 = Phase field &phi;, 2 = Undercooling DT. */
    size_t column;
    //! Case end time
    /*! String holding the time at which the simulation ends*/
    string end_time;
};

struct F {
  F() { BOOST_TEST_MESSAGE( "Setup fixture" ); }
  ~F() { BOOST_TEST_MESSAGE( "Teardown fixture" ); }
};

BOOST_FIXTURE_TEST_SUITE(CheckIfVariableValuesMatchExpectedValues, F);

    BOOST_AUTO_TEST_CASE(CheckIfCentrelineCoordinatesMatch)
    {
        BOOST_TEST_MESSAGE("Requiring coordinates to be equal");
        VariableVectors centreline{ "centreline", 1 };
        BOOST_REQUIRE_EQUAL_COLLECTIONS(centreline.expected.begin(), centreline.expected.end(),
                                        centreline.numerical.begin(), centreline.numerical.end());
    }
    
    BOOST_AUTO_TEST_CASE(CheckIfCentrelinePhaseFieldValuesMatchExpectedValues,
        * utf::depends_on("CheckIfVariableValuesMatchExpectedValues/CheckIfCentrelineCoordinatesMatch"))
    {
        BOOST_TEST_MESSAGE("Checking centreline phase field values");
        VariableVectors centreline{ "centreline", 2 };
        BOOST_CHECK_EQUAL_COLLECTIONS(centreline.expected.begin(), centreline.expected.end(),
                                      centreline.numerical.begin(), centreline.numerical.end());
    }

    BOOST_AUTO_TEST_CASE(CheckIfCentrelineUndercoolingValuesMatchExpectedValues,
        * utf::depends_on("CheckIfVariableValuesMatchExpectedValues/CheckIfCentrelineCoordinatesMatch"))
    {
        BOOST_TEST_MESSAGE("Checking centreline undercooling values");
        VariableVectors centreline{ "centreline", 3 };
        BOOST_CHECK_EQUAL_COLLECTIONS(centreline.expected.begin(), centreline.expected.end(),
                                      centreline.numerical.begin(), centreline.numerical.end());
    }

BOOST_AUTO_TEST_SUITE_END();