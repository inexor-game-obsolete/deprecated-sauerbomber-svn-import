from core.db import dbmanager
from core.net import ipLongToString, ipStringToLong
from core.timers import addTimer
from sqlalchemy import Column, Integer, String, Boolean, ForeignKey
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm.exc import NoResultFound, MultipleResultsFound


import time, string
import logging
import sauerbomber

Base = declarative_base()
session = dbmanager.session()

class Map(Base):
	__tablename__='maps'
	id = Column(Integer, primary_key=True)
	name = Column(String, index=True)
	def __init__(self, name):
		self.name = name

class MapModes(Base):
	__tablename__='map_modes'
	id = Column(Integer, primary_key=True)
	name = Column(String)
	mode = Column(String)
	def __init__(self, name, mode):
		self.nick = nick
		self.reason = reason

def getCurrentBanByIp(ipaddress):
	return dbmanager.query(Ban).filter(Ban.ip==ipaddress).filter('expiration>'+str(time.time())).one()

def getCurrentBanByNick(nick):
	return dbmanager.query(BanNick).filter(BanNick.nick==nick).one()

def isIpBanned(ipaddress):
	try:
		b = getCurrentBanByIp(ipaddress)
		return True
	except NoResultFound:
		return False
	except MultipleResultsFound:
		return True

def isNickBanned(nick):
	try:
		b = getCurrentBanByNick(nick)
		return True
	except NoResultFound:
		return False
	except MultipleResultsFound:
		return True

def ban(cn, seconds, reason, banner_cn):
	ip = sauerbomber.playerIpLong(cn)
	expiration = time.time() + seconds
	nick = sauerbomber.playerName(cn)
	if banner_cn != -1:
		banner_ip = sauerbomber.playerIpLong(banner_cn)
		banner_nick = sauerbomber.playerName(banner_cn)
	else:
		banner_ip = 0
		banner_nick = ''
	newban = Ban(ip, expiration, reason, nick, banner_ip, banner_nick, time.time())
	session.add(newban)
	session.commit()
	addTimer(200, sauerbomber.playerKick, (cn,))
	logging.info('Player %s (%s) banned for %s by %s (%s)',
		nick,
		ipLongToString(ip),
		reason,
		banner_nick,
		ipLongToString(banner_ip))
	sauerbomber.message(info(ban_message.substitute(colordict, name=nick, seconds=seconds, reason=reason)))

Base.metadata.create_all(dbmanager.engine)

