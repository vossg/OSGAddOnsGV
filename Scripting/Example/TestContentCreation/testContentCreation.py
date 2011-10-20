from math import sin,cos,floor,pi

_transCList = []

def init():
    global _transCList
    
    rootN = osg.ComplexSceneManager.the().findNamedComponent('ModelRoot')
    if rootN:
        print('Found model root')
    else:
        print('Model root not found. Aborting init()')
        return

    # Populate scene:
    for i in range(int(self.amount)):
        i = 2*pi/self.amount * i
        transC = osg.Transform.create()
        transN = osg.Node.create()
        transN.setCore(transC)
        mat = osg.Matrix()
        mat.setTranslate(self.radius*cos(i),self.radius*sin(i),0)
        transC.setMatrix(mat)
        _transCList.append(transC)

        filename = '../Models/tie.wrl'
        modelN = osg.SceneFileHandler.the().read(filename)
        transN.addChild(modelN)
        rootN.addChild(transN)

def frame(timeStamp, frameCount):
    idx   = 0
    phase = 0.5
    for transC in _transCList:
        mat = transC.getMatrix()
        q   = osg.Quaternion()
        q.setValueAsAxisRad(osg.Vec3f(0,1,0), sin(timeStamp + idx*phase))
        mat.setRotate(q)
        transC.setMatrix(mat)
        idx += 1

    osg.commitChanges()

def changed(whichField, origin, details):
    pass
    
def shutdown():
    pass