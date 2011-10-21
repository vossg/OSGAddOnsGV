def ptr_test():
    #----------------------------------------------------------------------------------------------
    print('\n*** Testing read access via GetHandle on mSFWeakNodePtr:')
    if self.mSFWeakNodePtr:
        print('  %s -> SUCCESS' % str(self.mSFWeakNodePtr))
    #----------------------------------------------------------------------------------------------
    
    #----------------------------------------------------------------------------------------------
    print('\n*** Testing read access via EditHandle on mSFWeakNodePtr (triggers changed() call):')
    n = self.edit_mSFWeakNodePtr()
    if osg.getName(self.mSFWeakNodePtr) == 'Node02':
        print('  read node name: "%s" -> SUCCESS' % osg.getName(self.mSFWeakNodePtr))
    else:
        print('  read node name: -> FAILURE')
    #----------------------------------------------------------------------------------------------
    
    #----------------------------------------------------------------------------------------------
    print('\n*** Testing write access via EditHandle on mSFWeakNodePtr (triggers changed() call):')
    n = osg.Node.create()
    n.setCore(osg.Group.create())
    osg.setName(n, 'ChangedName')
    self.mSFWeakNodePtr = n
    if osg.getName(self.mSFWeakNodePtr) == 'ChangedName':
        print('  read back changed node name: "%s" -> SUCCESS' % osg.getName(self.mSFWeakNodePtr))
    else:
        print('  read back changed node name: -> FAILURE')
    #----------------------------------------------------------------------------------------------
    
    #----------------------------------------------------------------------------------------------
    print('\n*** Testing read access via GetHandle for mMFUnrecNodePtr')
    mfield = self.get_mMFUnrecNodePtr()
    idx = 6
    for item in mfield:
        if osg.getName(item) == 'Node0' + str(idx):
            print('  Read item name: "%s" -> SUCCESS' % (osg.getName(item)))
        else:
            print('  Read item name: -> FAILURE')
        idx +=1
    #----------------------------------------------------------------------------------------------
    
    #----------------------------------------------------------------------------------------------
    print('\n*** Testing write access on GetHandle: (raises a "TypeError")')
    hasRaisedError = False
    try:
        mfield[0] = osg.Node.create()
    except TypeError:
        print('  TypeError raised -> SUCCESS')
        hasRaisedError = True
        
    if not hasRaisedError:
        print('  TypeError raised -> FAILURE')
    #----------------------------------------------------------------------------------------------
    
    #----------------------------------------------------------------------------------------------  
    print('\n*** Testing access via EditHandle for mMFUnrecNodePtr (triggers changed() call):')
    mfield = self.edit_mMFUnrecNodePtr()
    idx = 6
    for item in mfield:
        if osg.getName(item) == 'Node0' + str(idx):
            print('  Read item name: "%s" -> SUCCESS' % (osg.getName(item)))
        else:
            print('  Read item name: -> FAILURE')
        idx +=1
    #----------------------------------------------------------------------------------------------
    
    #----------------------------------------------------------------------------------------------
    print('\n*** Testing type check when changing values: (raises a "TypeError")')
    mfield = self.edit_mMFUnrecNodePtr()
    hasRaisedError = False
    try:
        mfield[0] = osg.AlgorithmStage.create()
    except TypeError:
        print('  TypeError raised -> SUCCESS')
        hasRaisedError = True
        
    if not hasRaisedError:
        print('  TypeError raised -> FAILURE')
    #----------------------------------------------------------------------------------------------
    
    #----------------------------------------------------------------------------------------------
    print('\n*** Changing content via EditHandle:')
    for idx in range(0, len(mfield)):
        old_node = mfield[idx]
        mfield[idx] = osg.ComplexSceneManager.the().findNamedComponent('Node0' + str(idx))
        print('  Replaced "%s" with "%s"' % \
              (osg.getName(old_node), osg.getName(mfield[idx])))
    size = len(mfield)
    for idx in range(size, size+3):
        mfield.append(osg.ComplexSceneManager.the().findNamedComponent('Node0' + str(idx)))
        print('  Appended node "%s"' % (osg.getName(mfield[-1])))
    #----------------------------------------------------------------------------------------------
    
    #----------------------------------------------------------------------------------------------
    print('\n*** Reading back changed items via new GetHandle:')
    idx = 0
    mfield = self.get_mMFUnrecNodePtr()
    for item in mfield:
        if osg.getName(item) == 'Node0' + str(idx):
            print('  Read item: %s -> SUCCESS' % (osg.getName(item)))
        else:
            print('  Read item: -> FAILURE')
        idx += 1
    #----------------------------------------------------------------------------------------------    

def init():
    ptr_test()

def shutdown():
    pass

def frame(timeStamp, frameCount):
    mfield = 0
    pass

def changed(whichField, origin, details):
    print('\n*** Testing trigger of change in mMFUnrecNodePtr:')
    if whichField & self.mMFUnrecNodePtrFieldMask:
        print('  -> SUCCESS')
    else:
        print('  -> failure')

    print('\n*** Testing trigger of change in mSFWeakNodePtr:')
    if whichField & self.mSFWeakNodePtrFieldMask:
        print('  -> SUCCESS')
    else:
        print('  -> failure')