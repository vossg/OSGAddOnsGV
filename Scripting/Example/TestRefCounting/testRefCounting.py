import sys
sys.path.append("/home/hecher/dev/Projekte/OSGExt/Scripting/Example/TestRefCounting")

from MFRefCountTest import (MFRecPtrAccessTest, MFUnrecPtrAccessTest, MFWeakPtrAccessTest)
from SFRefCountTest import (SFRecPtrAccessTest, SFUnrecPtrAccessTest, SFWeakPtrAccessTest, SingleFieldTest)

tests = []

def addMFPtrTests():
    tests.append(MFRecPtrAccessTest  ('mMFRecNodePtr'  , 2, _fieldAccessHandler))
    tests.append(MFUnrecPtrAccessTest('mMFUnrecNodePtr', 2, _fieldAccessHandler))
    tests.append(MFWeakPtrAccessTest ('mMFWeakNodePtr' , 2, _fieldAccessHandler))
    
def addSFPtrTests():
    tests.append(SFRecPtrAccessTest  ('mSFRecNodePtr'  , _fieldAccessHandler))
    #tests.append(SFRecPtrAccessTest  ('mSFUnrecNodePtr', _fieldAccessHandler))
    #tests.append(SFRecPtrAccessTest  ('mSFWeakNodePtr' , _fieldAccessHandler))
    
def addMFValueTests():
    pass

def addSFValueTests():
    pass      

def init():
    addMFPtrTests()
    addSFPtrTests()
    addMFValueTests()
    addSFValueTests()
    
    for test in tests:        
        print('\n\n-----------------------------------------------------------------')
        print('    STARTING TEST ON "' + test.get_fieldname() + '"\n')
        
        if test.do_test():
            print('\n    SUCCESSFULLY COMPLETED ON "' + test.get_fieldname() + '".')
        else:
            print('\n    FAILED ON "' + test.get_fieldname() + '".')

        print('-----------------------------------------------------------------')
        
    return
    
def shutdown():
    pass

def frame(timeStamp, frameCount):
    pass

def changed(whichField, origin, details):
    pass