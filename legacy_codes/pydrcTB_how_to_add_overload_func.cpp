#include "TBread.h"
#include "TBdetector.h"
#include "TButility.h"
#include "TBmonit.h"
#include "TBplot.h"
#include <boost/python.hpp>

// void (TBread::*ntuplizeFastmode_noSplit)(const boost::python::list& alist, const std::string& output) = &TBread::ntuplizeFastmode;
// void (TBread::*ntuplizeFastmode_Split)(const boost::python::list& alist, const std::string& output, const int maxEntry, const int entryPerSplit) = &TBread::ntuplizeFastmode;

BOOST_PYTHON_MODULE(pydrcTB) {
  boost::python::class_<TBread>("TBread")
    // .def("ntuplizeWaveform", &TBread::ntuplizeWaveform)
    .def("ntuplizeWaveform", static_cast<void (TBread::*)(const boost::python::list& alist, const std::string& output)>(&TBread::ntuplizeWaveform))
    .def("ntuplizeWaveform", static_cast<void (TBread::*)(const boost::python::list& alist, const std::string& output, const int maxEntry, const int entryPerSplit)>(&TBread::ntuplizeWaveform))
    // .def("ntuplizeFastmode", &TBread::ntuplizeFastmode)
    // .def("ntuplizeFastmode", ntuplizeFastmode_noSplit)
    // .def("ntuplizeFastmode", ntuplizeFastmode_Split)

    // or one can use the line below to overload functions
    // .def<void (TBread::*)(const boost::python::list& alist, const std::string& output)>("ntuplizeFastmode", &TBread::ntuplizeFastmode)
    // .def<void (TBread::*)(const boost::python::list& alist, const std::string& output, const int maxEntry, const int entryPerSplit)>("ntuplizeFastmode", &TBread::ntuplizeFastmode)

    // or also using static cast, like this 
    .def("ntuplizeFastmode", static_cast<void (TBread::*)(const boost::python::list& alist, const std::string& output)>(&TBread::ntuplizeFastmode))
    // for const functions >> add const behind like this >> .def("func_name", static_cast<ReturnType (Class::*)(arguments) const>(&Class::function))
    // EX) .def("getcid", static_cast<TBcid (TButility::*)(TBdetector::detid did) const>(&TButility::getcid))
    .def("ntuplizeFastmode", static_cast<void (TBread::*)(const boost::python::list& alist, const std::string& output, const int maxEntry, const int entryPerSplit)>(&TBread::ntuplizeFastmode))
    .def("setMappingPath", &TBread::setMappingPath)
    .def("setPedestalPath", &TBread::setPedestalPath);

  boost::python::class_<TBcid>("TBcid", boost::python::init<int, int>())
    .def("mid", &TBcid::mid)
    .def("channel", &TBcid::channel);

  boost::python::class_<TBdetector>("TBdetector")
    .def("detType", &TBdetector::detType)
    .def("isSiPM", &TBdetector::isSiPM)
    .def("isModule", &TBdetector::isModule)
    .def("isNull", &TBdetector::isNull)
    .def("module", &TBdetector::module)
    .def("tower", &TBdetector::tower)
    .def("isCeren", &TBdetector::isCeren)
    .def("plate", &TBdetector::plate)
    .def("column", &TBdetector::column);

  boost::python::class_<TButility>("TButility")
    .def("loading", &TButility::loading)
    .def("loadped", &TButility::loadped)
    .def("find", &TButility::find)
    .def("retrievePed", &TButility::retrievePed)
    .def("setPSpedcut", &TButility::setPSpedcut)
    .def("setPS1mipcut", &TButility::setPS1mipcut)
    .def("setPS3mipcut", &TButility::setPS3mipcut)
    .def("setMuoncut", &TButility::setMuoncut)
    .def("pid", &TButility::pid);

  boost::python::class_<TBmonit>("TBmonit", boost::python::init<const std::string&>())
    .def("setFastmodeFiles", &TBmonit::setFastmodeFiles)
    .def("setWaveformFiles", &TBmonit::setWaveformFiles)
    .def("setPedestalPath", &TBmonit::setPedestalPath)
    .def("setMappingPath", &TBmonit::setMappingPath)
    .def("SetADCmax", &TBmonit::SetADCmax)
    .def("SetADCbin", &TBmonit::SetADCbin)
    .def("SetOutputName", &TBmonit::SetOutputName)
    .def("MonitPlots", &TBmonit::MonitPlots);

  boost::python::class_<TBplot>("TBplot", boost::python::init<int, int, const std::string&, const std::string&>())
    .def("openFile", &TBplot::openFile)
    .def("closeFile", &TBplot::closeFile)
    .def("loadTH", &TBplot::loadTH)
    .def("loadTH1D", &TBplot::loadTH1D)
    .def("loadTH2D", &TBplot::loadTH2D)
    .def("Draw", &TBplot::Draw)
    .def("setSaveName", &TBplot::setSaveName)
    .def("SavePng", &TBplot::SavePng);
}
