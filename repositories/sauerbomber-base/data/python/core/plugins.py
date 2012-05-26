import os
import sys
import __builtin__
import logging
import inspect
import glob
import string
import sauerbomber

 # Initialize these before loading plugins
import core.events
import core.db

from core.config import Config


paths = [ sauerbomber.pyscriptspath() ]


class SauerbomberPlugin(object):

	name = None
	config = None
	context = None

	def __init__(self, name, config, context):
		self.name = name
		self.config = config
		self.context = context
		logging.info("Initialized plugin %s" %(name))

	def __del__(self):
		logging.info("Destroyed plugin %s" %(self.name))


class Plugin(object):

	name = "unnamed"
	config = None
	context = None
	package = None
	module = None
	plugin = None
	version = 0

	def __init__(self, name, config, context):
		self.name = name
		self.config = config
		self.context = context

	def info(self):
		pass
		
	def start(self):
		logging.info("Trying to start plugin %s" %(self.name))
		try:
			logging.info(sys.path)
			self.package = __import__(self.name)
			self.module = sys.modules[self.name]
			for name, obj in inspect.getmembers(self.module):
				logging.info("Checking for %s in %s..." %(name, self.module))
				if inspect.isclass(obj):
					if issubclass(obj, SauerbomberPlugin):
						self.plugin = obj(self.name, self.config, self.context)
						logging.info("Found plugin class for %s" %(name))
						break
		except Exception, e:
			logging.error("Error: %s"%(e))
			logging.exception(e)

	def stop(self):
		del self.plugin
		del self.module
		del self.package

	def is_started(self):
		return self.plugin != None


class PluginConfig(Config):

	name = None
	path = None

	def __init__(self, name):
		self.name = name
		self.path = "%s/plugins" %(sauerbomber.configdir())


class PluginManager(object):

	config = None
	ready = False
	plugins = {}

	def __init__(self):
		self.clear()
		self.config = Config("PluginManager")
		if self.config.load():
			if self.config.data.has_key('paths'):
				for path in self.config.data['paths']:
					logging.info("Added python plugin search path %s" %(os.path.abspath(path)))
					sys.path.append(os.path.abspath(path))
				self.ready = True
			else:
				logging.error("could not find python plugin paths in config file")
		else:
			logging.error("could not load plugin manager config")

	def clear(self):
		self.plugins = {}

	def add(self, name):
		logging.info("Added plugin %s" %(name))
		config = PluginConfig(name)
		config.load()
		self.plugins[name] = Plugin(name, config, self)
		
	def remove(self, name):
		del self.plugins[name]
		
	def auto_detect(self):
		if self.ready and self.config.data.has_key('paths'):
			for path in self.config.data['paths']:
				logging.info("Autodetecting plugin modules in %s" %(path))
				for plugin_type in [ 'core', 'client', 'server' ]:
					for module_path in glob.glob("%s/%s/*" %(path, plugin_type)):
						if os.path.isdir(module_path):
							module = module_path.replace(path, "")[1:].replace("/", ".")
							self.add(module)
#				for client_module_path in glob.glob("%s/client/*" %(path)):
#					if os.path.isdir(client_module_path):
#						client_module = client_module_path.replace(path, "")[1:].replace("/", ".")
#						self.add(client_module)
#				for server_module_path in glob.glob("%s/server/*" %(path)):
#					if os.path.isdir(server_module_path):
#						server_module = server_module_path.replace(path, "")[1:].replace("/", ".")
#						self.add(server_module)
		else:
			logging.error("auto detect error")

	def start(self, name):
		self.plugins[name].start()
		
	def stop(self, name):
		self.plugins[name].stop()

	def start_all(self):
		for name, plugin in self.plugins.items():
			plugin.start()
		
	def stop_all(self):
		for name, plugin in self.plugins.items():
			plugin.stop()
		

logging.basicConfig(
	filename = 'repositories/home/logs/python.log',
	format = '%(levelname)-10s %(asctime)s %(module)s: %(message)s',
	level = logging.INFO
)

plugin_manager = PluginManager()

def load_plugins():
	'''Loading python plugins. Called by SauerBomber at startup'''
	try:
		plugin_manager.clear()
		plugin_manager.auto_detect()
		plugin_manager.start_all()
	except Exception, e:
		logging.error("Error: %s"%(e))
		logging.exception(e)
	
def reload_plugins():
	try:
		plugin_manager.stop_all()
		plugin_manager.clear()
		plugin_manager.auto_detect()
	except Exception, e:
		logging.error("Error: %s"%(e))
		logging.exception(e)
