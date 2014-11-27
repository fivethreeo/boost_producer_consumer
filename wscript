import sys
import os
sys.path.append(os.path.abspath('waf/waflib/extras'))


def options(opt):
    opt.load('cxx msvc boost')
                
def configure(conf):
    conf.load('cxx msvc boost')
    conf.env['MSVC_TARGETS'] = ['x86']
    
    conf.check_libs_msvc('kernel32 user32 gdi32 oleaut32 ole32 comctl32 vfw32 ws2_32 advapi32')
    conf.check_boost(stlib='thread system filesystem')

    conf.env.CXXFLAGS = '/EHsc'
    
    if sys.platform == 'win32':
        pass

def build(bld):

    tg = bld.program(
       features='cxx',
       source='prodcon.cpp',
       target='prodcon',
       use='WS2_32 KERNEL32 USER32 GDI32 ADVAPI32 COMCTL32 OLEAUT32 OLE32 VFW32 BOOST'

    )