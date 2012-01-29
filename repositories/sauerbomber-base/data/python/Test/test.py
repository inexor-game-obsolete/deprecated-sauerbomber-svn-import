from core.events import registerServerEventHandler
#from core.settings import PluginConfig
import logging
import sauerbomber
import string
import random

# config = PluginConfig('test')

#def onMapChangePre():
#    print 'onMapChangePre'
#    logging.info('onMapChangePre')
#
#x = 0

#def onMapChange(name, mode):
#    print 'TEST onMapChange: %s %s' %(name, mode)
#    logging.info('onMapChange: %s %s' %(name, mode))
#    sauerbomber.switchName('Hanack %d'%(random.randint(1, 99)))
#    sauerbomber.showScores()
#    sauerbomber.switchPlayerModel(random.randint(0, 3))
#    sauerbomber.switchTeam('t %d'%(random.randint(1, 999)))
#    print 'Client cn: 0 Name: %s Team: %s Model: %s Icon: %s'%(sauerbomber.getClientName(0), sauerbomber.getClientTeam(0), sauerbomber.getClientModel(0), sauerbomber.getClientIcon(0))
#    mm = sauerbomber.getMasterMode()
#    print 'Mastermode: %d (%s)'%(mm, sauerbomber.masterModeName(mm))
#    logging.info('onMapChange A')
#    print 'onMapChange End'
#    logging.info('onMapChange End')

#def onIntermissionEnded():
#    print 'TEST onIntermissionEnded'

#def onPlayerNameChanged(cn, oldname, newname):
#    print 'onPlayerNameChanged cn: %d oldname: %s newname: %s'%(cn, oldname, newname)

#def onTrySetmaster(m):
#    if m == 1:
#        sauerbomber.setMasterMode(1)
#        sauerbomber.guiClear()
#        sauerbomber.guiTab("Info")
#        # sauerbomber.guiPushList()
#        sauerbomber.guiText("setmaster", 255, "0")
#        sauerbomber.guiBar()
#        sauerbomber.guiText("on", 0, "0")
#        # sauerbomber.guiPopList()
#        sauerbomber.guiTab("Info2")
#        sauerbomber.guiText("Blah", 255, "0")
#        sauerbomber.guiShow(True)
#    else:
#        sauerbomber.setMasterMode(0)
#        sauerbomber.guiClear()
#        sauerbomber.guiTab("Info")
#        # sauerbomber.guiPushList()
#        sauerbomber.guiText("setmaster", 255, "0")
#        sauerbomber.guiBar()
#        sauerbomber.guiText("off", 0, "0")
#        # sauerbomber.guiPopList()
#        sauerbomber.guiShow(True)

#def onPlayerMessage(cn, text):
#    sauerbomber.message(text)
# registerServerEventHandler('map_changed_pre', onMapChangePre)
#registerServerEventHandler('map_changed', onMapChange)

# registerServerEventHandler('player_name_changed', onPlayerNameChanged)

# registerServerEventHandler('player_frag', onMapChange)
#registerServerEventHandler('intermission_ended', onIntermissionEnded)

# registerServerEventHandler('try_setmaster', onTrySetmaster)

# registerServerEventHandler('player_message', onPlayerMessage)
