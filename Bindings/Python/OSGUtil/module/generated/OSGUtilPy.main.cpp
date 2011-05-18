// This file has been generated by Py++.

// PyOpenSG is (C) Copyright 2005-2009 by Allen Bierbaum
//
// This file is part of PyOpenSG.
//
// PyOpenSG is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// PyOpenSG is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
// more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#if __GNUC__ >= 4 || __GNUC_MINOR__ >=3
#pragma GCC diagnostic warning "-Wold-style-cast"
#endif


#include "boost/python.hpp"

#include "OSGUtil_mainheader.h"

#include "OsgPtrHelpers.h"

#include "boost/python/suite/indexing/map_indexing_suite.hpp"

#include "boost/python/suite/indexing/vector_indexing_suite.hpp"

#include "generated/GeoTypeGraphOp.pypp.hpp"

#include "generated/GraphOp.pypp.hpp"

#include "generated/MakeTransparentGraphOp.pypp.hpp"

#include "generated/MaterialMergeGraphOp.pypp.hpp"

#include "generated/MergeGraphOp.pypp.hpp"

#include "generated/NavigationManager.pypp.hpp"

#include "generated/Navigator.pypp.hpp"

#include "generated/PerfMonitorForeground.pypp.hpp"

#include "generated/PerfMonitorForegroundBase.pypp.hpp"

#include "generated/PruneGraphOp.pypp.hpp"

#include "generated/SharePtrGraphOp.pypp.hpp"

#include "generated/SimpleSceneManager.pypp.hpp"

#include "generated/SimpleStatisticsForeground.pypp.hpp"

#include "generated/SimpleStatisticsForegroundBase.pypp.hpp"

#include "generated/SplitGraphOp.pypp.hpp"

#include "generated/StripeGraphOp.pypp.hpp"

#include "generated/VerifyGeoGraphOp.pypp.hpp"

#include "generated/VerifyGraphOp.pypp.hpp"

using namespace std;

namespace bp = boost::python;

BOOST_PYTHON_MODULE(OSGUtilPy){
    register_GeoTypeGraphOp_class();

    register_GraphOp_class();

    register_MakeTransparentGraphOp_class();

    register_MaterialMergeGraphOp_class();

    register_MergeGraphOp_class();

    register_NavigationManager_class();

    register_Navigator_class();

    register_PerfMonitorForegroundBase_class();

    register_PerfMonitorForeground_class();

    register_PruneGraphOp_class();

    register_SharePtrGraphOp_class();

    register_SimpleSceneManager_class();

    register_SimpleStatisticsForegroundBase_class();

    register_SimpleStatisticsForeground_class();

    register_SplitGraphOp_class();

    register_StripeGraphOp_class();

    register_VerifyGeoGraphOp_class();

    bp::def("getCPtr", &pyopensg::ToCPtr<OSG::GraphOp::ObjRefPtr>, bp::return_value_policy<bp::reference_existing_object>());

    bp::def("getCPtr", &pyopensg::ToCPtr<OSG::MakeTransparentGraphOp::ObjRefPtr>, bp::return_value_policy<bp::reference_existing_object>());

    bp::def("getCPtr", &pyopensg::ToCPtr<OSG::MaterialMergeGraphOp::ObjRefPtr>, bp::return_value_policy<bp::reference_existing_object>());

    bp::def("getCPtr", &pyopensg::ToCPtr<OSG::PruneGraphOp::ObjRefPtr>, bp::return_value_policy<bp::reference_existing_object>());

    bp::def("getCPtr", &pyopensg::ToCPtr<OSG::SharePtrGraphOp::ObjRefPtr>, bp::return_value_policy<bp::reference_existing_object>());

    bp::def("getCPtr", &pyopensg::ToCPtr<OSG::VerifyGeoGraphOp::ObjRefPtr>, bp::return_value_policy<bp::reference_existing_object>());

    bp::def("getCPtr", &pyopensg::ToCPtr<OSG::GeoTypeGraphOp::ObjRefPtr>, bp::return_value_policy<bp::reference_existing_object>());

    bp::def("getCPtr", &pyopensg::ToCPtr<OSG::StripeGraphOp::ObjRefPtr>, bp::return_value_policy<bp::reference_existing_object>());

    bp::def("getCPtr", &pyopensg::ToCPtr<OSG::SplitGraphOp::ObjRefPtr>, bp::return_value_policy<bp::reference_existing_object>());

    bp::def("getCPtr", &pyopensg::ToCPtr<OSG::VerifyGraphOp::ObjRefPtr>, bp::return_value_policy<bp::reference_existing_object>());

    bp::def("getCPtr", &pyopensg::ToCPtr<OSG::MergeGraphOp::ObjRefPtr>, bp::return_value_policy<bp::reference_existing_object>());

    /** Helpers for OSG::SimpleStatisticsForeground */

    bp::def("RecPtr", &pyopensg::ToRefCountPtr<OSG::SimpleStatisticsForeground, OSG::SimpleStatisticsForeground::ObjRecPtr>);

    bp::def("CPtr", &pyopensg::ToCPtr<OSG::SimpleStatisticsForeground::ObjRecPtr >, bp::return_value_policy<bp::reference_existing_object>());

    /** Helpers for OSG::PerfMonitorForeground */

    bp::def("RecPtr", &pyopensg::ToRefCountPtr<OSG::PerfMonitorForeground, OSG::PerfMonitorForeground::ObjRecPtr>);

    bp::def("CPtr", &pyopensg::ToCPtr<OSG::PerfMonitorForeground::ObjRecPtr >, bp::return_value_policy<bp::reference_existing_object>());

    register_VerifyGraphOp_class();
}
