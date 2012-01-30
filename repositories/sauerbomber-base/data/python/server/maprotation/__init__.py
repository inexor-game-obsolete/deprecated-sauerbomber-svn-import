import logging
import random
import sauerbomber
from core.events import registerServerEventHandler
from server.commands import commandHandler, UsageError, registerCommandHandler
from server.players import masterRequired

@commandHandler('changemap')
@masterRequired
def changemap(cn, args):
    args = args.split(' ')
    sauerbomber.message(args[0])
    sauerbomber.message(args[1])
    sauerbomber.setMap(args[0], int(args[1]))
	
registerCommandHandler('changemap', changemap)

#@commandHandler('changetime')
#@masterRequired
#def changetime(cn, args):
#    args = args.split(' ')
#    sauerbomber.message(args[0])
#    sauerbomber.message(args[1])
#    sauerbomber.setMap(args[0], int(args[1]))
#registerCommandHandler('changetime', changetime)

maps = {
    'bomberman': [ 'gier', 'vendetta', 'marrakesch' ],
    'hideandseek': [ 'core_transfer', 'xenon', 'marrakesch' ]
}

def getNextMap(mode):
    logging.info('getNextMap')
    modename = sauerbomber.modeName(mode)
    nextmap = maps[modename][random.randint(0, len(maps[modename])-1)]
    if nextmap == sauerbomber.mapName():
        return getNextMap(mode)
    else:
        logging.info('next map for mode %s (%d) is: %s' %(modename, mode, nextmap))
        return nextmap

def rotate():
    logging.info('rotate')
    mode = sauerbomber.gameMode()
    sauerbomber.setMap(getNextMap(mode), mode)
    logging.info('rotated')

def onNoClients():
    logging.info('onNoClients')
    rotate()

def onIntermissionBegin():
    logging.info('onIntermissionBegin')
    rotate()

def onIntermissionEnded():
    logging.info('onIntermissionEnded')

def onMasterModeChanged(mm):
    logging.info('onMasterModeChanged to %d'%(mm))

registerServerEventHandler('no_clients', onNoClients)
registerServerEventHandler('intermission_begin', onIntermissionBegin)
registerServerEventHandler('intermission_ended', onIntermissionBegin)
registerServerEventHandler('server_mastermode_changed', onMasterModeChanged)
