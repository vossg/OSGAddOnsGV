from RefCountTestBase import FieldAccessTestBase
import osg2.osg as osg

#-------------------------------------------------------------------------------
class MFPtrAccessTestBase(FieldAccessTestBase):
    """
    Base class for pointer multi-field refcounting tests.
    
    Base class for testing the integrity of a pointer multi-field when accessing
    it with the access methods provided in the PyFieldAccessHandler class.
    """
    def __init__(self, fieldname, fieldidx, accesshandler):
        FieldAccessTestBase.__init__(self, fieldname, accesshandler)
        
        self.fieldidx      = fieldidx
        self.rec_refcount  = self.get_value().getRefCount() - 1 # goes out of scope below
        self.weak_refcount = self.get_value().getWeakRefCount()
        
    def get_value(self):
        return self.accesshandler.getPtrMField(self.fieldname, self.fieldidx)
        
    def set_value(self, value):
        self.accesshandler.setPtrMField(self.fieldname, self.fieldidx, value)
  
 
#-------------------------------------------------------------------------------
class MFRecPtrAccessTest(MFPtrAccessTestBase):
    """
    Tests the refcounting integrity of a MFRecPtr field when accessing it with
    the access methods provided in the PyFieldAccessHandler class.
    """
    def __init__(self, fieldname, fieldidx, accesshandler):
        MFPtrAccessTestBase.__init__(self, fieldname, fieldidx, accesshandler)

    def do_test(self):        
        print('Retrieve node entry 2 from multi-field mMFUnrecNodePtr as oldn2:')
        oldn2 = self.get_value()
        self.rec_refcount += 1
        osg.setName(oldn2, 'oldn2')
        if oldn2:
            print('    success!')
            
            
        print('Check refcount of oldn2:')
        print('    refcount = ' + str(oldn2.getRefCount()) + ' (should be 2)')
        if oldn2.getRefCount() != self.rec_refcount:
            print('    FAILED with refcount = ' + str(oldn2.getRefCount()))
            return False
        else:
            print('    success!')
            
            
        print('Create new node as newn2:')
        newn2 = osg.Node.create()
        osg.setName(newn2, 'newn2')
        print('    success!')
        
        
        print('Replace entry 2 in with newn2:')
        self.set_value(newn2)
        self.rec_refcount -= 1
        print('    success!')
        
        
        print('Check refcounts:')
        print('    oldn2 refcount = ' + str(oldn2.getRefCount()) + ' (should be 1 now)')
        if oldn2.getRefCount() != self.rec_refcount:
            print('        FAILED with refcount = ' + str(oldn2.getRefCount()))
            return False
        else:
            print('        success!')
            
            
        print('    newn2 refcount = ' + str(newn2.getRefCount()) + ' (should be 2)')
        self.rec_refcount += 1
        if newn2.getRefCount() != 2:
            print('        FAILED with refcount = ' + str(newn2.getRefCount()))
            return False
        else:
            print('        success!')        
        
        
        print('Retrieve entry 2 in as testnewn2:')
        testnewn2 = self.get_value()
        self.rec_refcount += 1
        if testnewn2:
            print('        success!')
        else:
            print('        FAILED retrieving testnewn2')
            return False
            
            
        print('Check refcounts:')
        print('    testnewn2 refcount = ' + str(testnewn2.getRefCount()) + ' (should be 3)')
        if testnewn2.getRefCount() != 3:
            print('        FAILED with refcount = ' + str(testnewn2.getRefCount()))
            return False
        else:
            print('        success!')
        print('    newn2 refcount = ' + str(newn2.getRefCount()) + ' (should be 3)')
        if newn2.getRefCount() != 3:
            print('        FAILED with refcount = ' + str(newn2.getRefCount()))
            return False
        else:
            print('        success!')
            
            
        print('Delete testnewn2 and newn2 and check refcount:')
        testnewn2 = 0
        newn2     = 0
        self.rec_refcount -= 2
        
        tmp = self.get_value()
        self.rec_refcount += 1
        print('    multi-field entry refcount = ' + str(tmp.getRefCount()) + ' (should be 2)')
        if tmp.getRefCount() != 2:
            print('        FAILED with refcount = ' + str(tmp.getRefCount()))
            return False
        else:
            print('        success!')
    
        return True


#-------------------------------------------------------------------------------
class MFUnrecPtrAccessTest(MFRecPtrAccessTest):
    """
    Tests the refcounting integrity of a MFRecPtr field when accessing it with
    the access methods provided with the PyFieldAccessHandler class.
    
    The class is identical to MFRecPtrAccessTest and is just there for a clearer
    name. It is derived from MFRecPtrAccessTest and implements no own functions.
    """
    def __init__(self, fieldname, fieldidx, accesshandler):
        MFPtrAccessTestBase.__init__(self, fieldname, fieldidx, accesshandler)
    
    
