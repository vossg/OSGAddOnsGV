def init():
    pass

def frame(timeStamp, frameCount):
    print('\n----------------- FRAME BEGIN --------------------\n')

    print('mySFDouble: %s' % self.mySFDouble)
    print('mySFVec3f : %s' % self.mySFVec3f)
    print('mySFNode  : %s' % osg.getName(self.mySFNode))
    
    print('myMFDouble:')
    idx = 0
    for item in self.myMFDouble:
        print('    [%d]: %s' % (idx, item))
        idx += 1
    
    print('myMFVec3f:')
    idx = 0
    for item in self.myMFVec3f:
        print('    [%d]: %s' % (idx, item))
        idx += 1
              
    print('myMFNode:')
    idx = 0
    for item in self.myMFNode:
        print('    [%d]: %s' % (idx, osg.getName(item)))
        idx += 1
              
    print('\n----------------- FRAME END   --------------------')
    
def changed(whichField, origin, details):
    pass

def shutdown():
    pass