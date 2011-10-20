if(osg.getName(self)):
    self.get_name = osg.getName(self)
else:
    self.get_name = 'unknown'

def init():
    
    #_fieldAccessHandler.testGetPointerMField(0)
    _fieldAccessHandler.testSetPointerMField(1, osg.Node.create())
    #mfield = _fieldAccessHandler.testEditPointerMField()
    #print(mfield.size())
    pass

def shutdown():
    pass

def frame(timeStamp, frameCount):
    return

    if(False):
        if(False):
            print('Reader: mMFInt32 = ' + str(_fieldAccessHandler.myGetMField("mMFInt32", int(0))))
            print('Reader: mMFVec3f = ' + str(_fieldAccessHandler.myGetMField("mMFVec3f", osg.Vec3f())))
            print('Reader:      [3] = ' + str(_fieldAccessHandler.myGetMField("mMFVec3f", osg.Vec3f())[3]))
    
        print('Reader: mMFInt32 = ' + str(self.get_mMFInt32()))
        print('Reader: mMFVec3f = ' + str(self.get_mMFVec3f()))
        print('Reader:      [3] = ' + str(self.get_mMFVec3f()[3]))

    mfNode = self.get_mMFNode()
    print('Reader: mMFNode')
    for node in mfNode:
        print('     node0: ' + osg.getName(mfNode[0]))
        print('     node1: ' + osg.getName(mfNode[1]))
        
    print('Reader: mSFNode.getName() = ' + str(osg.getName(self.get_mSFNode())))
        
def changed(whichField, origin, details):
    pass