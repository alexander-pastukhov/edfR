#include <sstream>
#include <math.h>

#include <Rcpp.h>
using namespace Rcpp;

#include "SRResearch/edf.h"

//' @title Opens EDF file, throws exception on error
//' @description Opens EDF file for reading, throws exception and prints error message if fails.
//' @param std::string filename, name of the EDF file
//' @param int consistency, consistency check control (for the time stamps of the start
//' and end events, etc). 0, no consistency check. 1, check consistency and report.
//' 2, check consistency and fix.
//' @param int loadevents, load/skip loading events 0, do not load events. 1, load events.
//' @param int loadsamples, load/skip loading of samples 0, do not load samples. 1, load samples.
//' @keywords internal
EDFFILE* safely_open_edf_file(std::string filename, int consistency, int loadevents, int loadsamples){
  // opening the edf file
  int ReturnValue;
  EDFFILE* edfFile= edf_open_file(filename.c_str(), consistency, loadevents, loadsamples, &ReturnValue);

  // throwing an exception, if things go pear shaped
  if (ReturnValue != 0){
    std::stringstream error_message_stream;
    error_message_stream << "Error opening file '" << filename << "', error code: " << ReturnValue;
    ::Rf_error(error_message_stream.str().c_str());
  }

  return edfFile;
}

//' @title Reads preamble of the EDF file.
//'
//' @description Reads preamble of the EDF file.
//'
//' @export
//' @examples
//' read_preamble('test.edf')
//[[Rcpp::export]]
std::string read_preamble(std::string filename){
  EDFFILE* edfFile= safely_open_edf_file(filename, 2, 0, 0);

  // getting preable
  int ReturnValue;
  char preamble_buffer[2048];
  ReturnValue= edf_get_preamble_text(edfFile, preamble_buffer, 2048);
  if (ReturnValue!=0)
  {
    std::stringstream error_message_stream;
    error_message_stream << "Error reading preable for file '" << filename << "', error code: " << ReturnValue;
    ::Rf_error(error_message_stream.str().c_str());
  }
  std::string preamble(preamble_buffer);

  // closing file
  edf_close_file(edfFile);

  return preamble;
}