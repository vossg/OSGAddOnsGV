#!/usr/bin/env python

import sys
import optparse
import logging
import os

from mod_pywebsocket.dispatch import Dispatcher
from mod_pywebsocket import standalone
from mod_pywebsocket.standalone import WebSocketServer
from mod_pywebsocket import util
from mod_pywebsocket import common

# copied from standalone.py
_DEFAULT_LOG_MAX_BYTES = 1024 * 256
_DEFAULT_LOG_BACKUP_COUNT = 5

_DEFAULT_REQUEST_QUEUE_SIZE = 128

# 1024 is practically large enough to contain WebSocket handshake lines.
_MAX_MEMORIZED_LINES = 1024

class FunctionDispatcher(Dispatcher):
    """ A customized Dispatcher that allows you to register
        a callable to execute when data is received. The
        function also gets a user_data variable as argument
        to exchange (non-threadsafe) data with the calling
        module.
    """
    def __init__(self, callable, user_data):
        Dispatcher.__init__(self, ".")
        self.receive_function = callable
        self.user_data = user_data
        
    def get_handler_suite(self, path):
        return True
    
    def do_extra_handshake(self, request):
        #print("do_extra_handshake")
        return True
        
    def transfer_data(self, request):
        self.receive_function(request, self.user_data)

_GOODBYE_MESSAGE = "Bye bye..."

def my_receive_function(request, user_data):
    print("new client from %s" % 'asdf')
    
    while True:
        try:
            line = request.ws_stream.receive_message()
        except:
            print("exception")
            
        if line is None:
            return
        
        user_data = line
        print("user_data> %s" % userdata)
        
        #if isinstance(line, unicode):
        #    request.ws_stream.send_message(line, binary=False)
        #    if line == _GOODBYE_MESSAGE:
        #        return
        #else:
        #    request.ws_stream.send_message(line, binary=True)

def run(user_data = []):
    parser = optparse.OptionParser()
    parser.add_option('-H', '--server-host', '--server_host',
                      dest='server_host',
                      default='',
                      help='server hostname to listen to')
    parser.add_option('-V', '--validation-host', '--validation_host',
                      dest='validation_host',
                      default=None,
                      help='server hostname to validate in absolute path.')
    parser.add_option('-p', '--port', dest='port', type='int',
                      default=common.DEFAULT_WEB_SOCKET_PORT,
                      help='port to listen to')
    parser.add_option('-P', '--validation-port', '--validation_port',
                      dest='validation_port', type='int',
                      default=None,
                      help='server port to validate in absolute path.')
    parser.add_option('-w', '--websock-handlers', '--websock_handlers',
                      dest='websock_handlers',
                      default='.',
                      help='WebSocket handlers root directory.')
    parser.add_option('-m', '--websock-handlers-map-file',
                      '--websock_handlers_map_file',
                      dest='websock_handlers_map_file',
                      default=None,
                      help=('WebSocket handlers map file. '
                            'Each line consists of alias_resource_path and '
                            'existing_resource_path, separated by spaces.'))
    parser.add_option('-s', '--scan-dir', '--scan_dir', dest='scan_dir',
                      default=None,
                      help=('WebSocket handlers scan directory. '
                            'Must be a directory under websock_handlers.'))
    parser.add_option('-d', '--document-root', '--document_root',
                      dest='document_root', default='.',
                      help='Document root directory.')
    parser.add_option('-x', '--cgi-paths', '--cgi_paths', dest='cgi_paths',
                      default=None,
                      help=('CGI paths relative to document_root.'
                            'Comma-separated. (e.g -x /cgi,/htbin) '
                            'Files under document_root/cgi_path are handled '
                            'as CGI programs. Must be executable.'))
    parser.add_option('-t', '--tls', dest='use_tls', action='store_true',
                      default=False, help='use TLS (wss://)')
    parser.add_option('-k', '--private-key', '--private_key',
                      dest='private_key',
                      default='', help='TLS private key file.')
    parser.add_option('-c', '--certificate', dest='certificate',
                      default='', help='TLS certificate file.')
    parser.add_option('-l', '--log-file', '--log_file', dest='log_file',
                      default='', help='Log file.')
    parser.add_option('--log-level', '--log_level', type='choice',
                      dest='log_level', default='warn',
                      choices=['debug', 'info', 'warning', 'warn', 'error',
                               'critical'],
                      help='Log level.')
    parser.add_option('--log-max', '--log_max', dest='log_max', type='int',
                      default=_DEFAULT_LOG_MAX_BYTES,
                      help='Log maximum bytes')
    parser.add_option('--log-count', '--log_count', dest='log_count',
                      type='int', default=_DEFAULT_LOG_BACKUP_COUNT,
                      help='Log backup count')
    parser.add_option('--allow-draft75', dest='allow_draft75',
                      action='store_true', default=False,
                      help='Allow draft 75 handshake')
    parser.add_option('--strict', dest='strict', action='store_true',
                      default=False, help='Strictly check handshake request')
    parser.add_option('-q', '--queue', dest='request_queue_size', type='int',
                      default=_DEFAULT_REQUEST_QUEUE_SIZE,
                      help='request queue size')
    options = parser.parse_args()[0]
    
    os.chdir(options.document_root)

    standalone._configure_logging(options)

    # TODO(tyoshino): Clean up initialization of CGI related values. Move some
    # of code here to WebSocketRequestHandler class if it's better.
    options.cgi_directories = []
    options.is_executable_method = None
    if options.cgi_paths:
        options.cgi_directories = options.cgi_paths.split(',')
        if sys.platform in ('cygwin', 'win32'):
            cygwin_path = None
            # For Win32 Python, it is expected that CYGWIN_PATH
            # is set to a directory of cygwin binaries.
            # For example, websocket_server.py in Chromium sets CYGWIN_PATH to
            # full path of third_party/cygwin/bin.
            if 'CYGWIN_PATH' in os.environ:
                cygwin_path = os.environ['CYGWIN_PATH']
            util.wrap_popen3_for_win(cygwin_path)

            def __check_script(scriptpath):
                return util.get_script_interp(scriptpath, cygwin_path)

            options.is_executable_method = __check_script

    if options.use_tls:
        if not _HAS_OPEN_SSL:
            logging.critical('To use TLS, install pyOpenSSL.')
            sys.exit(1)
        if not options.private_key or not options.certificate:
            logging.critical(
                    'To use TLS, specify private_key and certificate.')
            sys.exit(1)

    if not options.scan_dir:
        options.scan_dir = options.websock_handlers
        
    try:
        # Share a Dispatcher among request handlers to save time for
        # instantiation.  Dispatcher can be shared because it is thread-safe.
        dump_reveived_data = True
        options.dispatcher = FunctionDispatcher(my_receive_function, user_data)
        standalone._print_warnings_if_any(options.dispatcher)

        server = WebSocketServer(options)
        server.serve_forever()
    except Exception, e:
        logging.critical('mod_pywebsocket: %s' % e)
        logging.critical('mod_pywebsocket: %s' % util.get_stack_trace())
        sys.exit(1)    
    
if __name__ == '__main__':
    run()    
        

