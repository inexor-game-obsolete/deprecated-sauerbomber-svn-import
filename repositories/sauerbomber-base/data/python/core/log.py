from core.settings import PluginConfig
import logging
import string
import sbutil

config = PluginConfig('logging')
path = config.getOption('Config', 'path', 'repositories/home/data/logs/python.log')
level = config.getOption('Config', 'level', 'error')
del config

LEVELS = {'debug': logging.DEBUG,
	'info': logging.INFO,
	'warning': logging.WARNING,
	'error': logging.ERROR,
	'critical': logging.CRITICAL}

sbutil.enshure_file(path)

logging.basicConfig(
	filename = path,
	format = '%(levelname)-10s %(asctime)s %(module)s: %(message)s',
	level = LEVELS[level])

