def init():
  print('init')

def frame(timeStamp, frameCount):
  print('frame (timeStamp/frameCount): %s / %s' % (timeStamp, frameCount))
  self.set_myField(timeStamp)

def changed(whichField, origin, details):
  print('changed')

def shutdown():
  print('shutdown')