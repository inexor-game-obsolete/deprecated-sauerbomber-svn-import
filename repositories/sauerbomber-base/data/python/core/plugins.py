from ConfigParser import ConfigParser, NoOptionError
import os
import sys
import __builtin__
import logging
import sauerbomber
import core.events
import core.log # Initialize these before loading plugins

plugins = {}
paths = [ sauerbomber.pyscriptspath() ]
config_filename = 'plugin.conf'
init_modules = sys.modules.keys()

class Plugin:
	def __init__(self, path, config_path):
		self.path = path
		conf = ConfigParser()
		conf.read(config_path)
		self.isclientenabled = True
		self.isserverenabled = True
		try:
			self.initmodule = conf.get('Plugin', 'module')
			self.isclientenabled = (conf.get('Plugin', 'client') == 'yes')
			self.isserverenabled = (conf.get('Plugin', 'server') == 'yes')
			self.name = conf.get('Plugin', 'name')
			self.version = conf.get('Plugin', 'version')
			self.author = conf.get('Plugin', 'author')
		except NoOptionError:
			self.isenabled = False
		del conf
	def loadModule(self):
		if self.initmodule and ((self.isclientenabled and sauerbomber.isClient()) or (self.isserverenabled and sauerbomber.isServer())) :
			self.module = __import__(os.path.basename(self.path) + '.' + self.initmodule)
	def unloadModule(self):
		if (self.isclientenabled and sauerbomber.isClient()) or (self.isserverenabled and sauerbomber.isServer()):
			del self.module
	def clientenabled(self):
		return self.isclientenabled
	def serverenabled(self):
		return self.isserverenabled

def plugin(name):
	return plugins[name]

def loadPlugins():
	plugins.clear()
	logging.info('Loading plugins...')
	for path in paths:
		files = os.listdir(path)
		for file in files:
			dirpath = path + '/' + file
			config_path = dirpath + '/' + config_filename
			if os.path.isdir(dirpath) and os.path.exists(config_path):
				p = Plugin(dirpath, config_path)
				if (p.isclientenabled and sauerbomber.isClient()) or (p.isserverenabled and sauerbomber.isServer()):
					plugins[p.name] = p
				else:
					logging.info('Skipping %s plugin' % file)
	logging.info('Found %i plugins' % len(plugins.keys()))
	logging.info('Initializing plugins...')
	for plugin in plugins.values():
		plugin.loadModule()

def reloadPlugins():
	for p in plugins.values():
		p.unloadModule()
	core.events.triggerServerEvent('reload', ())
	for p in plugins.values():
		p.loadModule()

loadPlugins()

