#-------------------------------------------------------------------------------
# Changed mask helper:

changed_mask_to_test = 0

def to_binary_rep(s):
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

if(osg.getName(self)):
    self.name = osg.getName(self)
else:
    self.name = 'unknown'

def init():
    #---------------------------------------------------------------------------
    print('\n*** Testing static field masks:')
    print('Priority: Id/Mask = %s/%s' % (self.PriorityFieldId, to_binary_rep(self.PriorityFieldMask)))
    print('Script  : Id/Mask = %s/%s' % (self.ScriptFieldId  , to_binary_rep(self.ScriptFieldMask  )))
    #---------------------------------------------------------------------------
    
    #---------------------------------------------------------------------------
    print('\n*** Testing dynamic field masks:')
    print('Field1: Id/Mask = %s/%s' % (self.Field1FieldId, to_binary_rep(self.Field1FieldMask)))
    print('Field2: Id/Mask = %s/%s' % (self.Field2FieldId, to_binary_rep(self.Field2FieldMask)))
    #---------------------------------------------------------------------------

    #---------------------------------------------------------------------------
    print('\n*** Testing changed() for Field1:')
    self.set_Field1(3)
    set_changed_mask_to_test(self.Field1FieldMask)
    osg.commitChanges()
    reset_changed_mask()  
    #---------------------------------------------------------------------------
    
def shutdown():
    pass

def frame(timeStamp, frameCount):
    pass

def changed(whichField, origin, details):
    test_changed_mask(whichField)
