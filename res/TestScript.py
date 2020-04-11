print("Hello from module")
from direct.showbase.Loader import Loader
from direct.showbase.DirectObject import DirectObject

class TestScript(DirectObject):
	def __init__(self, *args, **kwargs):
		print("Hello from __init__")
		self.loader = Loader(self)
		panda = self.loader.loadModel("panda")
		# `render` here is a global variable injected from C++
		panda.reparentTo(render)
		panda.setPos(0, 50, 0)