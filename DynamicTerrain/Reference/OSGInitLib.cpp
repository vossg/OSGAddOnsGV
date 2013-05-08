#include <OSGAddOnsConfig.h>
#include <OSGBaseFunctions.h>
#include <OSGFieldContainer.h>
#include <OSGFieldContainerFactory.h>
//#include <OSGSharedObjectHandler.h>

OSG_USING_NAMESPACE

static StaticInitFuncWrapper OSGContribPLInit( &FieldContainerFactory::registerPlugin );
