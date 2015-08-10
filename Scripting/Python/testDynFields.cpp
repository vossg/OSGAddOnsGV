
#include "stdio.h"


#include "OSGBaseInitFunctions.h"
#include "OSGPythonScript.h"
#include "OSGVecSFields.h"

void dumpType(OSG::FieldContainerType &oType)
{
    fprintf(stderr, "td : %s\n",
            oType.getCName());

    for(OSG::UInt32 i = 0; i <= oType.getNumFieldDescs(); ++i)
    {
        OSG::FieldDescriptionBase *pDesc = oType.getFieldDesc(i);

        fprintf(stderr, "  [%d] : %p", i, static_cast<void *>(pDesc));
        
        if(pDesc != NULL)
        {
            fprintf(stderr, " %s", pDesc->getCName());
            fprintf(stderr, " %s", pDesc->getFieldType().getCName());
            fprintf(stderr, " %d", pDesc->getFieldId());
        }
        fprintf(stderr, "\n");
    }
}

int main(int argc, char **argv)
{
    OSG::osgInit(argc, argv);

    OSG::PythonScriptUnrecPtr pObj = OSG::PythonScript::create();

    fprintf(stderr, "sf type : %d\n",
            OSG::FieldTraits<OSG::Vec3f>::getType().getId());

    fprintf(stderr, "sf type : %d\n",
            OSG::SFVec3f::getClassType().getId());


    fprintf(stderr, "nc : %d | sp : %d | sb : %d | py : %d\n",
            OSG::NodeCore    ::NextFieldId,
            OSG::ScriptParent::NextFieldId,
            OSG::ScriptBase  ::NextFieldId,
            OSG::PythonScript::NextFieldId);

    fprintf(stderr, "nc : %d | sp : %d | sb : %d | py : %d\n",
            OSG::NodeCore    ::getClassType().getNumFieldDescs(),
            OSG::ScriptParent::getClassType().getNumFieldDescs(),
            OSG::ScriptBase  ::getClassType().getNumFieldDescs(),
            OSG::PythonScript::getClassType().getNumFieldDescs());

    dumpType(OSG::NodeCore    ::getClassType());
    dumpType(OSG::ScriptParent::getClassType());
    dumpType(OSG::ScriptBase  ::getClassType());
    dumpType(OSG::PythonScript::getClassType());

    pObj->dump();

    OSG::UInt32 fIdx = pObj->addField(OSG::SFVec3f::getClassType().getId(),
                                      "testField");

    pObj->dump();

    dumpType(pObj->getType());

    OSG::GetFieldHandlePtr pFH = pObj->getField(fIdx);

    if(pFH != NULL)
    {
        fprintf(stderr, "%s %s\n", 
                pFH->getName().c_str(), 
                pFH->getType().getCName());
    }
    

    pObj->subField(fIdx);

    pObj->dump();

    dumpType(pObj->getType());



    pObj = NULL;

    OSG::osgExit();

    return 0;
}

