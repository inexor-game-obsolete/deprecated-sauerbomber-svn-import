import os
import json
import sauerbomber

class Config:

	path = 'repositories/home/config'
	extension = 'json'
	data = {}
	
	def __init__(self, name, path = None):
		self.name = name
		if path != None:
			self.path = path
		else:
			self.path = sauerbomber.configdir()

	def save(self, data):
		f = open("%s/%s.%s" % (self.path, self.name, self.extension), "w")
		json.dump(data, f)
		f.close()
		
	def load(self):
		path = "%s/%s.%s" % (self.path, self.name, self.extension)
		if os.path.exists(path):
			f = open(path, "r")
			self.data = json.load(f)
			f.close()
			return True
		else:
			return False
