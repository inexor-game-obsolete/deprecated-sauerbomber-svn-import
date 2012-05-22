import json
encoded = json.dumps(obj)
obj = json.loads(encoded)


class Config:

	path = 'repositories/home/config'
	
	def save(self, name, data):
		f = open((%s/%s.json" % (self.path, name), "w")
		json.dump(data, f)
		f.close()
		
	def load(self, name):
		f = open("%s/%s.json" % (self.path, name), "r")
		data = json.load(f)
		f.close()
		return data
