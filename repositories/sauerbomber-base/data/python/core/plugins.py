import os
import sys
import __builtin__
import logging
import inspect
import glob
import string
import sauerbomber
from core.config import Config


paths = [ sauerbomber.pyscriptspath() ]


class SauerbomberPlugin(object):

	name = None
	config = None
	context = None
	requires = []

	def __init__(self, name, config, context):
		self.name = name
		self.config = config
		self.context = context
		logging.info("Initialized plugin %s" %(name))

	def __del__(self):
		logging.info("Destroyed plugin %s" %(self.name))
		
	def require(self, name):
		'''ultra light weight dependency injection'''
		setattr(self, name, self.context.plugins[name])
	
	def autowire(self):
		for name in self.requires:
			self.require(name)

	def init(self):
		'''ensure that the init method exists and can be overwritten by plugins'''
		pass


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
		
	def instantiate(self):
		logging.debug("Trying to start plugin %s" %(self.name))
		try:
			self.package = __import__(self.name)
			self.module = sys.modules[self.name]
			found = False
			for name, obj in inspect.getmembers(self.module):
				logging.debug("Checking for %s in %s..." %(name, self.module))
				if inspect.isclass(obj):
					if issubclass(obj, SauerbomberPlugin):
						self.plugin = obj(self.name, self.config, self.context)
						break
			if found:
				logging.info("Found plugin class for %s" %(name))
			else:
				logging.error("Could not find plugin class for %s" %(name))
		except Exception, e:
			logging.exception(e)

	def deinstantiate(self):
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
				for plugin_type in [ 'base', 'client', 'server' ]:
					for module_path in glob.glob("%s/%s/*" %(path, plugin_type)):
						if os.path.isdir(module_path):
							module = module_path.replace(path, "")[1:].replace("/", ".")
							self.add(module)
		else:
			logging.error("auto detect error")

	def instantiate(self, name):
		self.plugins[name].instantiate()

	def deinstantiate(self, name):
		self.plugins[name].deinstantiate()

	def autowire(self, name):
		self.plugins[name].autowire()
		
	def post_init(self, name):
		self.plugins[name].init()

	def instantiate_all(self):
		for name, plugin in self.plugins.items():
			plugin.instantiate()

	def deinstantiate_all(self):
		for name, plugin in self.plugins.items():
			plugin.deinstantiate()

	def autowire_all(self):
		for name, plugin in self.plugins.items():
			plugin.autowire()

	def post_init_all(self):
		for name, plugin in self.plugins.items():
			plugin.init()

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
		plugin_manager.instantiate_all()
		plugin_manager.autowire_all()
		plugin_manager.post_init_all()
	except Exception, e:
		logging.exception(e)
	
def reload_plugins():
	try:
		plugin_manager.deinstantiate_all()
		load_plugins()
	except Exception, e:
		logging.exception(e)

def unload_plugins():
	try:
		plugin_manager.deinstantiate_all()
	except Exception, e:
		logging.exception(e)
