def init():
    pass

def frame(timeStamp, frameCount):
    self.set_mySFDouble(timeStamp)
    self.edit_mySFVec3f().setValues(frameCount, frameCount, frameCount)
    
    mfdouble = self.edit_myMFDouble()
    for idx in range(len(mfdouble)):
        mfdouble[idx] = timeStamp
    
    mfvec3f = self.edit_myMFVec3f()
    for idx in range(len(mfvec3f)):
        mfvec3f[idx].setValues(frameCount, frameCount, frameCount)
    
    node = osg.Node.create()
    osg.setName(node, 'node_' + str(frameCount))
    self.set_mySFNode(node)
    
    mfnode = self.edit_myMFNode()
    for idx in range(len(mfnode)):
        node = osg.Node.create()
        osg.setName(node, 'node_' + str(frameCount))
        mfnode[idx] = node

    #print('myMFNode:')
    #idx = 0
    #for item in self.myMFNode:
    #    print('    [%d]: %s' % (idx, osg.getName(item)))
    #    idx += 1
        
def changed(whichField, origin, details):
    pass

def shutdown():
    pass