#-------------------------------------------------------------------------------
# Changed mask helper:

changed_mask_to_test = 0

def binary_rep(s):
    return str(s) if s<=1 else bin(s>>1) + str(s&1)
    
def set_changed_mask_to_test(mask):
    global changed_mask_to_test
    changed_mask_to_test = mask
    
def reset_changed_mask():
    global changed_mask_to_test
    changed_mask_to_test = 0
    
def test_changed_mask(whichField):    
    if changed_mask_to_test == 0:
        return
    
    if whichField & changed_mask_to_test:
        print('  -> SUCCESS')
    else:
        print('  -> failure')
#-------------------------------------------------------------------------------
    
def test_single_fields():
    """
    Tests the supported value single-fields. Types which are commented out are
    not supported (yet).
    """
    
    #---------------------------------------------------------------------------
    print('\n*** Testing read access via property:')
    
    print('prop: SFBool      = ' + str(self.mSFBool))
    print('prop: SFInt32     = ' + str(self.mSFInt32))
    print('prop: SFUInt32    = ' + str(self.mSFUInt32))
    print('prop: SFUInt64    = ' + str(self.mSFUInt64))
    print('prop: SFDouble    = ' + str(self.mSFDouble))
    print('prop: SFReal32    = ' + str(self.mSFReal32))
    print('prop: SFReal64    = ' + str(self.mSFReal64))
    print('prop: SFColor     = ' + str(self.mSFColor))
    print('prop: SFColor3f   = ' + str(self.mSFColor3f))
    print('prop: SFColorRGBA = ' + str(self.mSFColorRGBA))
    print('prop: SFColor4f   = ' + str(self.mSFColor4f))
    print('prop: SFColor4ub  = ' + str(self.mSFColor4ub))
    print('prop: SFString    = ' + str(self.mSFString))
    print('prop: SFTime      = ' + str(self.mSFTime))
    print('prop: SFVolume    = ' + str(self.mSFVolume))
    print('prop: SFBoxVolume = ' + str(self.mSFBoxVolume))
    print('prop: SFPnt2f     = ' + str(self.mSFPnt2f))
    print('prop: SFPnt3f     = ' + str(self.mSFPnt3f))
    print('prop: SFPnt4f     = ' + str(self.mSFPnt4f))
    print('prop: SFVec2d     = ' + str(self.mSFVec2d))
    print('prop: SFVec2f     = ' + str(self.mSFVec2f))
    print('prop: SFVec3d     = ' + str(self.mSFVec3d))
    print('prop: SFVec3f     = ' + str(self.mSFVec3f))
    print('prop: SFVec4d     = ' + str(self.mSFVec4d))
    print('prop: SFVec4f     = ' + str(self.mSFVec4f))
    print('prop: SFMatrix    = ' + str(self.mSFMatrix))
    print('prop: SFMatrix4d  = ' + str(self.mSFMatrix4d))
    print('prop: SFRotation  = ' + str(self.mSFRotation))
    print('prop: SFQuaterion = ' + str(self.mSFQuaternion))
    
    # Check if we are working on a Vec type:
    print('mSFVec4d.length() = ' + str(self.mSFVec4d.length()))
    
    # Not supported types:
    
    #print('prop: SFInt64      = ' + str(self.mSFInt64))
    #print('prop: SFColor3ub   = ' + str(self.mSFColor3ub))
    #print('prop: SFPlane      = ' + str(self.mSFPlane))
    #print('prop: SFPnt2d      = ' + str(self.mSFPnt2d))
    #print('prop: SFPnt3d      = ' + str(self.mSFPnt3d))
    #print('prop: SFPnt4d      = ' + str(self.mSFPnt4d))
    #print('prop: SFVec2ub     = ' + str(self.mSFVec2ub))
    #print('prop: SFVec3ub     = ' + str(self.mSFVec3ub))
    #print('prop: SFVec4ub     = ' + str(self.mSFVec4ub))
    #print('prop: SFQuateriond = ' + str(self.mSFQuaterniond))
    #---------------------------------------------------------------------------

    #---------------------------------------------------------------------------
    print('\n*** Testing write access via set_FIELDNAME():')
    
    self.set_mSFBool(True)
    if(self.mSFBool == True):
        print '  mSFBool       -> SUCCESS'
    else:
        print '  mSFBool       -> FAILURE'
        
    self.set_mSFInt32(-3)
    if(self.mSFInt32 == -3):
        print '  mSFInt32      -> SUCCESS'
    else:
        print '  mSFInt32      -> FAILURE'
        
    self.set_mSFUInt32(2)
    if(self.mSFUInt32 == 2):
        print '  mSFUInt32     -> SUCCESS'
    else:
        print '  mSFUInt32     -> FAILURE'
        
    self.set_mSFUInt64(5)
    if(self.mSFUInt64 == 5):
        print '  mSFUInt64     -> SUCCESS'
    else:
        print '  mSFUInt64     -> FAILURE'
        
    self.set_mSFDouble(3.4)
    if(self.mSFDouble == 3.4):
        print '  mSFDouble     -> SUCCESS'
    else:
        print '  mSFDouble     -> FAILURE'
        
    # Real32 has a small precision error:
    self.set_mSFReal32(5.2)
    if(self.mSFReal32 == 5.2):
        print '  mSFReal32     -> SUCCESS'
    else:
        print '  mSFReal32     -> FAILURE (small precision error)'
        print '                  value = ' + str(self.mSFReal32)
        
    self.set_mSFReal64(4.4)
    if(self.mSFReal64 == 4.4):
        print '  mSFReal64     -> SUCCESS'
    else:
        print '  mSFReal64     -> FAILURE'
        
    self.set_mSFColor(osg.Color3f(3,3,3))
    if(self.mSFColor == osg.Color3f(3,3,3)):
        print '  mSFColor      -> SUCCESS'
    else:
        print '  mSFColor      -> FAILURE'
        
    self.set_mSFColor3f(osg.Color3f(2.1,1,1))
    if(self.mSFColor3f == osg.Color3f(2.1,1,1)):
        print '  mSFColor3f    -> SUCCESS'
    else:
        print '  mSFColor3f    -> FAILURE'
        
    self.set_mSFColorRGBA(osg.Color4f(1,2,3,4))
    if(self.mSFColorRGBA == osg.Color4f(1,2,3,4)):
        print '  mSFColorRGBA  -> SUCCESS'
    else:
        print '  mSFColorRGBA  -> FAILURE'
        
    self.set_mSFColor4f(osg.Color4f(1,2,3,0))
    if(self.mSFColor4f == osg.Color4f(1,2,3,0)):
        print '  mSFColor4f    -> SUCCESS'
    else:
        print '  mSFColor4f    -> FAILURE'
        
    self.set_mSFColor4ub(osg.Color4ub(0,0,0,1))
    if(self.mSFColor4ub == osg.Color4ub(0,0,0,1)):
        print '  mSFColor4ub   -> SUCCESS'
    else:
        print '  mSFColor4ub   -> FAILURE'
        
    self.set_mSFString('test')
    if(self.mSFString == 'test'):
        print '  mSFString     -> SUCCESS'
    else:
        print '  mSFString     -> FAILURE'
        
    self.set_mSFTime(3)
    if(self.mSFTime == 3):
        print '  mSFTime       -> SUCCESS'
    else:
        print '  mSFTime       -> FAILURE'
        
    self.set_mSFVolume(osg.BoxVolume(2,2,2,3,3,2))
    if(self.mSFVolume.getMin() == osg.Vec3f(2,2,2)):
        print '  mSFVolume     -> SUCCESS'
    else:
        print self.mSFVolume
        print '  mSFVolume     -> FAILURE'
        
    self.set_mSFBoxVolume(osg.BoxVolume(2,2,2,3,3,3))
    if(self.mSFBoxVolume.getMin() == osg.Vec3f(2,2,2)):
        print '  mSFBoxVolume  -> SUCCESS'
    else:
        print '  mSFBoxVolume  -> FAILURE'
        
    self.set_mSFPnt2f(osg.Pnt2f(1.1,2))
    if(self.mSFPnt2f == osg.Pnt2f(1.1,2)):
        print '  mSFPnt2f      -> SUCCESS'
    else:
        print '  mSFPnt2f      -> FAILURE'
        
    self.set_mSFPnt3f(osg.Pnt3f(3.1,2,1))
    if(self.mSFPnt3f == osg.Pnt3f(3.1,2,1)):
        print '  mSFPnt3f      -> SUCCESS'
    else:
        print '  mSFPnt3f      -> FAILURE'
        
    self.set_mSFPnt4f(osg.Pnt4f(0.1,0,0,1))
    if(self.mSFPnt4f == osg.Pnt4f(0.1,0,0,1)):
        print '  mSFPnt4f      -> SUCCESS'
    else:
        print '  mSFPnt4f      -> FAILURE'
        
    self.set_mSFVec2d(osg.Vec2d(3.3,3.3))
    if(self.mSFVec2d == osg.Vec2d(3.3,3.3)):
        print '  mSFVec2d      -> SUCCESS'
    else:
        print '  mSFVec2d      -> FAILURE'
        
    self.set_mSFVec2f(osg.Vec2f(1.1,2))
    if(self.mSFVec2f == osg.Vec2f(1.1,2)):
        print '  mSFVec2f      -> SUCCESS'
    else:
        print '  mSFVec2f      -> FAILURE'
        
    self.set_mSFVec3d(osg.Vec3d(1,2.1,3))
    if(self.mSFVec3d == osg.Vec3d(1,2.1,3)):
        print '  mSFVec3d      -> SUCCESS'
    else:
        print '  mSFVec3d     -> FAILURE'
        
    self.set_mSFVec3f(osg.Vec3f(8.4,7,6))
    if(self.mSFVec3f == osg.Vec3f(8.4,7,6)):
        print '  mSFVec3f      -> SUCCESS'
    else:
        print '  mSFVec3f      -> FAILURE'
        
    self.set_mSFVec4d(osg.Vec4d(4,2,4.3,0.2))
    if(self.mSFVec4d == osg.Vec4d(4,2,4.3,0.2)):
        print '  mSFVec4d      -> SUCCESS'
    else:
        print '  mSFVec4d      -> FAILURE'
        
    self.set_mSFVec4f(osg.Vec4f(3,3,3.2,3))
    if(self.mSFVec4f == osg.Vec4f(3,3,3.2,3)):
        print '  mSFVec4f      -> SUCCESS'
    else:
        print '  mSFVec4f      -> FAILURE'
        
    self.set_mSFMatrix(osg.Matrix(1,2.2,3,4,5,6,7,8,8,8,8,8,8,8,8,8))
    if(self.mSFMatrix == osg.Matrix(1,2.2,3,4,5,6,7,8,8,8,8,8,8,8,8,8)):
        print '  mSFMatrix     -> SUCCESS'
    else:
        print '  mSFMatrix     -> FAILURE'
        
    self.set_mSFMatrix4d(osg.Matrix4d(2.1,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1))
    if(self.mSFMatrix4d == osg.Matrix4d(2.1,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1)):
        print '  mSFMatrix4d   -> SUCCESS'
    else:
        print '  mSFMatrix4d   -> FAILURE'
        
    self.set_mSFRotation(osg.Quaternion(3,2,1,45))
    if(self.mSFRotation == osg.Quaternion(3,2,1,45)):
        print '  mSFRotation   -> SUCCESS'
    else:
        print '  mSFRotation   -> FAILURE'
        
    self.set_mSFQuaternion(osg.Quaternion(3,2,1,45))
    if(self.mSFRotation == osg.Quaternion(3,2,1,45)):
        print '  mSFQuaternion -> SUCCESS'
    else:
        print '  mSFQuaternion -> FAILURE'
    #---------------------------------------------------------------------------
    
    #---------------------------------------------------------------------------
    print('\n*** Testing read access via get_FIELDNAME():')
    
    print('get_FIELDNAME() SFBool      = ' + str(self.get_mSFBool()))
    print('get_FIELDNAME() SFInt32     = ' + str(self.get_mSFInt32()))
    print('get_FIELDNAME() SFUInt32    = ' + str(self.get_mSFUInt32()))
    print('get_FIELDNAME() SFUInt64    = ' + str(self.get_mSFUInt64()))
    print('get_FIELDNAME() SFDouble    = ' + str(self.get_mSFDouble()))
    print('get_FIELDNAME() SFReal32    = ' + str(self.get_mSFReal32()))
    print('get_FIELDNAME() SFReal64    = ' + str(self.get_mSFReal64()))
    print('get_FIELDNAME() SFColor     = ' + str(self.get_mSFColor()))
    print('get_FIELDNAME() SFColor3f   = ' + str(self.get_mSFColor3f()))
    print('get_FIELDNAME() SFColorRGBA = ' + str(self.get_mSFColorRGBA()))
    print('get_FIELDNAME() SFColor4f   = ' + str(self.get_mSFColor4f()))
    print('get_FIELDNAME() SFColor4ub  = ' + str(self.get_mSFColor4ub()))
    print('get_FIELDNAME() SFString    = ' + str(self.get_mSFString()))
    print('get_FIELDNAME() SFTime      = ' + str(self.get_mSFTime()))
    print('get_FIELDNAME() SFVolume    = ' + str(self.get_mSFVolume()))
    print('get_FIELDNAME() SFBoxVolume = ' + str(self.get_mSFBoxVolume()))
    print('get_FIELDNAME() SFPnt2f     = ' + str(self.get_mSFPnt2f()))
    print('get_FIELDNAME() SFPnt3f     = ' + str(self.get_mSFPnt3f()))
    print('get_FIELDNAME() SFPnt4f     = ' + str(self.get_mSFPnt4f()))
    print('get_FIELDNAME() SFVec2d     = ' + str(self.get_mSFVec2d()))
    print('get_FIELDNAME() SFVec2f     = ' + str(self.get_mSFVec2f()))
    print('get_FIELDNAME() SFVec3d     = ' + str(self.get_mSFVec3d()))
    print('get_FIELDNAME() SFVec3f     = ' + str(self.get_mSFVec3f()))
    print('get_FIELDNAME() SFVec4d     = ' + str(self.get_mSFVec4d()))
    print('get_FIELDNAME() SFVec4f     = ' + str(self.get_mSFVec4f()))
    print('get_FIELDNAME() SFMatrix    = ' + str(self.get_mSFMatrix()))
    print('get_FIELDNAME() SFMatrix4d  = ' + str(self.get_mSFMatrix4d()))
    print('get_FIELDNAME() SFRotation  = ' + str(self.get_mSFRotation()))
    print('get_FIELDNAME() SFQuaterion = ' + str(self.get_mSFQuaternion()))    
    #---------------------------------------------------------------------------

    #---------------------------------------------------------------------------
    print('\n*** Testing absence of edit_XXX methods for integral types:')
    error_flag = False
    try:
        self.edit_mSFBool()
    except AttributeError:
        error_flag = True
    if error_flag == True:
        print('  Bool   -> SUCCESS')
    else:
        print('  Bool   -> FAILURE')
        
    error_flag = False
    try:
        self.edit_mSFInt32()
    except AttributeError:
        error_flag = True
    if error_flag == True:
        print('  Int32  -> SUCCESS')
    else:
        print('  Int32  -> FAILURE')
        
    error_flag = False
    try:
        self.edit_mSFUInt32()
    except AttributeError:
        error_flag = True
    if error_flag == True:
        print('  UInt32 -> SUCCESS')
    else:
        print('  UInt32 -> FAILURE')
        
    error_flag = False
    try:
        self.edit_mSFInt64()
    except AttributeError:
        error_flag = True
    if error_flag == True:
        print('  Int64  -> SUCCESS')
    else:
        print('  Int64  -> FAILURE')
        
    error_flag = False
    try:
        self.edit_mSFUInt64()
    except AttributeError:
        error_flag = True
    if error_flag == True:
        print('  UInt64 -> SUCCESS')
    else:
        print('  UInt64-> FAILURE')
        
    error_flag = False
    try:
        self.edit_mSFReal32()
    except AttributeError:
        error_flag = True
    if error_flag == True:
        print('  Real32 -> SUCCESS')
    else:
        print('  Real32 -> FAILURE')
        
    error_flag = False
    try:
        self.edit_mSFReal64()
    except AttributeError:
        error_flag = True
    if error_flag == True:
        print('  Real64 -> SUCCESS')
    else:
        print('  Real64 -> FAILURE')        
    #---------------------------------------------------------------------------

    #---------------------------------------------------------------------------
    print('\n*** Testing trigger of changed() method: edit_mSFVec3f()')
    
    osg.commitChanges() # commit all changes to reset the changelist
    
    self.edit_mSFVec3f().setValues(-2,-3,-3)
    set_changed_mask_to_test(self.mSFVec3fFieldMask)
    osg.commitChanges()
    reset_changed_mask()  
    #---------------------------------------------------------------------------
    
    #---------------------------------------------------------------------------
    print('\n*** Testing trigger of changed() method: self.set_mSFBool(True)')
    
    osg.commitChanges() # commit all changes to reset the changelist
    
    self.set_mSFBool(True)
    set_changed_mask_to_test(self.mSFBoolFieldMask)
    osg.commitChanges()
    reset_changed_mask()  
    #---------------------------------------------------------------------------
    
    #---------------------------------------------------------------------------
    print("\n*** Testing get_FIELDNAME()'s fake read-only behaviour:")    
    self.edit_mSFColor().setValues(-1,0,0)
    self.get_mSFColor().setValues(2,2,-1)
    if(self.get_mSFColor() == osg.Color3f(-1,0,0)):
        print('  get_FIELDNAME()  -> SUCCESS')
    else:
        print('  get_FIELDNAME()  -> FAILURE')

    self.edit_mSFColor().setValues(-2,0,0)
    self.mSFColor.setValues(2,2,-1)
    if(self.mSFColor == osg.Color3f(-2,0,0)):
        print('  get via property -> SUCCESS')
    else:
        print('  get via property -> FAILURE')
        
