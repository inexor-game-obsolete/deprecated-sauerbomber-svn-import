import sauerbomber
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
	