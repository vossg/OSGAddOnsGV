from math import sin

def init():
    pass	

def frame(timeStamp, frameCount):
    self.edit_myPosition().setValues(0,0,sin(timeStamp))

def changed(whichField, origin, details):
    pass

def shutdown():
    pass