def test_multi_fields():
    """
    Tests the supported value multi-fields.
    """
    
    #---------------------------------------------------------------------------
    print('\n*** Testing read access via GetHandle on mMFVec3f:')
    if self.mMFVec3f:
        print('  %s -> SUCCESS' % str(self.mMFVec3f[0]))
    #---------------------------------------------------------------------------
    
    #---------------------------------------------------------------------------
    print('\n*** Testing read access via EditHandle on mMFVec3f (triggers changed() call):')
    mfield = self.edit_mMFVec3f()
    if mfield[0] == osg.Vec3f(3.3,3.3,3.3):
        print('  compare value: "%s" -> SUCCESS' % mfield[0])
    else:
        print('  compare value: -> FAILURE')
    #---------------------------------------------------------------------------
        
    #---------------------------------------------------------------------------
    print('\n*** Testing trigger of the changed() method:')
    set_changed_mask_to_test(self.mMFVec3fFieldMask)
    osg.commitChanges()
    reset_changed_mask()    
    #---------------------------------------------------------------------------
       
    #---------------------------------------------------------------------------
    print('\n*** Testing write access via EditHandle on mMFVec3f (triggers changed() call):')
    mfield = self.edit_mMFVec3f()
    value = osg.Vec3f(7,6,5)
    mfield[0] = value
    if self.mMFVec3f[0] == value:
        print('  read back changed value: "%s" -> SUCCESS' % self.mMFVec3f[0])
    else:
        print('  read back changed value: -> FAILURE')
        
    mfield[0].setValues(5,4,3)
    if self.mMFVec3f[0] == osg.Vec3f(5,4,3):
        print('  read back changed value: "%s" -> SUCCESS' % self.mMFVec3f[0])
    else:
        print('  read back changed value: -> FAILURE')
    #---------------------------------------------------------------------------
    
    #---------------------------------------------------------------------------
    print('\n*** Testing iterator interface:')
    idx = 0
    error_flag = False
    try:
        for i in self.mMFVec3f:
            print('mMFVec3f[%s]: %s' % (idx, i))
            idx += 1
    except:
        error_flag = True
        
    if error_flag == False:
        print('  iterator test: -> SUCCESS')
    else:
        print('  iterator: -> FAILURE')        
    #---------------------------------------------------------------------------
    
    #---------------------------------------------------------------------------
    print('\n*** Testing proxy support of the boost::python::indexing_suite:')
    # taken from http://mail.python.org/pipermail/cplusplus-sig/2009-July/014717.html
    mfield = self.mMFVec3f
    # stores reference to item 7 in tmp
    tmp = mfield[1]
    tmp.setValues(2,2,2)
    # almost certainly causes reallocation of underlying array in c++
    mfield.extend([osg.Vec3f() for i in range(1000)]) 
    length = tmp.length() # -> still works due to indexing_suite proxy magic
    if length == osg.Vec3f(2,2,2).length():
        print('  read back length value: "%s" -> SUCCESS' % tmp.length())
    else:
        print('  read back length value: -> FAILURE')
    #---------------------------------------------------------------------------  
    
def init():
    print('\n----------------------------------------------------------------------')
    print('  SINGLE-FIELD TEST SUITE')
    print('----------------------------------------------------------------------')
    test_single_fields()
    
    print('\n----------------------------------------------------------------------')
    print('  MULTI-FIELD TEST SUITE')
    print('----------------------------------------------------------------------')
    test_multi_fields()
    
def shutdown():
    pass

def frame(timeStamp, frameCount):
    pass

def changed(whichField, origin, details):
    test_changed_mask(whichField)