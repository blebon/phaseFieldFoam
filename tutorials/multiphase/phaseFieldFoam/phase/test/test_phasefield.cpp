#define BOOST_TEST_MODULE CheckVariableValues
#include <boost/test/included/unit_test.hpp>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

using namespace std;
namespace fs = std::filesystem;
namespace utf = boost::unit_test;

struct VariableVectors
{
    VariableVectors(
        string casename,
        size_t column = 1, 
        string end_time = "0.5",
        double large_tolerance = 3.0,
        double small_tolerance = 40.0
        )
        : casename{casename},
          column{column},
          end_time{end_time},
          large_tolerance{large_tolerance},
          small_tolerance{small_tolerance}
    {
        string expected_file{"../" + casename + ".xy"}; // Expected in test directory when run in build subdir
        fs::path expected_path{expected_file};
        if (!fs::exists(expected_path))
            expected_file = casename + ".xy";
        expected = read_variable(expected_file, column); // Try current directory if not found

        string numerical_file{"../../postProcessing/" + casename + "/" + end_time + "/line.xy"};
        fs::path numerical_path{ numerical_file };
        if (!fs::exists(numerical_path))
            numerical_file = "../postProcessing/" + casename + "/" + end_time + "/line.xy";
        numerical = read_variable(numerical_file, column); // Numerical result in postProcessing directory

        expected_path = fs::path(expected_file);
        numerical_path = fs::path(numerical_file);
        BOOST_ASSERT_MSG(fs::exists(expected_path), "-- Benchmark file not found!");
        BOOST_ASSERT_MSG(fs::exists(numerical_path), "-- Prediction file not found!");
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

    // Check if predicted temperatures agree with benchmark
    void boost_check()
    {
        BOOST_TEST_MESSAGE("Checking " + casename + " temperatures");
        double tolerance{0.0};
        for (auto e = expected.begin(), n= numerical.begin();
                  e != expected.end() && n != numerical.end(); 
                  ++e, ++n)
        {
            tolerance = (*e > 0.1) ? large_tolerance : small_tolerance;
            BOOST_CHECK_CLOSE(*e, *n, tolerance);
        };
        // BOOST_CHECK_CLOSE(expected.begin(), numerical.begin(), 1e-6);
        // BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(),
        //                               numerical.begin(), numerical.end(), tolerance);
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
    //! Comparison tolerance
    /*! Double containing the tolerance for large floating point comparison*/
    double large_tolerance;
    //! Comparison tolerance
    /*! Double containing the tolerance for small floating point comparison*/
    double small_tolerance;
};

struct F {
  F() { BOOST_TEST_MESSAGE( "Setup fixture" ); }
  ~F() { BOOST_TEST_MESSAGE( "Teardown fixture" ); }
};

BOOST_FIXTURE_TEST_SUITE(CheckIfVariableValuesMatchExpectedValues, F);

    BOOST_AUTO_TEST_CASE(CheckIfCentrelineCoordinatesMatch)
    {
        string casename{"centreline"};
        VariableVectors centreline{ casename, 1 };
        BOOST_TEST_MESSAGE("Looking for benchmark file.");
        BOOST_ASSERT(!centreline.expected.empty());
        BOOST_TEST_MESSAGE("Looking for numerical predictions file.");
        BOOST_ASSERT(!centreline.numerical.empty());
        BOOST_TEST_MESSAGE("Requiring coordinates to be equal");
        BOOST_REQUIRE_EQUAL_COLLECTIONS(centreline.expected.begin(), centreline.expected.end(),
                                        centreline.numerical.begin(), centreline.numerical.end());
    }
    
    BOOST_AUTO_TEST_CASE(CheckIfCentrelinePhaseFieldValuesMatchExpectedValues,
        * utf::depends_on("CheckIfVariableValuesMatchExpectedValues/CheckIfCentrelineCoordinatesMatch"))
    {
        string casename{"centreline"};
        VariableVectors centreline{ casename, 2, "0.5", 12.0, 50.0};
        BOOST_TEST_MESSAGE("Checking centreline phase field values");
        centreline.boost_check();
    }

    BOOST_AUTO_TEST_CASE(CheckIfCentrelineUndercoolingValuesMatchExpectedValues,
        * utf::depends_on("CheckIfVariableValuesMatchExpectedValues/CheckIfCentrelineCoordinatesMatch"))
    {
        string casename{"centreline"};
        VariableVectors centreline{ casename, 3, "0.5", 3.0, 20.0};
        BOOST_TEST_MESSAGE("Checking centreline undercooling values");
        centreline.boost_check();
    }

BOOST_AUTO_TEST_SUITE_END();