#-------------------------------------------------------------------------------
class MFWeakPtrAccessTest(MFPtrAccessTestBase):
    """
    Tests the refcounting integrity of a MFWeakPtr field when accessing it with
    the access methods provided in the PyFieldAccessHandler class.
    """
    def __init__(self, fieldname, fieldidx, accesshandler):
        MFPtrAccessTestBase.__init__(self, fieldname, fieldidx, accesshandler)
  
    def do_test(self):
        print('Retrieve node entry 2 from multi-field as oldn2:')
        oldn2 = self.get_value()
        
        self.rec_refcount  += 1
        self.weak_refcount += 0
        
        osg.setName(oldn2, 'oldn2')
        if oldn2:
            print('    success!')
            
        print('Check refcount of oldn2:')
        print('    refcount      = ' + str(oldn2.getRefCount())     + ' (should be ' + str(self.rec_refcount) + ')')
        
        print('Check weak refcount of oldn2:')
        print('    weak refcount = ' + str(oldn2.getWeakRefCount()) + ' (should be ' + str(self.weak_refcount) + ')')
        
        if (oldn2.getRefCount() != self.rec_refcount) or (oldn2.getWeakRefCount() != self.weak_refcount):
            print('    FAILED')
            return False
        else:
            print('    success!')
            
            
        print('Create new node as newn2:')
        newn2 = osg.Node.create()
        osg.setName(newn2, 'newn2')
        print('    success!')
        
        
        print('Replace entry 2 in with newn2:')
        self.set_value(newn2)
        self.rec_refcount  -= 0 # for oldn2
        self.weak_refcount -= 1 # for oldn2
        print('    success!')
        
        print('Check refcount of oldn2:')
        print('    refcount      = ' + str(oldn2.getRefCount())     + ' (should be ' + str(self.rec_refcount) + ')')
        
        print('Check weak refcount of oldn2:')
        print('    weak refcount = ' + str(oldn2.getWeakRefCount()) + ' (should be ' + str(self.weak_refcount) + ')')
        
        if (oldn2.getRefCount() != self.rec_refcount) or (oldn2.getWeakRefCount() != self.weak_refcount):
            print('        FAILED')
            return False
        else:
            print('        success!')
            
        self.rec_refcount  = 1 # for newn2
        self.weak_refcount = 1 # for newn2
        print('Check refcount of newn2:')
        print('    refcount      = ' + str(newn2.getRefCount())     + ' (should be ' + str(self.rec_refcount) + ')')
        
        print('Check weak refcount of newn2:')
        print('    weak refcount = ' + str(newn2.getWeakRefCount()) + ' (should be ' + str(self.weak_refcount) + ')')
        
        if (newn2.getRefCount() != self.rec_refcount) or (newn2.getWeakRefCount() != self.weak_refcount):
            print('        FAILED')
            return False
        else:
            print('        success!')  
        
        
        print('Retrieve entry 2 in as testnewn2:')
        testnewn2 = self.get_value()
        self.rec_refcount  += 1 # for testnewn2
        self.weak_refcount  = 1 # for testnewn2
        if testnewn2:
            print('        success!')
        else:
            print('        FAILED retrieving testnewn2')
            return False
            
            
        print('Check refcount of testnewn2:')
        print('    refcount      = ' + str(testnewn2.getRefCount())     + ' (should be ' + str(self.rec_refcount) + ')')
        
        print('Check weak refcount of testnewn2:')
        print('    weak refcount = ' + str(testnewn2.getWeakRefCount()) + ' (should be ' + str(self.weak_refcount) + ')')
        
        if (testnewn2.getRefCount() != self.rec_refcount) or (testnewn2.getWeakRefCount() != self.weak_refcount):
            print('        FAILED')
            return False
        else:
            print('        success!')  
            
            
        print('Delete testnewn2 and newn2 and check refcount:')
        testnewn2 = 0
        newn2     = 0
        self.rec_refcount  -= 2 # for ptr inside the multi-field
        self.weak_refcount -= 0 # for ptr inside the multi-field
        
        # ptr inside multifield gets deleted, as the refcount is 0 and the 1
        # remaining weakcount does not prevent the object from being destroyed
        tmp = self.get_value()
        
        if(self.get_value()):
            return False
        else:
            print('   success! entry deleted from multi-field')
        
        return True
