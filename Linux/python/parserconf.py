import ConfigParser
cfg = ConfigParser.ConfigParser()
print cfg.read('conf.ini')
print cfg.sections()
print cfg.get('installation','library')
print cfg.getint('server','port')
print cfg.get('server','signature')
cfg.set('server','port','6666')
import sys
cfg.write(sys.stdout)
