import json
encoded = json.dumps(obj)
obj = json.loads(encoded)


class Config:

	path = 'repositories/home/config'
	
	def save(self, name, data):
		f = open("%s/%s.json", self.path, name)
		json_data = json.dump(data, f)
		
	def load(self, name):
		f = open("%s/%s.json", self.path, name)
		data = json.load(raw_data)
		return data
